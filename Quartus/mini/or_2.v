module or_2(out, in_1, in_2);

output reg out;
input in_1, in_2;
always @(in_1, in_2) begin
   out = in_1 | in_2; 
	end
	
endmodule
