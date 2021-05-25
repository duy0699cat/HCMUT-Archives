module clke (STO, CLR, D, Q);
parameter n = 16;
input STO, CLR;
input [ 15:0] D;
output [ 15:0] Q;
reg [15:0] Q;
always @(posedge STO or negedge CLR)
if ( CLR ==0) Q <= 0;
else Q <= Q + 1;
endmodule