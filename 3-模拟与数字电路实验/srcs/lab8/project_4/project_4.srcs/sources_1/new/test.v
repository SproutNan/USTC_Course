`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2021/12/08 16:55:50
// Design Name: 
// Module Name: test
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


module test(
	input CLK100MHZ,
	input dir,
	input BTN,
	output reg [2:0] hexplay_an,
	output reg [3:0] hexplay_data
);
reg [4:0] CS = 4'h0;
reg [4:0] NS;

reg OUT = 0;
reg [3:0] NUM = 4'h0;
reg [15:0] HISTORY = 16'h0000;

reg in = 0;

//获得btn按下的边沿
reg BTNR1,BTNR2;
wire BTNEDGE;
always@(posedge CLK100MHZ) begin
    BTNR1<=BTN;
    BTNR2<=BTNR1;
end
assign BTNEDGE = BTNR1&(~BTNR2);


always@(posedge CLK100MHZ) begin
    if(BTNEDGE) begin
        HISTORY <= {HISTORY[11:0],3'b0,dir};
        if(dir == 0) begin
            case(CS)
                4'h0: NS <= 4'h0;
                4'h1: NS <= 4'h0;
                4'h2: NS <= 4'h3;
                4'h3: begin
                        NS <= 4'h0;
                        NUM = NUM + 1;
                      end
            endcase
        end
        else begin
            case(CS)
                4'h0: NS <= 4'h1;
                4'h1: NS <= 4'h2;
                4'h2: NS <= 4'h2;
                4'h3: NS <= 4'h1;
            endcase
         end
    end
    else NS <= NS;
end
         
always@(posedge CLK100MHZ) begin
    CS<=NS;
end

reg [4:0] hexplay_cnt;

always@(posedge CLK100MHZ) begin
	if (hexplay_cnt == 5'b10111)
		hexplay_cnt <= 0;
	else
		hexplay_cnt <= hexplay_cnt + 1;
end

always@(posedge CLK100MHZ) begin
    hexplay_an = hexplay_cnt[4:2];
	case(hexplay_an)
	   3'b000: hexplay_data = CS;
	   3'b001: hexplay_data = NUM;
	   3'b010: hexplay_data = HISTORY[3:0];
	   3'b011: hexplay_data = HISTORY[7:4];
	   3'b100: hexplay_data = HISTORY[11:8];
	   3'b101: hexplay_data = HISTORY[15:12];
	   default: hexplay_data = 4'b0000;
    endcase
end
endmodule
