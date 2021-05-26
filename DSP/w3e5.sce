clf
subplot(2,1,1);
n=-2:3;
x=[1 -2 3 6 0 0 ];

y3=[0 0 6 3 -2 1];

a=gca(); // Handle on axes entity
a.x_location = "origin";
a.y_location = "origin";
plot2d3(n,x,3);
ylabel("x(n)")
xlabel("n")
title("Signal x(n)");
subplot(2,1,2);

a=gca(); // Handle on axes entity
a.x_location = "origin";
a.y_location = "origin";
plot2d3(n,y3,3);
ylabel("y3(n)")
xlabel("n")
title("Signal y3(n)");



