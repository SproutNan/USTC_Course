`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/04/24 18:41:29
// Design Name: 
// Module Name: ID_EX
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


module ID_EX
    (
    input clk,
    input rst,
    input [31:0] npc_i,
    input [31:0] reg_a_i,
    input [31:0] reg_b_i,
    input [31:0] imm_i,
    input [31:0] ir_i,
    output reg [31:0] npc,
    output reg [31:0] reg_a,
    output reg [31:0] reg_b,
    output reg [31:0] imm,
    output reg [31:0] ir,
    
    // CONTROLLER_EX
    input alu_a_sel_i,
    input alu_b_sel_i,
    input [3:0] alu_ctrl_i,
    output reg alu_a_sel,
    output reg alu_b_sel,
    output reg [3:0] alu_ctrl,
    
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
    
    // CONTROLLER_EX
    always@(posedge clk) begin
        alu_a_sel <= alu_a_sel_i;
        alu_b_sel <= alu_b_sel_i;
        alu_ctrl <= alu_ctrl_i;
    end
    
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
            reg_a <= 32'h12345678;
            reg_b <= 32'h87654321;
            imm <= 32'h0ABCDEF0;
            ir <= 32'h0;
        end
        else begin
            npc <= npc_i;
            reg_a <= reg_a_i;
            reg_b <= reg_b_i;
            imm <= imm_i;
            ir <= ir_i;
        end
    end
endmodule
