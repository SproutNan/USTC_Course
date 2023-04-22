`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/04/24 19:04:21
// Design Name: 
// Module Name: MEM_WB
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


module MEM_WB
    (
    input clk,
    input rst,
    input npc_i,
    input [31:0] alu_out_i,
    input [31:0] mdr_i,
    input [31:0] ir_i,
    output reg [31:0] npc,
    output reg [31:0] alu_out,
    output reg [31:0] mdr,
    output reg [31:0] ir,
    
    // CONRTOLLER_WB
    input rf_wr_en_i,
    input [1:0] rf_wr_sel_i,
    output reg rf_wr_en,
    output reg [1:0] rf_wr_sel
    );
    
    // CONTROLLER_WB
    always@(posedge clk) begin
        rf_wr_en <= rf_wr_en_i;
        rf_wr_sel <= rf_wr_sel_i;
    end
    
    always@(posedge clk or posedge rst) begin
        if (rst) begin
            mdr <= 32'h8000_0000;
            alu_out <= 32'h12345678;
            ir <= 32'h0;
            npc <= 32'h80000000;
        end
        else begin
            mdr <= mdr_i;
            alu_out <= alu_out_i;
            ir <= ir_i;
            npc <= npc_i;
        end
    end
endmodule
