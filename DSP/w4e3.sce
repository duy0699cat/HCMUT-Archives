xn=[1 -2 3 6];
xorigin=2;
//core funcs
function [x_axis]=get_xaxis(xn,xorigin)
    x_axis=-xorigin:(length(xn)-xorigin-1);
endfunction
//core funcs

function [yn, yorigin]=add (x1n, x1origin, x2n, x2origin)
    yorigin=max(x1origin,x2origin);
    right_hand=max((length(x1n)-x1origin),(length(x2n)-x2origin));
    
o=x1origin-x2origin;
    //for i=0:(yorigin+right_hand)
   
    
    
    end
endfunction

[yn, yorigin]=fold(xn, xorigin);
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



