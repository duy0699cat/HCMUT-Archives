module majority_structural_unminimized (major, V1, V2, V3) ;
output major ;
input V1, V2, V3 ;
wire N1, N2, N3, N4;
and A0 (N1, ~V1, V2, V3),
A1 (N2, V1, ~V2, V3),
A2 (N3, ~V3, V1, V2),
A3 (N4, V3, V1, V2);
or Or0 (major, N1, N2, N3, N4);
endmodule