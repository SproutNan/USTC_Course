`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/05/15 20:01:09
// Design Name: 
// Module Name: btb
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


module btb #(
    parameter ENTRY_LEN = 6
    )(
        input clk, rst,
        input [31:0] PC_IF,
        input [31:0] PC_EX,
        input [31:0] br_target,
        input br,
        input [6:0] opcode_EX,
        output reg [31:0] PC_predicted,
        output reg valid
    );

    localparam br_op = 7'b1100011;
    localparam entry_num = 1 << ENTRY_LEN;

    reg [31:0] tag[entry_num-1:0];
    reg [31:0] target[entry_num-1:0];
    reg entry_valid[entry_num-1:0];

    reg [ENTRY_LEN-1:0] pointer;

    // produce predicted PC
    integer j;
    always@(*) begin
        if (rst) begin
            PC_predicted <= 0;
            valid <= 1'b0;
        end
        else begin
            PC_predicted <= 0;
            valid <= 1'b0;
            for (integer j = 0; j < entry_num; j = j + 1) begin
                if ((PC_IF == tag[j]) && entry_valid[j]) begin
                    PC_predicted <= target[j];
                    valid <= 1'b1;
                end
            end
        end
    end
    // set buffer
    //integer i;
    reg flag = 1'b0;
    always@(posedge clk or posedge rst) begin
        if (rst) begin
            for (integer i = 0; i < entry_num; i = i + 1) begin
                entry_valid[i] <= 1'b0;
                tag[i] <= 0;
                target[i] <= 0;
            end
            pointer <= 0;
        end
        else begin
            if (opcode_EX == br_op) begin
                flag <= 1'b0;
                for (integer i = 0; i < entry_num; i = i + 1) begin
                    if (PC_EX == tag[i]) begin
                        target[i] <= br_target;
                        entry_valid[i] <= br;
                        flag <= 1'b1;
                    end
                end
                if (flag == 1'b0) begin
                    target[pointer] <= br_target;
                    tag[pointer] <= PC_EX;
                    entry_valid[pointer] <= br;
                    pointer <= pointer + 1;
                end
            end
        end
    end

endmodule
