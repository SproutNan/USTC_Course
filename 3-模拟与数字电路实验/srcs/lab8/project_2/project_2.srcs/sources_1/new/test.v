`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2021/12/08 15:30:38
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


module test(input sw,output [3:0] cnt);
reg [3:0] cn;
always@(posedge sw) begin
    cn<=cn+1;
end
always@(negedge sw) begin
    cn<=cn+1;
end
assign cnt = cn;
endmodule
