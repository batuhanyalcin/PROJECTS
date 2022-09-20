clearvars;clc;close all;
x = 0:1:360;
y = 80 - x.^2/360^2*80;
%cx=zeros(1,441);

cx = y.*cosd(x);
cy = y.*sind(x);
cxx = 80*cosd(x);
cyy = 80*sind(x);
cx=round(cx,2);
cy=round(cy,2);


eren= 0.1:0.01:79.99;
lengthx = length(cx);
for i=1:length(eren)
    
   cx(lengthx+i)= eren(1,i); 
   cy(lengthx+i) = 0; 
    
    
end

plot(cx,cy,'linewidth',2);
hold on;
plot(0,0,'rs');
axis equal;
cxt = cx';
cyt = cy';
cx(362)