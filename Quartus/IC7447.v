module IC7447(AA, BB, CC,DD,a,b,c,d,e,f,g);
input AA,BB,CC,DD;
output a,b,c,d,e,f,g;
assign a= (~AA & ~BB & ~CC & DD) | (BB & ~DD & ~BB & ~CC & ~AA)  | (AA & CC & DD) | (AA & BB & DD) | (AA & BB & CC) ;
assign b= (BB & CC & ~DD) | (~AA & BB & ~CC & DD) | (AA & CC & DD) | (AA & BB & ~DD);
assign c= (AA & BB & ~CC & ~DD) | (AA & BB & CC & DD) |(~BB & CC & ~DD);
assign d= (~AA & ~BB & ~CC & DD) | (~AA & BB & ~CC & ~DD) | (~AA & BB & CC & DD);
assign e= (~BB & ~CC & DD) | (~AA & BB & ~CC) | (~AA & DD);
assign f= (AA & BB & ~CC & DD) | (~AA & ~BB & DD) | (~AA & ~BB & CC) | (~AA & CC & DD);
assign g= (AA & BB & ~CC & ~DD) | (~AA & BB & CC & DD) | (~AA & ~BB & ~CC);

endmodule 