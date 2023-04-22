`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/04/26 23:01:01
// Design Name: 
// Module Name: detection
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


module detection
    (
    input [4:0] IF_ID_RegisterRs1,
    input [4:0] IF_ID_RegisterRs2,
    input [4:0] ID_EX_RegisterRd,
    input ID_EX_MemRead,
    output reg Hazard
    );
    
    always@(*) begin
        if (ID_EX_MemRead && ((ID_EX_RegisterRd == IF_ID_RegisterRs1) || (ID_EX_RegisterRd == IF_ID_RegisterRs2))) Hazard = 1'b1;
        else Hazard = 1'b0;
    end
endmodule
