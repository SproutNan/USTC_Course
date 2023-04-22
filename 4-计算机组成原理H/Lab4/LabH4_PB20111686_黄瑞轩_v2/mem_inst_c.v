`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/04/08 20:30:56
// Design Name: 
// Module Name: mem_inst_c
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


module mem_inst_c(
    input clk,
    input [31:0] im_addr,
    output [31:0] im_dout
    );
    
    wire [31:0] out;
    
    MEM_INST mem(
    .clk(clk),
    .a(im_addr[9:2]),
    .spo(out)
    );
    
    assign im_dout = (im_addr[31] & (~(|im_addr[30:10]))) ? out : 32'h0;
endmodule
