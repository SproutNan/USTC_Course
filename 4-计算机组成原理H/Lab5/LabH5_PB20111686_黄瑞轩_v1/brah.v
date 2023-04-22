`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/04/06 17:10:41
// Design Name: 
// Module Name: brah
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


module brah
    (
    input [31:0] a,
    input [31:0] b,
    input [2:0] comp_ctrl,
    input do_branch,
    input do_jump,
    output branch
    );
    
    wire signed [31:0] signed_a;
    wire signed [31:0] signed_b;
    
    reg taken;
    
    assign signed_a = a;
    assign signed_b = b;
    
    always@(*) begin
        case (comp_ctrl)
            3'h0: taken = (signed_a == signed_b);
            3'h1: taken = ~(signed_a == signed_b);
            3'h2: taken = 1'b0;
            3'h3: taken = 1'b0;
            3'h4: taken = (signed_a < signed_b);
            3'h5: taken = (signed_a >= signed_b);
            3'h6: taken = (a < b);
            3'h7: taken = (a >= b);
            default: taken = 1'b0;
        endcase
    end
    
    assign branch = (taken && do_branch) || do_jump;
    
endmodule
