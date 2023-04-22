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
	input rst,
	input BTN,
	output reg [2:0] hexplay_an,
	output reg [3:0] hexplay_data
);

reg [7:0] data;
reg [32:0] hexplay_cnt;

always@(posedge CLK100MHZ) begin
	if (hexplay_cnt >= (2000000 / 8))
		hexplay_cnt <= 0;
	else
		hexplay_cnt <= hexplay_cnt + 1;
end

always@(posedge CLK100MHZ) begin
	if (hexplay_cnt == 0)begin
		if (hexplay_an == 1)
			hexplay_an <= 0;
		else
			hexplay_an <= hexplay_an + 1;
	end
end

always@(*) begin
	case(hexplay_an)
		0: hexplay_data = data[3:0];
		1: hexplay_data = data[7:4];
	endcase
end

reg [26:0] timer_cnt;
always@(posedge CLK100MHZ) begin
	if (timer_cnt >= 100000000)
		timer_cnt <= 0;
	else
		timer_cnt <= timer_cnt + 1;
end
reg [3:0] cnt;
always@(posedge CLK100MHZ) begin
if(BTN == 1'b0) cnt<=4'h0;
else if(cnt<4'h8)cnt<=cnt+1;
end
wire BTN_c;
assign BTN_c = cnt[3];
reg BTN_R1,BTR_R2;
 wire BTN_EDGE;
 
 always @(posedge CLK100MHZ) BTN_R1 <= BTN_c;
 always @(posedge CLK100MHZ) BTR_R2 <= BTN_R1;
 assign BTN_EDGE = BTN_R1 & (~BTR_R2);
 
always@(posedge CLK100MHZ) begin
    if(rst) data<=8'h1F;
	else if (BTN_EDGE) begin
		if (!dir) begin
			data <= data - 1;
		end
		else begin
			data <= data + 1;
		end
	end
end

endmodule
