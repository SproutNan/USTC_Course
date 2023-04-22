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
input rst,
input [7:0] sw,
output reg [7:0] led);
always@(posedge clk or posedge rst)
begin
if(rst) led<=8'haa;
else led<={sw[0],sw[1],sw[2],sw[3],sw[4],sw[5],sw[6],sw[7]};
end
endmodule
