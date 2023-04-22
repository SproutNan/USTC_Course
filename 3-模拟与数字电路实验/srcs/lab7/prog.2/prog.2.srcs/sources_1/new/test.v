`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2021/12/02 13:34:09
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
	input [7:0] sw,
	output reg [2:0] hexplay_an,
	output reg [3:0] hexplay_data
);
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
			hexplay_an <= hexplay_an+1;
	end
end

always@(*) begin
	case(hexplay_an)
		1: hexplay_data = {sw[7],sw[6],sw[5],sw[4]};
		0: hexplay_data = {sw[3],sw[2],sw[1],sw[0]};
	endcase
end

endmodule
