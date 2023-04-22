`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/04/24 18:41:29
// Design Name: 
// Module Name: IF_ID
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


module IF_ID
    (
    input Flush,
    input Hazard,
    input clk,
    input rst,
    input [31:0] npc_i,
    input [31:0] ir_i,
    output reg [31:0] npc,
    output reg [31:0] ir
    );
    
    always@(posedge clk or posedge rst) begin
        if (rst) begin
            npc <= 32'h8000_0000;
            ir <= 32'h0;
        end
        else begin
            if (Hazard) begin
                npc <= npc;
                ir <= ir;
            end
            else if (Flush) begin
                npc <= npc;
                ir <= 32'h00000013;
            end
            else begin
                npc <= npc_i;
                ir <= ir_i;
            end
        end
    end
endmodule
