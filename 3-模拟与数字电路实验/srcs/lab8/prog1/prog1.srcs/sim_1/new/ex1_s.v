`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2021/12/08 14:41:09
// Design Name: 
// Module Name: ex1_s
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


module ex1_s();
reg clk = 1'b0;
wire led;
always@(*) #5 clk = ~clk;
ex1_v ex1(.clk(clk),.led(led));
endmodule
