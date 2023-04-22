`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2021/11/08 21:02:28
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
reg clk, rst_n, d;
wire q;
d_ff inst(.clk(clk),.rst_n(rst_n),.d(d),.q(q));
initial
begin
clk = 0;
rst_n = 0;
d = 0;
end
always #5 clk = ~clk;
initial #27 rst_n = 1;
initial
begin 
#12 d = 1;
#24 d = 0;
#20 $finish;
end
endmodule