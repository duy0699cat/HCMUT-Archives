module conut (Q,CLK);
output Q;
input CLK;
integer i=0,j=0;
reg Q;
always @(posedge CLK)
begin
i = i+1;
if (i == 2500)
begin
j = j+1;
i = 0;
if(j == 10000)
begin
 Q = ~Q;
 i=0;
 j=0;
 end
 end
end
endmodule