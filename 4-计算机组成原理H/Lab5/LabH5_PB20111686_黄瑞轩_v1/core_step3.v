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
    wire alu_zero_out;
    reg [31:0] alu_a, alu_b;
    wire [31:0] alu_out;
    wire [31:0] dm_dout;
    
    ////////////////////////////////////////////
    // Forward & Hazard Detection输出端口
    ////////////////////////////////////////////
    wire [1:0] ForwardA;
    wire [1:0] ForwardB;
    wire Hazard;

    ////////////////////////////////////////////
    // DEBUG总线
    ////////////////////////////////////////////
    reg [9:0] Debug_DM_addr = 0;
    wire [31:0] Debug_DM_out;
    
    ////////////////////////////////////////////
    // IF/ID的输出端口
    ////////////////////////////////////////////
    wire [31:0] IF_ID_OUT_npc;
    wire [31:0] IF_ID_OUT_ir;
    wire [31:0] adder_out;
    assign adder_out = imm_out + IF_ID_OUT_npc;
    
    ////////////////////////////////////////////
    // ID/EX的输出端口
    ////////////////////////////////////////////
    wire [31:0] ID_EX_OUT_npc;
    wire [31:0] ID_EX_OUT_reg_a;
    wire [31:0] ID_EX_OUT_reg_b;
    wire [31:0] ID_EX_OUT_imm;
    wire [31:0] ID_EX_OUT_ir;
    // CONTROLLER_EX
    wire ID_EX_OUT_alu_a_sel;
    wire ID_EX_OUT_alu_b_sel;
    wire [3:0] ID_EX_OUT_alu_ctrl;
    // CONTROLLER_M
    wire [2:0] ID_EX_OUT_comp_ctrl;
    wire ID_EX_OUT_do_branch;
    wire ID_EX_OUT_do_jump;
    wire [2:0] ID_EX_OUT_dm_rd_ctrl;
    wire [1:0] ID_EX_OUT_dm_wr_ctrl;
    // CONTROLLER_WB
    wire ID_EX_OUT_rf_wr_en;
    wire [1:0] ID_EX_OUT_rf_wr_sel;
    
    ////////////////////////////////////////////
    // EX/MEM的输出端口
    ////////////////////////////////////////////
    wire [31:0] EX_MEM_OUT_npc;
    wire EX_MEM_OUT_alu_zero;
    wire [31:0] EX_MEM_OUT_alu_out;
    wire [31:0] EX_MEM_OUT_reg_b;
    wire [31:0] EX_MEM_OUT_ir;
    // CONTROLLER_M
    wire [2:0] EX_MEM_OUT_comp_ctrl;
    wire EX_MEM_OUT_do_branch;
    wire EX_MEM_OUT_do_jump;
    wire [2:0] EX_MEM_OUT_dm_rd_ctrl;
    wire [1:0] EX_MEM_OUT_dm_wr_ctrl;
    // CONTROLLER_WB
    wire EX_MEM_OUT_rf_wr_en;
    wire [1:0] EX_MEM_OUT_rf_wr_sel;
    
    ////////////////////////////////////////////
    // MEM_WB输出端口
    ////////////////////////////////////////////
    wire [31:0] MEM_WB_OUT_alu_out;
    wire [31:0] MEM_WB_OUT_mdr;
    wire [31:0] MEM_WB_OUT_ir;
    wire [31:0] MEM_WB_OUT_npc;
    // CONTROLLER_WB
    wire MEM_WB_OUT_rf_wr_en;
    wire [1:0] MEM_WB_OUT_rf_wr_sel;
    
        
    pc_calc inst_pc(
    .Hazard(Hazard),
    .clk(clk),
    .rst(rst),
    .branch(branch),
    .alu_out(adder_out),
    .pc(pc),
    .pc_plus4(pc_plus4)
    );
    
    mem_data_c inst_data(
    .clk(clk),
    .dm_rd_ctrl(EX_MEM_OUT_dm_rd_ctrl),
    .dm_wr_ctrl(EX_MEM_OUT_dm_wr_ctrl),
    .dm_addr(EX_MEM_OUT_alu_out),
    .dm_din(EX_MEM_OUT_reg_b),
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
    .inst(IF_ID_OUT_ir),
    .imm_out(imm_out)
    );
    
    controller inst_controller(
    .inst(IF_ID_OUT_ir),
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
    
    ////////////////////////////////////////////
    // 流水段寄存器
    ////////////////////////////////////////////
    
    // IF_ID
    IF_ID IF_ID_INST(
    .Flush(branch),
    .Hazard(Hazard),
    .clk(clk), 
    .rst(rst),
    .npc_i(pc),
    .ir_i(inst),
    .npc(IF_ID_OUT_npc),
    .ir(IF_ID_OUT_ir)
    );
    
    ID_EX ID_EX_INST(
    .clk(clk),
    .rst(rst), 
    .npc_i(IF_ID_OUT_npc),
    .reg_a_i(rf_rd1),
    .reg_b_i(rf_rd2),
    .imm_i(imm_out),
    .ir_i(IF_ID_OUT_ir),
    .npc(ID_EX_OUT_npc),
    .reg_a(ID_EX_OUT_reg_a),
    .reg_b(ID_EX_OUT_reg_b),
    .imm(ID_EX_OUT_imm),
    .ir(ID_EX_OUT_ir),
    // CONTROLLER_EX
    .alu_a_sel_i(Hazard ? 1'b0 : alu_a_sel),
    .alu_b_sel_i(Hazard ? 1'b0 : alu_b_sel),
    .alu_ctrl_i(Hazard ? 4'b0 : alu_ctrl),
    .alu_a_sel(ID_EX_OUT_alu_a_sel),
    .alu_b_sel(ID_EX_OUT_alu_b_sel),
    .alu_ctrl(ID_EX_OUT_alu_ctrl),
    // CONTROLLER_M
    .comp_ctrl_i(Hazard ? 3'b0 : comp_ctrl),
    .do_branch_i(Hazard ? 1'b0 : do_branch),
    .do_jump_i(Hazard ? 1'b0 : do_jump),
    .dm_rd_ctrl_i(Hazard ? 3'b0 : dm_rd_ctrl),
    .dm_wr_ctrl_i(Hazard ? 2'b0 : dm_wr_ctrl),
    .comp_ctrl(ID_EX_OUT_comp_ctrl),
    .do_branch(ID_EX_OUT_do_branch),
    .do_jump(ID_EX_OUT_do_jump),
    .dm_rd_ctrl(ID_EX_OUT_dm_rd_ctrl),
    .dm_wr_ctrl(ID_EX_OUT_dm_wr_ctrl),
    // CONRTOLLER_WB
    .rf_wr_en_i(Hazard ? 1'b0 : rf_wr_en),
    .rf_wr_sel_i(Hazard ? 2'b0 : rf_wr_sel),
    .rf_wr_en(ID_EX_OUT_rf_wr_en),
    .rf_wr_sel(ID_EX_OUT_rf_wr_sel)
    );
    
    EX_MEM EX_MEM_INST(
    .clk(clk),
    .rst(rst), 
    .npc_i(ID_EX_OUT_npc),
    .alu_zero_i(alu_zero_out),
    .alu_out_i(alu_out),
    .reg_b_i(ID_EX_OUT_reg_b),
    .ir_i(ID_EX_OUT_ir),
    .npc(EX_MEM_OUT_npc),
    .alu_zero(EX_MEM_OUT_alu_zero),
    .alu_out(EX_MEM_OUT_alu_out),
    .reg_b(EX_MEM_OUT_reg_b),
    .ir(EX_MEM_OUT_ir),
    // CONTROLLER_M
    .comp_ctrl_i(ID_EX_OUT_comp_ctrl),
    .do_branch_i(ID_EX_OUT_do_branch),
    .do_jump_i(ID_EX_OUT_do_jump),
    .dm_rd_ctrl_i(ID_EX_OUT_dm_rd_ctrl),
    .dm_wr_ctrl_i(ID_EX_OUT_dm_wr_ctrl),
    .comp_ctrl(EX_MEM_OUT_comp_ctrl),
    .do_branch(EX_MEM_OUT_do_branch),
    .do_jump(EX_MEM_OUT_do_jump),
    .dm_rd_ctrl(EX_MEM_OUT_dm_rd_ctrl),
    .dm_wr_ctrl(EX_MEM_OUT_dm_wr_ctrl),
    // CONRTOLLER_WB
    .rf_wr_en_i(ID_EX_OUT_rf_wr_en),
    .rf_wr_sel_i(ID_EX_OUT_rf_wr_sel),
    .rf_wr_en(EX_MEM_OUT_rf_wr_en),
    .rf_wr_sel(EX_MEM_OUT_rf_wr_sel)
    );
    
    
    MEM_WB MEM_WB_INST(
    .clk(clk),
    .rst(rst),
    .npc_i(EX_MEM_OUT_npc),
    .alu_out_i(EX_MEM_OUT_alu_out),
    .mdr_i(dm_dout),
    .ir_i(EX_MEM_OUT_ir),
    .npc(MEM_WB_OUT_npc),
    .alu_out(MEM_WB_OUT_alu_out),
    .ir(MEM_WB_OUT_ir),
    .mdr(MEM_WB_OUT_mdr),
    // CONRTOLLER_WB
    .rf_wr_en_i(EX_MEM_OUT_rf_wr_en),
    .rf_wr_sel_i(EX_MEM_OUT_rf_wr_sel),
    .rf_wr_en(MEM_WB_OUT_rf_wr_en),
    .rf_wr_sel(MEM_WB_OUT_rf_wr_sel)
    );
    
    always@(*) begin
        case (MEM_WB_OUT_rf_wr_sel)
            2'b00: rf_wd3 = 32'h0;
            2'b01: rf_wd3 = MEM_WB_OUT_npc + 32'h4;
            2'b10: rf_wd3 = MEM_WB_OUT_alu_out;
            2'b11: rf_wd3 = MEM_WB_OUT_mdr;
        endcase
    end
    
    reg [4:0] Debug_RF_addr = 0;
    wire [31:0] Debug_RF_out;
    
    reg_file inst_rf(
    .clk(clk),
    .a1(IF_ID_OUT_ir[19:15]),
    .a2(IF_ID_OUT_ir[24:20]),
    .a3(MEM_WB_OUT_ir[11:7]),
    .debug_ra(Debug_RF_addr),
    .debug_rd(Debug_RF_out),
    .wd3(rf_wd3),
    .we3(MEM_WB_OUT_rf_wr_en),
    .rd1(rf_rd1),
    .rd2(rf_rd2)
    );
    
    ///////////////////////////////////////////////////
    // ALU前递相关
    ///////////////////////////////////////////////////
    always@(*) begin
        case (ForwardA)
            2'b10: alu_a = EX_MEM_OUT_alu_out;
            2'b01: alu_a = rf_wd3;
            default: alu_a = ID_EX_OUT_alu_a_sel ? ID_EX_OUT_reg_a : ID_EX_OUT_npc;
        endcase
        
        case (ForwardB)
            2'b10: alu_b = EX_MEM_OUT_alu_out;
            2'b01: alu_b = rf_wd3;
            default: alu_b = ID_EX_OUT_alu_b_sel ? ID_EX_OUT_imm : ID_EX_OUT_reg_b;
        endcase
    end
//    assign alu_a = 
//    assign alu_b = 
    
    alu inst_alu(
    .a(alu_a),
    .b(alu_b),
    .alu_ctrl(ID_EX_OUT_alu_ctrl),
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
    
    forward inst_forward(
    .EX_MEM_RegWrite(EX_MEM_OUT_rf_wr_en),
    .MEM_WB_RegWrite(MEM_WB_OUT_rf_wr_en),
    .EX_MEM_RegisterRd(EX_MEM_OUT_ir[11:7]),
    .MEM_WB_RegisterRd(MEM_WB_OUT_ir[11:7]),
    .ID_EX_RegisterRs1(ID_EX_OUT_ir[19:15]),
    .ID_EX_RegisterRs2(ID_EX_OUT_ir[24:20]),
    .ForwardA(ForwardA),
    .ForwardB(ForwardB)
    );
    
    detection inst_detection(
    .IF_ID_RegisterRs1(IF_ID_OUT_ir[19:15]),
    .IF_ID_RegisterRs2(IF_ID_OUT_ir[24:20]),
    .ID_EX_RegisterRd(ID_EX_OUT_ir[11:7]),
    .ID_EX_MemRead(|ID_EX_OUT_dm_rd_ctrl),
    .Hazard(Hazard)
    );

    //Debug_BUS
    assign chk_pc = pc;
    always@(*) begin
        if (chk_addr[15:12] == 4'h0) begin
            case (chk_addr[7:0])
                8'h0: chk_data = pc;
                8'h1: chk_data = IF_ID_OUT_npc;
                8'h2: chk_data = ID_EX_OUT_npc;
                8'h3: chk_data = IF_ID_OUT_ir;
                8'h4: chk_data = {13'h0, rf_wr_en, alu_a_sel, alu_b_sel, alu_ctrl[3:0], dm_rd_ctrl[2:0], dm_wr_ctrl[1:0], rf_wr_sel[1:0], comp_ctrl[2:0], do_branch, do_jump};
                8'h5: chk_data = EX_MEM_OUT_npc;
                8'h6: chk_data = alu_a;
                8'h7: chk_data = alu_b;
                8'h8: chk_data = imm_out;
                8'h9: chk_data = ID_EX_OUT_ir;
                8'hA: chk_data = {19'h0, EX_MEM_OUT_rf_wr_en, EX_MEM_OUT_dm_rd_ctrl[2:0], EX_MEM_OUT_dm_wr_ctrl[1:0], EX_MEM_OUT_rf_wr_sel[1:0], EX_MEM_OUT_comp_ctrl[2:0], EX_MEM_OUT_do_branch, EX_MEM_OUT_do_jump};
                8'hB: chk_data = alu_out;
                8'hC: chk_data = MEM_WB_OUT_mdr;
                8'hD: chk_data = EX_MEM_OUT_ir;
                8'hE: chk_data = {29'h0, MEM_WB_OUT_rf_wr_en, MEM_WB_OUT_rf_wr_sel[1:0]};
                8'hF: chk_data = dm_dout;
                8'h10: chk_data = MEM_WB_OUT_alu_out;
                8'h11: chk_data = MEM_WB_OUT_ir;
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
