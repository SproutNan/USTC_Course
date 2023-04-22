`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2021/11/17 14:37:15
// Design Name: 
// Module Name: test_bench
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


module test_bench();
reg clk;
reg [2:0] in;
wire [7:0] out;
decoder inst(.in(in),.out(out));
initial clk = 0;
initial in = 3'b000;
always #5 clk = ~clk;
always@(posedge clk) in = in + 1'b1;
endmodule
