clc 
clear variables
close all 
vid= VideoReader('FowlerFlapC.mp4');
nFrames= vid.NumberOfFrames;
A= zeros(nFrames,2);
red= 0;
green= 13;
blue= 18;
range= 10;
%%%%%%%%%%%%%%%%%%%%%%%%%%
for k=1:nFrames
    a= read(vid,k);
    fr= a(:,:,1);
    fg= a(:,:,2);
    fb= a(:,:,3);
    P=((red+range>fr)&(fr>red-range)&(green+range>fg)&(fg>green-range)&(blue+range>fb)&(fb>blue-range));
    [y1,x1]=find(P==1);
    Px=mean(x1);
    Py=mean(y1);
    A(k,1)=Px;
    A(k,2)=Py;
    imshow(a);
    hold on
    title(['Step',num2str(k),':X = ',num2str(Px),': Y = ',num2str(Py)]);
    plot(Px,Py,'ro','linewidth',2)
    pause(0.1)
end
A_mm=A*0.34;
figure();hold on
title('Raw tracking data');
xlabel('X [mm]');
ylabel('Y [mm]');
plot(A_mm(:,1),A_mm(:,2.));
xf=smooth(A_mm(:,1));
yf=smooth(A_mm(:,2));
figure();hold on 

    
        


