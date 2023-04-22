`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2021/11/20 21:49:55
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
input clk,
output reg [7:0] led);
reg [31:0] ctr = 0;
always@(posedge clk)ctr<=ctr+1;
always@(posedge clk)
begin
led<={ctr[31],ctr[30],ctr[29],ctr[28],ctr[27],ctr[26],ctr[25],ctr[24]};
end
endmodule
