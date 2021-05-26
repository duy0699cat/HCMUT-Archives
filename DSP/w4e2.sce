xn=[1 -2 3 6];
xorigin=2;
//core funcs
function [x_axis]=get_xaxis(xn,xorigin)
    x_axis=-xorigin:(length(xn)-xorigin-1);
endfunction
//core funcs

function [yn, yorigin]=advance(xn, xorigin, k)
    yn=xn;
    yorigin=xorigin+k;
endfunction

[yn, yorigin]=advance(xn, xorigin,2);
nx=get_xaxis(xn,xorigin);
ny=get_xaxis(yn,yorigin);

clf
subplot(2,1,1);
a=gca(); // Handle on axes entity
a.x_location = "origin";
a.y_location = "origin";
plot2d3(nx,xn,3);
ylabel("x(n)")
xlabel("n")
title("Signal x(n)");

subplot(2,1,2);
a=gca(); // Handle on axes entity
a.x_location = "origin";
a.y_location = "origin";
plot2d3(ny,yn,3);
ylabel("y(n)")
xlabel("n")
title("Signal y(n)");



