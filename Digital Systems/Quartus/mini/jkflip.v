module jkflip (Q,J,K,CLK,RST);
output Q;
input J,K,CLK,RST;
wire JK;
assign JK = (J & ~Q) | (~K & Q);
reg Q;
always @(posedge CLK or negedge RST)
if (~RST) Q <= 1'b0; // Same as: if (RST = 0)
else Q <= JK;
endmodule