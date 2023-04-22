`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/04/25 17:03:36
// Design Name: 
// Module Name: cpu_tb
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


module cpu_tb();
    reg clk = 0, rst = 1;
    reg setp = 0, data = 0;
    reg [15:0] x = 0;
    //cpu inst(.clk(clk), .rstn(~rst), .step(setp), .x(x), .data(data));
    core inst_core(.clk(clk), .rst(rst));
    always #1 clk = ~clk;
    initial #3 rst = 0;
    initial begin
        #1000 x = 16'b10;
        #1000 data = 1;
        #5000 setp = 1;
        #1000 setp = 0;
        #5000 setp = 1;
        #1000 setp = 0;
        #5000 setp = 1;
        #1000 setp = 0;
        #5000 setp = 1;
        #1000 setp = 0;
    end
endmodule
