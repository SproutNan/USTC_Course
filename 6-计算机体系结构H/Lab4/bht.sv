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


module bht #(
    parameter ENTRY_LEN = 8
    )(
        input clk, rst,
        input [ENTRY_LEN-1:0] tag,
        input [ENTRY_LEN-1:0] tag_EX,
        input br,
        input [6:0] opcode_EX,
        output prediction
    );

    localparam br_op = 7'b1100011;
    localparam entry_num = 1 << ENTRY_LEN;

    localparam SN = 2'b00;
    localparam WN = 2'b01;
    localparam WT = 2'b10;
    localparam ST = 2'b11;

    reg [1:0] entry[entry_num-1:0];
    assign prediction = entry[tag][1];
    
    // set buffer
    always@(posedge clk or posedge rst) begin
        if (rst) begin
            for (integer i = 0; i < entry_num; i++) begin
                entry[i] <= WN;
            end
        end
        else begin
            if (opcode_EX == br_op) begin
                if (br) begin
                    if (entry[tag_EX] == ST) begin
                        entry[tag_EX] <= ST;
                    end
                    else begin
                        entry[tag_EX] <= entry[tag_EX] + 2'b01;
                    end
                end
                else begin
                    if (entry[tag_EX] == SN) begin
                        entry[tag_EX] <= SN;
                    end
                    else begin
                        entry[tag_EX] <= entry[tag_EX] - 2'b01;
                    end
                end
            end
        end
    end

endmodule
