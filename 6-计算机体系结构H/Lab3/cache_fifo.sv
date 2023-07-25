// FIFO strategy

module cache_fifo #(
    parameter  LINE_ADDR_LEN = 3,
    parameter  SET_ADDR_LEN  = 2,
    //parameter  TAG_ADDR_LEN  = 6,
    parameter  WAY_CNT       = 2,
    parameter  WAY_CNT_LEN   = 2
)(
    input  clk, rst,
    output miss, 
    input  [31:0] addr,       
    input  rd_req,            
    output reg [31:0] rd_data,
    input  wr_req,            
    input  [31:0] wr_data   
);
localparam TAG_ADDR_LEN = 11 - SET_ADDR_LEN - LINE_ADDR_LEN;
localparam MEM_ADDR_LEN    = 11 - LINE_ADDR_LEN ;
localparam UNUSED_ADDR_LEN = 32 - TAG_ADDR_LEN - SET_ADDR_LEN - LINE_ADDR_LEN - 2 ;     

localparam LINE_SIZE       = 1 << LINE_ADDR_LEN  ;     
localparam SET_SIZE        = 1 << SET_ADDR_LEN   ;      

reg [            31:0] cache_mem    [SET_SIZE][WAY_CNT][LINE_SIZE]; 
reg [TAG_ADDR_LEN-1:0] cache_tags   [SET_SIZE][WAY_CNT];         
reg                    valid        [SET_SIZE][WAY_CNT];        
reg                    dirty        [SET_SIZE][WAY_CNT];          
reg [ WAY_CNT_LEN-1:0] next_out     [SET_SIZE]; // fifo pointer, representing next swap_in location
reg [ WAY_CNT_LEN-1:0] last_in_way;             // last swap in location


wire [              2-1:0]   word_addr;  
wire [  LINE_ADDR_LEN-1:0]   line_addr;
wire [   SET_ADDR_LEN-1:0]    set_addr;
wire [   TAG_ADDR_LEN-1:0]    tag_addr;
wire [UNUSED_ADDR_LEN-1:0] unused_addr;

enum  {IDLE, SWAP_OUT, SWAP_IN, SWAP_IN_OK} cache_stat; 

reg  [   SET_ADDR_LEN-1:0] mem_rd_set_addr = 0;
reg  [   TAG_ADDR_LEN-1:0] mem_rd_tag_addr = 0;
wire [   MEM_ADDR_LEN-1:0] mem_rd_addr = {mem_rd_tag_addr, mem_rd_set_addr};
reg  [   MEM_ADDR_LEN-1:0] mem_wr_addr = 0;

reg  [31:0] mem_wr_line [LINE_SIZE];
wire [31:0] mem_rd_line [LINE_SIZE];

wire mem_gnt; 

assign {unused_addr, tag_addr, set_addr, line_addr, word_addr} = addr; 

reg cache_hit = 1'b0;
reg [ WAY_CNT_LEN-1:0] cache_hit_way = 0;

always @ (*) begin 
    cache_hit = 1'b0;
    cache_hit_way = 0;
    for(integer i = 0; i < WAY_CNT; i++) begin
        if(valid[set_addr][i] && cache_tags[set_addr][i] == tag_addr) begin
            cache_hit = 1'b1;
            cache_hit_way = i;
        end
    end
end

always @ (posedge clk or posedge rst) begin 
    if(rst) begin
        cache_stat <= IDLE;
        for(integer i = 0; i < SET_SIZE; i++) begin
            next_out[i] <= 0;
            for (integer j = 0; j < WAY_CNT; j++) begin
                dirty[i][j] <= 1'b0;
                valid[i][j] <= 1'b0;
            end
        end
        for(integer k = 0; k < LINE_SIZE; k++)
            mem_wr_line[k] <= 0;
        mem_wr_addr <= 0;
        {mem_rd_tag_addr, mem_rd_set_addr} <= 0;
        rd_data <= 0;
    end 
    else begin
        case(cache_stat)
        IDLE:       begin
                        if(cache_hit) begin
                            if(rd_req) begin 
                                rd_data <= cache_mem[set_addr][cache_hit_way][line_addr]; 
                            end 
                            else if(wr_req) begin 
                                cache_mem[set_addr][cache_hit_way][line_addr] <= wr_data; 
                                dirty[set_addr][cache_hit_way] <= 1'b1; 
                            end 
                        end 
                        else begin
                            if(wr_req | rd_req) begin 
                                // use fifo
                                last_in_way <= next_out[set_addr];
                                next_out[set_addr] <= (next_out[set_addr] + 1) % WAY_CNT;
                                
                                if(valid[set_addr][next_out[set_addr]] & dirty[set_addr][next_out[set_addr]]) begin
                                    cache_stat  <= SWAP_OUT;
                                    mem_wr_addr <= {cache_tags[set_addr][next_out[set_addr]], set_addr};
                                    mem_wr_line <= cache_mem[set_addr][next_out[set_addr]];
                                end // else no need to swap-out
                                else begin
                                    cache_stat  <= SWAP_IN;
                                end
                                {mem_rd_tag_addr, mem_rd_set_addr} <= {tag_addr, set_addr};
                            end
                        end
                    end
        SWAP_OUT:   begin
                        if(mem_gnt) begin  
                            cache_stat <= SWAP_IN;
                        end
                    end
        SWAP_IN:    begin
                        if(mem_gnt) begin 
                            cache_stat <= SWAP_IN_OK;
                        end
                    end
        SWAP_IN_OK: begin 
                        for(integer i=0; i<LINE_SIZE; i++)  cache_mem[mem_rd_set_addr][last_in_way][i] <= mem_rd_line[i];
                        cache_tags[mem_rd_set_addr][last_in_way] <= mem_rd_tag_addr;
                        valid     [mem_rd_set_addr][last_in_way] <= 1'b1;
                        dirty     [mem_rd_set_addr][last_in_way] <= 1'b0;
                        cache_stat <= IDLE; 
                    end
        endcase
    end
end

wire mem_rd_req = (cache_stat == SWAP_IN );
wire mem_wr_req = (cache_stat == SWAP_OUT);
wire [   MEM_ADDR_LEN-1 :0] mem_addr = mem_rd_req ? mem_rd_addr : ( mem_wr_req ? mem_wr_addr : 0);

assign miss = (rd_req | wr_req) & ~(cache_hit && cache_stat==IDLE) ; 

main_mem #( 
    .LINE_ADDR_LEN  ( LINE_ADDR_LEN          ),
    .ADDR_LEN       ( MEM_ADDR_LEN           )
) main_mem_instance (
    .clk            ( clk                    ),
    .rst            ( rst                    ),
    .gnt            ( mem_gnt                ),
    .addr           ( mem_addr               ),
    .rd_req         ( mem_rd_req             ),
    .rd_line        ( mem_rd_line            ),
    .wr_req         ( mem_wr_req             ),
    .wr_line        ( mem_wr_line            )
);

endmodule





