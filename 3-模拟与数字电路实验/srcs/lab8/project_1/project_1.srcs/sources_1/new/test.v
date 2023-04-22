`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2021/12/08 14:48:37
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


module test(input clk, rst, output reg led);
	parameter ZE = 2'b00;
	parameter UN = 2'b01;
	parameter DE = 2'b10;
	parameter TR = 2'b11;
	reg [1:0] CS = 2'b00;
	reg [1:0] NS;
	always@(*) begin
		case(CS)
			ZE: NS = UN;
			UN: NS = DE;
			DE: NS = TR;
			TR: NS = ZE;
		endcase
end
always@(posedge clk or posedge rst) begin
	if(rst)
		CS <= ZE;
	else
		CS <= NS;
end
always@(*) led <= (CS == TR) ? 1'b1 : 1'b0;
endmodule

