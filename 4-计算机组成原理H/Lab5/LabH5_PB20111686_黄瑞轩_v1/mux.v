`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/04/06 16:17:41
// Design Name: 
// Module Name: mux
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


module mux #(parameter WIDTH = 32)
    (
    input [WIDTH-1:0] a,
    input [WIDTH-1:0] b,
    input [WIDTH-1:0] c,
    input [WIDTH-1:0] d,
    input [1:0] sel,
    output reg [WIDTH-1:0] o
    );
    
    always@(*) begin
        case (sel)
            2'b00: o = a;
            2'b01: o = b;
            2'b10: o = c;
            2'b11: o = d;
        endcase
    end
    
endmodule
