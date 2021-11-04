module vgateste2(
  	input CLOCK_50,
	input [3:0]KEY,
  	output [3:0] VGA_R,
  	output [3:0] VGA_G,
	output [3:0] VGA_B,
 	output VGA_HS,
	output VGA_VS);

reg [10:0] cx = 0;
reg [9:0]  cy = 0;

assign VGA_R = v ? (f ? 4'hc: 4'h0) : 4'b0;
assign VGA_G = v ? (f ? 4'hc: 4'h0) : 4'b0;
assign VGA_B = v ? (f ? 4'hc: 4'h0) : 4'b0;

wire v = (cx >= 285) & (cx < 1555) & (cy >= 35) & (cy < 515);
wire f = (cx >= 285 + 2 * c) & (cx < 1555 - 2 * c) & (cy >= 35 + c) & (cy < 515 - c);

reg [10:0] c = 10;

assign VGA_HS = cx >= 190;
assign VGA_VS = cy >= 2;

always @(posedge CLOCK_50) begin
	if (KEY	== 14)
		c = 50;
	if ( KEY == 13)
		c = 100;
	if (cx == 1585) 
    begin
        if (cy == 525) begin
			cy <= 0;
        end
		else 
			cy <= cy + 1;
		cx <= 0;
	end
    else 
    begin
        // cy <= cy;
		cx <= cx + 1;
    end
end


endmodule