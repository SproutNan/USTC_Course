`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2021/12/13 22:58:43
// Design Name: 
// Module Name: myISA
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module rx(
    input clk, rst,
    input rx,
    output reg rx_vld,
    output reg [7:0] rx_data
);
    parameter DIV_CNT = 10'd867;
    parameter HDIV_CNT = 10'd433;
    parameter RX_CNT = 4'h8;
    parameter C_IDLE = 1'b0;
    parameter C_RX = 1'b1; 
    reg curr_state;
    reg next_state;
    reg [9:0] div_cnt;
    reg [3:0] rx_cnt;
    reg rx_reg_0,rx_reg_1,rx_reg_2,rx_reg_3,rx_reg_4,rx_reg_5,rx_reg_6,rx_reg_7;
    wire rx_pulse;
    always@(posedge clk or posedge rst)
        begin
            if(rst)
                curr_state <= C_IDLE;
            else
                curr_state <= next_state;
        end 
    
    always@(*)
        begin
            case(curr_state)
                C_IDLE:
                    if(div_cnt == HDIV_CNT)
                        next_state = C_RX;
                    else
                        next_state = C_IDLE;
                C_RX:
                    if((div_cnt == DIV_CNT) && (rx_cnt >= RX_CNT))
                        next_state = C_IDLE;
                    else
                        next_state = C_RX;
            endcase
        end
    
    always@(posedge clk or posedge rst)
        begin
            if(rst)
                div_cnt <= 10'h0;
            else if(curr_state == C_IDLE)
                begin
                    if(rx==1'b1)
                        div_cnt <= 10'h0;
                    else if(div_cnt < HDIV_CNT)
                        div_cnt <= div_cnt + 10'h1;
                    else
                        div_cnt <= 10'h0; 
                end
            else if(curr_state == C_RX)
                begin
                    if(div_cnt >= DIV_CNT)
                        div_cnt <= 10'h0;
                    else
                        div_cnt <= div_cnt + 10'h1;
                end
        end

    always@(posedge clk or posedge rst)
        begin
            if(rst)
                rx_cnt <= 4'h0;
            else if(curr_state == C_IDLE)
                rx_cnt <= 4'h0;
            else if((div_cnt == DIV_CNT) && (rx_cnt < 4'hF))
                rx_cnt <= rx_cnt + 1'b1; 
        end

        assign rx_pulse = (curr_state == C_RX) && (div_cnt == DIV_CNT);

    always@(posedge clk)
        begin
            if(rx_pulse)
                begin
                    case(rx_cnt)
                        4'h0: rx_reg_0 <= rx;
                        4'h1: rx_reg_1 <= rx;
                        4'h2: rx_reg_2 <= rx;
                        4'h3: rx_reg_3 <= rx;
                        4'h4: rx_reg_4 <= rx;
                        4'h5: rx_reg_5 <= rx;
                        4'h6: rx_reg_6 <= rx;
                        4'h7: rx_reg_7 <= rx;
                    endcase
                end
        end

    always@(posedge clk or posedge rst)
        begin
            if(rst)
                begin
                    rx_vld <= 1'b0;
                    rx_data <= 8'h55;
                end 
            else if((curr_state==C_RX)&&(next_state==C_IDLE))
                begin
                    rx_vld <= 1'b1;
                    rx_data <= {rx_reg_7,rx_reg_6,rx_reg_5,rx_reg_4,rx_reg_3,rx_reg_2,rx_reg_1,rx_reg_0};
                end
            else
                rx_vld <= 1'b0;
        end
endmodule

//顶层模块
module top(
    input clk,
    input rx,
    output reg [7:0] led,
    input [7:0] sw,
	output reg [2:0] hexplay_an,
	output reg [3:0] hexplay_data
);
    reg [31:0] data;//七段数码管
    //LC-3寄存器
    reg [15:0] mem[63:0];
    reg [5:0] PC;//没有ENDL，最后写一行HALT就行
    reg [5:0] ptr;
    reg [2:0] CC;
    reg [15:0] IR;
    reg [15:0] R[7:0];
    initial begin
        PC = 0;
        ptr = 0;
        CC = 3'b000;
        IR = 0;
        R[7] = 0;
        R[6] = 0;
        R[5] = 0;
        R[4] = 0;
        R[3] = 0;
        R[2] = 0;
        R[1] = 0;
        R[0] = 0;
    end
    wire [7:0] trap_vec8;
    wire ben;
    assign trap_vec8 = IR[7:0];
    
    //缓冲区
    reg [15:0] buffer = 0;
    reg [3:0] buffer_ptr;
    initial buffer_ptr = 4'b1111;

    wire [7:0] rx_data;
    rx rx_inst(.clk (clk),.rst (rst),.rx (rx),.rx_vld (rx_vld),.rx_data (rx_data));

    //LC-3状态机
    reg [1:0] cs;//current_state
    reg [1:0] ns;//next_state
    parameter S_idle = 2'd0;
    parameter S_fetch = 2'd1;
    parameter S_carry = 2'd2;
    parameter S_setcc = 2'd3;
    initial cs = S_idle;
    
    parameter INST_ADD = 4'b0001;
    parameter INST_AND = 4'b0101;
    parameter INST_BR = 4'b1000;
    parameter INST_JMP = 4'b1100;
    parameter INST_JSR = 4'b0100;
    parameter INST_LD = 4'b0010;
    parameter INST_LDI = 4'b1010;
    parameter INST_LDR = 4'b0110;
    parameter INST_LEA = 4'b1110;
    parameter INST_NOT = 4'b1001;
    parameter INST_MOD = 4'b0000;
    parameter INST_ST = 4'b0011;
    parameter INST_STI = 4'b1011;
    parameter INST_STR = 4'b0111;
    parameter INST_TRAP = 4'b1111;//全1是输入结束标记，1111000000000000，用&R来判断
    parameter INST_RSF = 4'b1101;//右移，1101 DR SR ...

    //状态机第一部分
    always@(*)begin
        case(cs)
            S_idle:begin
                if(rx_vld && rx_data == 8'h0a && buffer == 16'b1111111111111111) 
                    ns = S_fetch;
                else ns = S_idle;
            end
            S_fetch:begin
                ns = S_carry;
            end
            S_carry:begin
                case(IR[15:12])
                    INST_ADD, INST_AND, INST_LD, INST_LDI, INST_LDR, INST_NOT, INST_RSF: ns = S_setcc;
                    INST_TRAP: ns = S_idle;
                    default: ns = S_fetch;
                endcase
            end
            S_setcc: ns = S_fetch;
            default: ns = S_idle;
        endcase
    end
    //状态机第二部分
    always@(posedge clk)begin
        if(cs == S_idle && ns == S_carry) begin
            //buffer <= 0;
        end
        cs <= ns;
    end

    //状态机第三部分
    always@(posedge clk)begin
        led <= {5'b0, CC};
        case(cs)
        S_idle: begin
            if(rx_vld)begin
                if(rx_data == 8'h0a)begin
                    buffer_ptr <= 4'b1111;
                    if(buffer!=16'b1111111111111111)begin
                        mem[ptr] <= buffer;
                        ptr <= ptr + 1;
                    end
                end
                else begin
                    buffer[buffer_ptr] <= rx_data[0];
                    buffer_ptr <= buffer_ptr - 1;
                end
            end
            casex(sw)
                8'bxxxxxxx1: data <= {4'ha,4'h0,8'h0,R[0]};
                8'bxxxxxx1x: data <= {4'ha,4'h1,8'h0,R[1]};
                8'bxxxxx1xx: data <= {4'ha,4'h2,8'h0,R[2]};
                8'bxxxx1xxx: data <= {4'ha,4'h3,8'h0,R[3]};
                8'bxxx1xxxx: data <= {4'ha,4'h4,8'h0,R[4]};
                8'bxx1xxxxx: data <= {4'ha,4'h5,8'h0,R[5]};
                8'bx1xxxxxx: data <= {4'ha,4'h6,8'h0,R[6]};
                8'b1xxxxxxx: data <= {4'ha,4'h7,8'h0,R[7]};
                default: data <= {4'hc,2'h0,ptr,4'h0,mem[ptr-1]};
            endcase
        end
        S_fetch:begin
            IR <= mem[PC];
            PC <= PC + 1;
        end
        S_setcc:begin
            if(R[IR[11:9]] == 0) CC <= 3'b010;
            else if(R[IR[11:9]][15]) CC <= 3'b100;
            else CC <= 3'b001;
        end
        S_carry:begin
            case(IR[15:12])
                INST_ADD:begin
                    if(IR[5] == 1'b1)begin
                        R[IR[11:9]] <= R[IR[8:6]] + {{11{IR[4]}}, IR[4:0]};
                    end
                    else begin
                        R[IR[11:9]] <= R[IR[8:6]] + R[IR[2:0]];
                    end
                end
                INST_AND:begin
                    if(IR[5] == 1'b1)begin
                        R[IR[11:9]] <= R[IR[8:6]] & {{11{IR[4]}}, IR[4:0]};
                    end
                    else begin
                        R[IR[11:9]] <= R[IR[8:6]] & R[IR[2:0]];
                    end
                end
                INST_BR:begin
                    if((IR[11] & CC[2]) | (IR[10] & CC[1]) | (IR[9] & CC[0])) begin
                        PC <= PC + IR[5:0];
                        R[3'b110] <= 16'hFFFF;
                    end
                end
                INST_JMP:begin
                    PC <= R[IR[8:6]][5:0];
                end
                INST_JSR:begin
                    R[7] <= PC;
                    if(IR[11]) PC <= PC + IR[5:0];
                    else PC <= R[IR[8:6]][5:0];
                end
                INST_LD:begin
                    R[IR[11:9]] <= mem[PC + IR[5:0]];
                end
                INST_LDI:begin
                    R[IR[11:9]] <= mem[mem[PC + IR[5:0]][5:0]];
                end
                INST_LDR:begin
                    R[IR[11:9]] <= mem[R[IR[8:6]][5:0]+IR[5:0]];
                end
                INST_LEA:begin
                    R[IR[11:9]] <= PC + IR[5:0];
                end
                INST_NOT:begin
                    R[IR[11:9]] <= ~R[IR[8:6]];
                end
                INST_ST:begin
                    mem[PC + IR[5:0]] <= R[IR[11:9]];
                end
                INST_STI:begin
                    mem[mem[PC + IR[5:0]][5:0]] <= R[IR[11:9]];
                end
                INST_STR:begin
                    mem[R[IR[8:6]][5:0] + IR[5:0]] <= R[IR[11:9]];
                end
                INST_RSF:begin
                    R[IR[11:9]] <= {1'b0, R[IR[8:6]][7:1]};
                end
                INST_TRAP:begin
                    //PC <= ENDL;
                end
                INST_MOD:begin
                    R[IR[11:9]] <= R[IR[8:6]] % R[IR[2:0]];
                end
                //default: PC <= PC;
            endcase
        end
        endcase
    end
    
    
    //七段数码管
    
    reg [32:0] hexplay_cnt;
    always@(posedge clk) begin
        if (hexplay_cnt >= (2000000 / 8)) hexplay_cnt <= 0;
        else hexplay_cnt <= hexplay_cnt + 1;
    end
    always@(posedge clk) begin
        if (hexplay_cnt == 0)begin
		    if (hexplay_an == 7)hexplay_an <= 0;
		    else hexplay_an <= hexplay_an + 1;
	    end
    end
    always@(*) begin
	    case(hexplay_an)
		    0: hexplay_data = data[3:0];
		    1: hexplay_data = data[7:4];
		    2: hexplay_data = data[11:8];
		    3: hexplay_data = data[15:12];
		    4: hexplay_data = data[19:16];
		    5: hexplay_data = data[23:20];
		    6: hexplay_data = data[27:24];
		    7: hexplay_data = data[31:28];
	    endcase
    end
endmodule

