`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/04/24 19:04:21
// Design Name: 
// Module Name: EX_MEM
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


module EX_MEM
    (
    input clk,
    input rst,
    input [31:0] npc_i,
    input alu_zero_i,
    input [31:0] alu_out_i,
    input [31:0] reg_b_i,
    input [31:0] ir_i,
    output reg [31:0] npc,
    output reg alu_zero,
    output reg [31:0] alu_out,
    output reg [31:0] reg_b,
    output reg [31:0] ir,
    
    // CONTROLLER_M
    input [2:0] comp_ctrl_i,
    input do_branch_i,
    input do_jump_i,
    input [2:0] dm_rd_ctrl_i,
    input [1:0] dm_wr_ctrl_i,
    output reg [2:0] comp_ctrl,
    output reg do_branch,
    output reg do_jump,
    output reg [2:0] dm_rd_ctrl,
    output reg [1:0] dm_wr_ctrl,
    
    // CONRTOLLER_WB
    input rf_wr_en_i,
    input [1:0] rf_wr_sel_i,
    output reg rf_wr_en,
    output reg [1:0] rf_wr_sel
    );
    
    // CONTROLLER_M
    always@(posedge clk) begin
        comp_ctrl <= comp_ctrl_i;
        do_branch <= do_branch_i;
        do_jump <= do_jump_i;
        dm_rd_ctrl <= dm_rd_ctrl_i;
        dm_wr_ctrl <= dm_wr_ctrl_i;
    end
    
    // CONTROLLER_WB
    always@(posedge clk) begin
        rf_wr_en <= rf_wr_en_i;
        rf_wr_sel <= rf_wr_sel_i;
    end
    
    always@(posedge clk or posedge rst) begin
        if (rst) begin
            npc <= 32'h8000_0000;
            alu_zero <= 1'b0;
            alu_out <= 32'h12345678;
            reg_b <= 32'h87654321;
            ir <= 32'h0;
        end
        else begin
            npc <= npc_i;
            alu_zero <= alu_zero_i;
            alu_out <= alu_out_i;
            reg_b <= reg_b_i;
            ir <= ir_i;
        end
    end
endmodule
