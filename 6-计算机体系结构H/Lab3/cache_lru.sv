// LRU strategy

module cache_lru #(
    parameter  LINE_ADDR_LEN = 3,               // length of line_addr
    parameter  SET_ADDR_LEN  = 2,  // length of set_addr
    //parameter  TAG_ADDR_LEN  = 6,               // length of tag_addr
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
//localparam MEM_ADDR_LEN    = TAG_ADDR_LEN + SET_ADDR_LEN ; // length of mem_addr, mem has 2^MEM_ADDR_LEN lines
localparam UNUSED_ADDR_LEN = 32 - TAG_ADDR_LEN - SET_ADDR_LEN - LINE_ADDR_LEN - 2 ;

localparam LINE_SIZE       = 1 << LINE_ADDR_LEN  ;         // word count in a line: 2^LINE_ADDR_LEN
localparam SET_SIZE        = 1 << SET_ADDR_LEN   ;         // set count: 2^SET_ADDR_LEN

reg [            31:0] cache_mem    [SET_SIZE][WAY_CNT][LINE_SIZE]; // SET_SIZE sets, each set has WAY_CNT ways, each way has LINE_SIZE lines
reg [TAG_ADDR_LEN-1:0] cache_tags   [SET_SIZE][WAY_CNT];            // SET_SIZE * WAY_CNT tags, each way has one tag
reg                    valid        [SET_SIZE][WAY_CNT];            // SET_SIZE * WAY_CNT valid bits, each way has one valid bit
reg                    dirty        [SET_SIZE][WAY_CNT];            // SET_SIZE * WAY_CNT dirty bits, each way has one dirty bit
reg [     WAY_CNT-1:0] lru_output   [SET_SIZE][WAY_CNT];            // use matrix alg for ALU
reg [ WAY_CNT_LEN-1:0] next_out;                                    // right swap-out way
reg [ WAY_CNT_LEN-1:0] last_in_way;                                 // store `next_out` to SWAP_IN_OK state


wire [              2-1:0]   word_addr;
wire [  LINE_ADDR_LEN-1:0]   line_addr;
wire [   SET_ADDR_LEN-1:0]    set_addr;
wire [   TAG_ADDR_LEN-1:0]    tag_addr;
wire [UNUSED_ADDR_LEN-1:0] unused_addr;

enum  {IDLE, SWAP_OUT, SWAP_IN, SWAP_IN_OK} cache_stat;  // cache state machine

reg  [   SET_ADDR_LEN-1:0] mem_rd_set_addr = 0;
reg  [   TAG_ADDR_LEN-1:0] mem_rd_tag_addr = 0;
wire [   MEM_ADDR_LEN-1:0] mem_rd_addr = {mem_rd_tag_addr, mem_rd_set_addr};
reg  [   MEM_ADDR_LEN-1:0] mem_wr_addr = 0;

reg  [31:0] mem_wr_line [LINE_SIZE];
wire [31:0] mem_rd_line [LINE_SIZE];

wire mem_gnt;     // grant signal with memory

assign {unused_addr, tag_addr, set_addr, line_addr, word_addr} = addr;  // 32bit ADDR

reg cache_hit = 1'b0;
reg [ WAY_CNT_LEN-1:0] cache_hit_way = 0; // way cnt if cache hit

always @ (*) begin              // parallel comparsion to find hit way
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
            for (integer j = 0; j < WAY_CNT; j++) begin
                dirty[i][j] <= 1'b0;
                valid[i][j] <= 1'b0;
                lru_output[i][j] <= 0;
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
                            if(rd_req) begin    // cache hit & read req, read from cache directly
                                rd_data <= cache_mem[set_addr][cache_hit_way][line_addr];
                            end 
                            else if(wr_req) begin // cache hit & write req, write in cache directly
                                cache_mem[set_addr][cache_hit_way][line_addr] <= wr_data;   // 
                                dirty[set_addr][cache_hit_way] <= 1'b1; // marks dirty
                            end 
                        end 
                        else begin
                            if(wr_req | rd_req) begin   // cache miss & write/read req
                                // using LRU to determine where new way to place
                                for(integer j = 0; j < WAY_CNT; j++) begin
                                    if(lru_output[set_addr][j]==0) begin
                                        next_out <= j;
                                    end
                                end
                                // if the new way to be placed is valid & dirty, 
                                if(valid[set_addr][next_out] & dirty[set_addr][next_out]) begin
                                    cache_stat  <= SWAP_OUT;
                                    mem_wr_addr <= {cache_tags[set_addr][next_out], set_addr};
                                    mem_wr_line <= cache_mem[set_addr][next_out];
                                end // else no need to swap-out
                                else begin
                                    cache_stat  <= SWAP_IN;
                                end
                                last_in_way <= next_out;
                                {mem_rd_tag_addr, mem_rd_set_addr} <= {tag_addr, set_addr};
                            end
                        end
                    end
        SWAP_OUT:   begin
                        if(mem_gnt) begin           // swap-out success, jump to next
                            cache_stat <= SWAP_IN;
                        end
                    end
        SWAP_IN:    begin
                        if(mem_gnt) begin           // swap-in success, jump to next
                            cache_stat <= SWAP_IN_OK;
                        end
                    end
        SWAP_IN_OK: begin           // last swap-in success, write line-from-mem to cache, update tag/valid/dirty
                        for(integer i=0; i<LINE_SIZE; i++)  cache_mem[mem_rd_set_addr][last_in_way][i] <= mem_rd_line[i];
                        cache_tags[mem_rd_set_addr][last_in_way] <= mem_rd_tag_addr;
                        valid     [mem_rd_set_addr][last_in_way] <= 1'b1;
                        dirty     [mem_rd_set_addr][last_in_way] <= 1'b0;
                        // update lru widget
                        for(integer j=0; j<WAY_CNT; j++) begin
                            if (j == last_in_way) lru_output[mem_rd_set_addr][last_in_way] <= ~(1 << (WAY_CNT - 1 - last_in_way));
                            else lru_output[mem_rd_set_addr][j] <= lru_output[mem_rd_set_addr][j] & ~(1 << (WAY_CNT - 1 - last_in_way));
                        end
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





