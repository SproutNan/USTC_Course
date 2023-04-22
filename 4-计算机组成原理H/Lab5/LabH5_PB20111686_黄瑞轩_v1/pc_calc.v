`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/04/06 16:34:41
// Design Name: 
// Module Name: pc_calc
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


module pc_calc
    (
    input Hazard,
    input clk,
    input rst, // 注意是正有效
    input branch,
    input [31:0] alu_out,
    output reg [31:0] pc,
    output [31:0] pc_plus4
    );
    
    always@(posedge clk or posedge rst) begin
        if (rst) pc <= 32'h8000_0000;
        else if (branch) pc <= alu_out;
        else begin
            if (Hazard) pc <= pc;
            else pc <= pc_plus4;
        end
    end
    
    assign pc_plus4 = pc + 32'h4;
    
endmodule
