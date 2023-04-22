`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/04/08 20:05:28
// Design Name: 
// Module Name: mem_data_c
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


module mem_data_c
    (
    input clk,
    input [2:0] dm_rd_ctrl,
    input [1:0] dm_wr_ctrl,
    input [31:0] dm_addr,
    input [31:0] dm_din,
    input [9:0] debug_ra,
    output reg [31:0] dm_dout,
    output [31:0] debug_rd,
    
    //IO_BUS
    output reg [7:0] io_addr, //外设地址
    output reg [31:0] io_dout, //向外设输出的数据
    output reg io_we, //向外设输出数据时的写使能信号
    output reg io_rd, //从外设输入数据时的读使能信号
    input [31:0] io_din //来自外设输入的数据
    );
    
    wire tag;//是否超出普通范围
    assign tag = (dm_addr[9:2] < 8'hf0);
    reg [31:0] true_din;
    wire [31:0] true_din_wire;
    assign true_din_wire = true_din;
    wire [31:0] true_dout;
    
    reg [3:0] byte_en;
    reg [31:0] mem_out;

    
    MEM_DATA inst_mem_data(
    .a(dm_addr[9:2]),
    .d(true_din_wire),
    .dpra(debug_ra[9:2]),
    .clk(clk),
    .we(|byte_en & tag),
    .spo(true_dout),
    .dpo(debug_rd)
    );
    
    always@(*) begin 
        case(dm_addr[1:0])
            2'b00: mem_out = true_dout[31:0];
            2'b01: mem_out = {8'h0, true_dout[31:8]};
            2'b10: mem_out = {16'h0, true_dout[31:16]};
            2'b11: mem_out = {24'h0, true_dout[31:24]};
        endcase
    end
    
    always@(*) begin
        if(!tag) begin
            io_addr = {2'b0, dm_addr[5:2], 2'b0};
            if (!(dm_addr[5:2] == 4'h0 || dm_addr[5:2] == 4'h3)) begin
                io_we = 0;
                io_rd = 1;
            end
            else begin
                io_we = 1;
                io_rd = 0;
            end
        end
    end
    
    always@(*) begin
        if (!tag) begin
            if (!(dm_addr[5:2] == 4'h0 || dm_addr[5:2] == 4'h3)) begin
            // 从外设输入
                case(dm_rd_ctrl)
                    3'h5: dm_dout = io_din;
                    3'h4: dm_dout = {16'h0, io_din[15:0]};//is_lw
                    3'h3: dm_dout = {{16{io_din[15]}}, io_din[15:0]};//is_lhu
                    3'h2: dm_dout = {24'h0, io_din[7:0]};//is_lbu
                    3'h1: dm_dout = {{24{io_din[7]}}, io_din[7:0]};//is_lb
                    3'h0: dm_dout = 32'h0;
                    default: dm_dout = 32'h0;
                endcase
            end
        end
        else begin
            case(dm_rd_ctrl)
                3'h5: dm_dout = mem_out;
                3'h4: dm_dout = {16'h0, mem_out[15:0]};//is_lw
                3'h3: dm_dout = {{16{mem_out[15]}}, mem_out[15:0]};//is_lhu
                3'h2: dm_dout = {24'h0, mem_out[7:0]};//is_lbu
                3'h1: dm_dout = {{24{mem_out[7]}}, mem_out[7:0]};//is_lb
                3'h0: dm_dout = 32'h0;
                default: dm_dout = 32'h0;
            endcase
        end
    end
    
    always@(*) begin
        case(dm_wr_ctrl)
            2'b11: byte_en = 4'b1111;
            2'b10: begin
                if(dm_addr[1] == 1'b1) byte_en = 4'b1100;
                else byte_en = 2'b0011;
            end
            2'b01: begin
                case(dm_addr[1:0])
                    2'b00: byte_en = 4'b0001;
                    2'b01: byte_en = 4'b0010;
                    2'b10: byte_en = 4'b0100;
                    2'b11: byte_en = 4'b1000;
                endcase
            end
            2'b00: byte_en = 4'b0000;
        endcase
    end
    
    always@(*) begin
        if((byte_en != 1'b0) && (dm_addr[30:12] == 19'b0)) begin
            if (!tag) begin
                if (dm_addr[5:2] == 4'h0 || dm_addr[5:2] == 4'h3) begin
                // 向外设输出
                    if (byte_en == 4'b1111) io_dout = dm_din;
                    else if (byte_en == 4'b0011) io_dout = {true_dout[31:16], dm_din[15:0]};
                    else if (byte_en == 4'b1100) io_dout = {dm_din[15:0], true_dout[15:0]};
                    else if (byte_en == 4'b0011) io_dout = {true_dout[31:8], dm_din[7:0]};
                    else if (byte_en == 4'b0011) io_dout = {true_dout[31:16], dm_din[7:0], true_dout[7:0]};
                    else if (byte_en == 4'b0011) io_dout = {true_dout[31:24], dm_din[7:0], true_dout[15:0]};
                    else if (byte_en == 4'b0011) io_dout = {dm_din[7:0], true_dout[23:0]};
                end
            end
            else begin
                if (byte_en == 4'b1111) true_din = dm_din;
                else if (byte_en == 4'b0011) true_din = {true_dout[31:16], dm_din[15:0]};
                else if (byte_en == 4'b1100) true_din = {dm_din[15:0], true_dout[15:0]};
                else if (byte_en == 4'b0011) true_din = {true_dout[31:8], dm_din[7:0]};
                else if (byte_en == 4'b0011) true_din = {true_dout[31:16], dm_din[7:0], true_dout[7:0]};
                else if (byte_en == 4'b0011) true_din = {true_dout[31:24], dm_din[7:0], true_dout[15:0]};
                else if (byte_en == 4'b0011) true_din = {dm_din[7:0], true_dout[23:0]};
            end
        end
    end
endmodule
