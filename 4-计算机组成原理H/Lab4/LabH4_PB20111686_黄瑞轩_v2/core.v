`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/04/08 19:46:10
// Design Name: 
// Module Name: core
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


module core
    (
    input clk,
    input rst,
    
    //IO_BUS
    output [7:0] io_addr, //外设地址
    output [31:0] io_dout, //向外设输出的数据
    output io_we, //向外设输出数据时的写使能信号
    output io_rd, //从外设输入数据时的读使能信号
    input [31:0] io_din, //来自外设输入的数据
    
    //Debug_BUS
    output [31:0] chk_pc,//当前执行指令地址
    input [15:0] chk_addr, //数据通路状态的编码地址
    output reg [31:0] chk_data //数据通路状态的数据
    );
    

    
    wire branch;
    wire [31:0] pc;
    wire [31:0] pc_plus4;
    wire [31:0] inst;
    wire [31:0] imm_out;
    wire rf_wr_en;
    wire alu_a_sel;
    wire alu_b_sel;
    wire [3:0] alu_ctrl;
    wire [2:0] dm_rd_ctrl;
    wire [1:0] dm_wr_ctrl;
    wire [1:0] rf_wr_sel;
    wire [2:0] comp_ctrl;
    wire do_branch;
    wire do_jump;
    
    reg [31:0] rf_wd3;
    wire [31:0] rf_rd1, rf_rd2;
    wire [31:0] alu_a, alu_b, alu_out;
    wire [31:0] dm_dout;
    
    pc_calc inst_pc(
    .clk(clk),
    .rst(rst),
    .branch(branch),
    .alu_out(alu_out),
    .pc(pc),
    .pc_plus4(pc_plus4)
    );
    
    reg [9:0] Debug_DM_addr = 0;
    wire [31:0] Debug_DM_out;
    
    mem_data_c inst_data(
    .clk(clk),
    .dm_rd_ctrl(dm_rd_ctrl),
    .dm_wr_ctrl(dm_wr_ctrl),
    .dm_addr(alu_out),
    .dm_din(rf_rd2),
    .dm_dout(dm_dout),
    .debug_ra(Debug_DM_addr),
    .debug_rd(Debug_DM_out),
    .io_rd(io_rd),
    .io_we(io_we),
    .io_din(io_din),
    .io_dout(io_dout),
    .io_addr(io_addr)
    );
    
    mem_inst_c inst_inst(
    .clk(clk),
    .im_addr(pc),
    .im_dout(inst)
    );
    
    imm inst_imm(
    .inst(inst),
    .imm_out(imm_out)
    );
    
    controller inst_controller(
    .inst(inst),
    .rf_wr_en(rf_wr_en),
    .alu_a_sel(alu_a_sel),
    .alu_b_sel(alu_b_sel),
    .alu_ctrl(alu_ctrl),
    .dm_rd_ctrl(dm_rd_ctrl),
    .dm_wr_ctrl(dm_wr_ctrl),
    .rf_wr_sel(rf_wr_sel),
    .comp_ctrl(comp_ctrl),
    .do_branch(do_branch),
    .do_jump(do_jump)
    );
    
    always@(*) begin
        case (rf_wr_sel)
            2'b00: rf_wd3 = 32'h0;
            2'b01: rf_wd3 = pc_plus4;
            2'b10: rf_wd3 = alu_out;
            2'b11: rf_wd3 = dm_dout;
        endcase
    end
    
    reg [4:0] Debug_RF_addr = 0;
    wire [31:0] Debug_RF_out;
    
    reg_file inst_rf(
    .clk(clk),
    .a1(inst[19:15]),
    .a2(inst[24:20]),
    .a3(inst[11:7]),
    .debug_ra(Debug_RF_addr),
    .debug_rd(Debug_RF_out),
    .wd3(rf_wd3),
    .we3(rf_wr_en),
    .rd1(rf_rd1),
    .rd2(rf_rd2)
    );
    
    assign alu_a = alu_a_sel ? rf_rd1 : pc;
    assign alu_b = alu_b_sel ? imm_out : rf_rd2;
    
    alu inst_alu(
    .a(alu_a),
    .b(alu_b),
    .alu_ctrl(alu_ctrl),
    .alu_out(alu_out)
    );
    
    brah inst_brah(
    .a(rf_rd1),
    .b(rf_rd2),
    .comp_ctrl(comp_ctrl),
    .do_branch(do_branch),
    .do_jump(do_jump),
    .branch(branch)
    );

    //Debug_BUS
    assign chk_pc = pc;
    always@(*) begin
        if (chk_addr[15:12] == 4'h0) begin
            case (chk_addr[3:0])
                4'h0: chk_data = pc + 32'h4;
                4'h1: chk_data = pc;
                4'h2: chk_data = inst;
                4'h3: chk_data = {13'h0, rf_wr_en, alu_a_sel, alu_b_sel, alu_ctrl[3:0], dm_rd_ctrl[2:0], dm_wr_ctrl[1:0], rf_wr_sel[1:0], comp_ctrl[2:0], do_branch, do_jump};
                4'h4: chk_data = rf_rd1;
                4'h5: chk_data = rf_rd2;
                4'h6: chk_data = imm_out;
                4'h7: chk_data = alu_out;
                4'h8: chk_data = dm_dout;
                default: chk_data = 32'h0ABCDEF0;
            endcase
        end
        else if (chk_addr[15:12] == 4'h1) begin
            Debug_RF_addr = chk_addr[4:0];
            chk_data = Debug_RF_out;
        end
        else if (chk_addr[15:12] == 4'h2) begin
            Debug_DM_addr = chk_addr[9:0];
            chk_data = Debug_DM_out;
        end
        else chk_data = 32'h0ABCDEF0;
    end
    
    //IO_BUS
endmodule
