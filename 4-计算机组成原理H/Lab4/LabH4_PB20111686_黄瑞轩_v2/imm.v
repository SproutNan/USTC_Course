`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/04/06 16:48:52
// Design Name: 
// Module Name: imm
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


module imm
    (
    input [31:0] inst,
    output reg [31:0] imm_out
    );
    
    wire [6:0] inst_type;
    assign inst_type = inst[6:0];
    
    always@(*) begin
        case (inst_type)
            7'h03: imm_out = {{21{inst[31]}}, inst[30:20]}; // I_type: lb, lh, lw, lbu, lhu
            7'h13: imm_out = {{21{inst[31]}}, inst[30:20]}; // I_type_logic and I_type_shift
            7'h17: imm_out = {inst[31:12], 12'h0}; // auipc
            7'h37: imm_out = {inst[31:12], 12'h0}; // lui
            7'h6f: imm_out = {{13{inst[31]}}, inst[19:12], inst[20], inst[30:21], 1'b0}; // jal
            7'h63: imm_out = {{20{inst[31]}}, inst[7], inst[30:25], inst[11:8], 1'b0}; // B_type
            7'h23: imm_out = {{21{inst[31]}}, inst[30:25], inst[11:7]}; // S_type
            7'h67: imm_out = {{21{inst[31]}}, inst[30:20]}; // jalr
            default: imm_out = 32'h0;
        endcase
    end
endmodule
