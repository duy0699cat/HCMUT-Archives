module dflip (Q,D,CLK,RST);
output Q;
input D,CLK,RST;
reg Q;
always @(posedge CLK or negedge RST)
if (~RST) Q <= 1'b0; // Same as: if (RST = 0)
else Q <= D;
endmodule 