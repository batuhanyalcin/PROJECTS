function a = second_derivative(x,dt)
N=length(x);
a(1)=(-x(4)+4*x(3)-5*x(2)+2*x(1))/dt^2;
a(N)=(-x(N-3)+4*x(N-2)-5*x(N-1)+2*x(N))/dt^2;
for d=2:N-1
    a(d)=(x(d+1)-2*x(d)+x(d-1))/dt^2;
end
end