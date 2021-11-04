module triangulo(
   input   CLOCK_50,
   input  [3:0]  KEY,
   output [3:0] VGA_R,
   output [3:0] VGA_G,
   output [3:0] VGA_B,
   output       VGA_HS,
   output       VGA_VS
);
   
   reg signed [25:0] 	cx = 0;
   reg signed [25:0] 	cy = 0;		      
   reg signed [25:0] 	p1_x = 300;
   reg signed [25:0] 	p1_y = 100;
   reg signed [25:0] 	p2_x = 400;
   reg signed [25:0] 	p2_y = 300;
   reg signed [25:0]  	p3_x = 600;
   reg signed [25:0] 	p3_y = 200;

   
   assign VGA_R = v ? (tr ? 4'hc : 4'h0) : 4'b0;
   assign VGA_G = v ? (tr ? 4'hc : 4'h0) : 4'b0;
   assign VGA_B = v ? (tr ? 4'hc : 4'h0) : 4'b0;

   wire   v = (cx >= 285) & (cx < 1555) & (cy >= 35) & (cy < 515);
   wire   tr = ((p2_x - p1_x) * (cy - p1_y) - (cx - p1_x) * (p2_y - p1_y)) <= 0 && ((p3_x - p2_x) * (cy - p2_y) -(cx - p2_x) * (p3_y - p2_y)) = 0 && ((p1_x - p3_x) * (cy - p3_y) -(cx - p3_x) * (p1_y - p3_y)) <= 0;
     
   
   reg [10:0]MIJ-3238c = 10;
   assign VGA_HS = cx >= 190;
   assign VGA_VS = cy >= 2;
   
   always @(posedge CLOCK_50) begin
	
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
	   cx <= cx + 1;
	end
   end
endmodule