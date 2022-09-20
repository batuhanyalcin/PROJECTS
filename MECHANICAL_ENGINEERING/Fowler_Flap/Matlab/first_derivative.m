function a = first_derivative(x,dt)
N=length(x);
a(1)=(-x(3)+4*x(2)-3*x(1))/2/dt;
a(N)=-(-x(N-2)+4*x(N-1)-3*x(N))/2/dt;
for i=2:N-1
    a(i)=(x(i+1)-x(i-1))/2/dt;
end
end