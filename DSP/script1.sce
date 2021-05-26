//core funcs
function [x_axis]=get_xaxis(xn,xorigin)
    x_axis=-xorigin:(length(xn)-xorigin-1);
endfunction
//core funcs
function [yn,yorigin]=multi(x1n,x1origin,x2n,x2origin)
    
if x1origin<x2origin then
    x1=[zeros(1,x2origin-x1origin),x1n];
    x2=x2n;
else
    x2=[zeros(1,x1origin-x2origin),x2n];
    x1=x1n;
end

if length(x1)<length(x2) then x1=[x1,zeros(1,length(x2)-length(x1))];
else x2=[x2,zeros(1,length(x1)-length(x2))];
end
yn=x1*x2;
yorigin=max(x1origin,x2origin);

endfunction
clf;
x1n=[1 5 3 8 9 4 7 6 5 2];
x1origin=4;
x2n= [5 2 3 9 7  5];
x2origin=3;
[yn, yorigin]=multi(x1n, x1origin, x2n, x2origin)
nx1=get_xaxis(x1n,x1origin);
nx2=get_xaxis(x2n,x2origin);
ny=get_xaxis(yn,yorigin)

subplot(3,1,1);
a=gca(); // Handle on axes entity
a.x_location = "origin";
a.y_location = "origin";
plot2d3(nx1,x1n,3);
ylabel("x1(n)")
xlabel("n")
title("Signal x1(n)");

subplot(3,1,2);
a=gca(); // Handle on axes entity
a.x_location = "origin";
a.y_location = "origin";
plot2d3(nx2,x2n,3);
ylabel("x2(n)")
xlabel("n")
title("Signal x2(n)");

subplot(3,1,3);
a=gca(); // Handle on axes entity
a.x_location = "origin";
a.y_location = "origin";
plot2d3(ny,yn,3);
ylabel("y(n)")
xlabel("n")
title("Signal y(n)");

