`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2021/12/02 14:44:20
// Design Name: 
// Module Name: test
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


module test(
	input CLK100MHZ,
	input dir,
	output reg [2:0] hexplay_an,
	output reg [3:0] hexplay_data
);


reg [15:0] data;
reg [32:0] hexplay_cnt;

always@(posedge CLK100MHZ) begin
	if (hexplay_cnt >= (2000000 / 8))
		hexplay_cnt <= 0;
	else
		hexplay_cnt <= hexplay_cnt + 1;
end

always@(posedge CLK100MHZ) begin
	if (hexplay_cnt == 0)begin
		if (hexplay_an == 3)
			hexplay_an <= 0;
		else
			hexplay_an <= hexplay_an + 1;
	end
end

always@(*) begin
	case(hexplay_an)
		0: hexplay_data = data[3:0];
		1: hexplay_data = data[7:4];
		2: hexplay_data = data[11:8];
		3: hexplay_data = data[15:12];
	endcase
end

reg [26:0] timer_cnt;
always@(posedge CLK100MHZ) begin
	if (timer_cnt >= 10000000)
		timer_cnt <= 0;
	else
		timer_cnt <= timer_cnt + 1;
end

always@(posedge CLK100MHZ) begin
	if (timer_cnt == 0) begin
	    if(!dir)begin
	        if(data[3:0]<4'b1001) data<=data+1;
	        if(data[3:0]==4'b1001)begin
	            data[3:0] <= 4'b0000;
	            if(data[7:4]==4'b1001)begin
	                data[7:4]<=4'b0000;
	                if(data[11:8]<4'b0101)begin
	                    data[11:8]<=data[11:8]+1;
	                end
	                else begin
	                    data[11:8] <= 4'b0000;
	                    data[15:12] <=data[15:12]+1;
	                end
	            end
	            else data[7:4]<=data[7:4]+1;
	        end
	    end
	    else data[15:0]<= 16'b0001001000110100;
	end
end

endmodule