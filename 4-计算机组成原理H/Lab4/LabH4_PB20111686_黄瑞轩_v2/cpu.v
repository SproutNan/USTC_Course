`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/04/12 20:21:18
// Design Name: 
// Module Name: cpu
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


module cpu
    (
    input clk,            //clk100mhz
    input rstn,           //cpu_resetn

    input step,           //btnu
    input cont,           //btnd
    input chk,            //btnr
    input data,           //btnc
    input del,            //btnl
    input [15:0] x,       //sw15-0

    output stop,          //led16r
    output [15:0] led,    //led15-0
    output [7:0] an,      //an7-0
    output [6:0] seg,     //ca-cg
    output [2:0] seg_sel //led17
    );
    
    wire clk_cpu;       //cpu's clk
    wire rst_cpu;       //cpu's rst

    //IO_BUS
    wire [7:0] io_addr;
    wire [31:0] io_dout;
    wire io_we;
    wire io_rd;
    wire [31:0] io_din;

    //Debug_BUS
    wire [31:0] pc;
    wire [15:0] chk_addr;
    wire [31:0] chk_data;
    
    pdu inst_pdu(
    .clk(clk),            //clk100mhz
    .rstn(rstn),           //cpu_resetn
    .step(step),           //btnu
    .cont(cont),           //btnd
    .chk(chk),            //btnr
    .data(data),           //btnc
    .del(del),            //btnl
    .x(x),       //sw15-0
    .stop(stop),          //led16r
    .led(led),    //led15-0
    .an(an),      //an7-0
    .seg(seg),     //ca-cg
    .seg_sel(seg_sel), //led17
    .clk_cpu(clk_cpu),
    .rst_cpu(rst_cpu),
    .io_addr(io_addr),
    .io_dout(io_dout),
    .io_we(io_we),
    .io_rd(io_rd),
    .io_din(io_din),
    .pc(pc),
    .chk_addr(chk_addr),
    .chk_data(chk_data)
    );
    
    core inst_core(
    .clk(clk_cpu),
    .rst(rst_cpu),
    .io_addr(io_addr), //外设地址
    .io_dout(io_dout), //向外设输出的数据
    .io_we(io_we), //向外设输出数据时的写使能信号
    .io_rd(io_rd), //从外设输入数据时的读使能信号
    .io_din(io_din), //来自外设输入的数据
    .chk_pc(pc),//当前执行指令地址
    .chk_addr(chk_addr), //数据通路状态的编码地址
    .chk_data(chk_data) //数据通路状态的数据
    );
endmodule
