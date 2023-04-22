`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/04/06 17:25:43
// Design Name: 
// Module Name: reg_file
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


module reg_file
    (
    input clk,
    input [4:0] a1, a2, a3, debug_ra, 
    input [31:0] wd3,
    input we3,
    output [31:0] rd1, rd2, debug_rd
    );
    
    reg [31:0] reg_file[0:31];
    integer i;
    initial begin
        for(i=0;i<32;i=i+1) reg_file[i] = 0;
    end
    
    always@(posedge clk) begin
        if (we3 && (|a3)) reg_file[a3] <= wd3;
    end
    
    assign rd1 = a1 ? reg_file[a1] : 32'h0;
    assign rd2 = a2 ? reg_file[a2] : 32'h0;
    assign debug_rd = debug_ra ? reg_file[debug_ra] : 32'h0;
    
endmodule
