`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2021/11/17 14:19:27
// Design Name: 
// Module Name: decoder
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


module decoder(
input [2:0] in,
output [7:0] out
    );
    assign out[0] = ~in[2]&~in[1]&~in[0];
    assign out[1] = ~in[2]&~in[1]&in[0];
    assign out[2] = ~in[2]&in[1]&~in[0];
    assign out[3] = ~in[2]&in[1]&in[0];
    assign out[4] = in[2]&~in[1]&~in[0];
    assign out[5] = in[2]&~in[1]&in[0];
    assign out[6] = in[2]&in[1]&~in[0];
    assign out[7] = in[2]&in[1]&in[0];
endmodule
