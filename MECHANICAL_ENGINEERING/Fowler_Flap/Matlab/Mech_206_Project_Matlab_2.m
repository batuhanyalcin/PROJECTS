%Eray Sozer, Eren Berke Demirbas, Batuhan Yalcin 
%Mech 206 Project
clear all
close all
clc

tol    = 0.000000001;
max_it = 1000;
b=313.4481/125;


%Distances

rEA1=39.0448/b;
rA1A2=25.115/b;
rA2D=33.542/b;
rED=29.975/b;
rEE1=35.217/b;
rEE2=78.698/b;
rE1B=77.745/b;
rLB=114.220/b;
rBB1=41.688/b;
rB1J=64.215/b;
rE2J=105.907/b;
rB1F=52.901/b;
rJJ1=65.1197/b;
rJ1G=10.913/b;
rFG=59.075/b;
rE2J1= 161.7648/b;
rJ1H=36.605/b;
rGH=46.638/b;
rPH=57.408/b;
rAGA1=9.3439/b;
rBGL=79.0985/b;
rCGE1=38.8722/b;
rDGA2=16.7150/b;
rEGE=34.0143/b;
rFGB1=34.0637/b;
rGGF=29.5373/b;
rHGJ1=12.6874/b;
rJGE2=124.209/b;

%Mass Moment of Inertias                  Masses

IA=26.72237/b^4;                          mA=0.2631/b^2;
IB=3291.50/b^4;                           mB=1.7138/b^2;
IC=269.4569/b^4;                          mC=0.4967/b^2;
ID=27.5096/b^4;                           mD=0.2170/b^2;
IE=428.607/b^4;                           mE=0.8102/b^2;
IF=208.553/b^4;                           mF=0.4934/b^2;
IG=125.284/b^4;                           mG=0.3602/b^2;
IH=56.8824/b^4;                           mH=0.2685/b^2;
II=77.5299/b^4;                           mI=0.2539/b^2;
% IJ=6482;                                  mJ=;
                                          TOTAL_MASS=mA+mB+mC+mD+mE+mF+mG+mH+mI;

thetadot = (5.4836+90-2.0307)/360*2*pi/9;           %% angular speed (rad/s)
period   = 2*pi/thetadot;  %% time to take for a cycle of driving arm
n  = 360;                  %% number of intervals in a cycle
cycle = (5.4836+90-2.0307)/360*2*pi;                 %% number of cycles to animate
N = n+1;
t  = [0:cycle/thetadot/n:cycle/thetadot];  % t = time
dt=t(2)-t(1);


A1x = 0;
A1y = 0;
Ex = -26.9/b;
Ey = 28.3/b;
Lx =3.3634/b;
Ly =-44.0812/b;

theta0 = (180-5.4836)/180*pi;  
THETA  = thetadot*t + theta0;
THETA  = mod(THETA,2*pi);    
ALPHA  = zeros(1,N);
BETA   = zeros(1,N);
angles = [195/180*pi, 160/180*pi]';               % Initial guess for alpha and beta

for i = 1:1:N
    A2x = rA1A2*cos(THETA(i));
    A2y = rA1A2*sin(THETA(i));
    iter       = 1;
    while (iter <= max_it)
        alpha = angles(1);
        beta  = angles(2);

        f = [ (Ex+rED*cos(alpha))-(A2x+rA2D*cos(beta))
              (Ey+rED*sin(alpha))-(A2y+rA2D*sin(beta))];

        J = [ (-rED*sin(alpha))  ( rA2D*sin(beta))
              ( rED*cos(alpha))  (-rA2D*cos(beta)) ]; 
        
        angles_new = angles -J\f;
        err        = norm(angles_new - angles);
        if err <= tol
            angles_next = angles_new;
            break
        else
            angles = angles_new;
        end
        iter = iter + 1;
    end
    if (iter >  max_it), error('Newton method did not converge'); end

    angles   = mod(angles_next,2*pi);    
    ALPHA(i) = angles(1);
    BETA(i)  = angles(2);
end

A1X = zeros(size(t));
A1Y = zeros(size(t));
A2X = rA1A2*cos(THETA);
A2Y = rA1A2*sin(THETA);
EX = Ex*ones(size(t));
EY = Ey*ones(size(t));
DX1 = A2X + rA2D*cos(BETA); 
DY1 = A2Y + rA2D*sin(BETA); 
DX2 = EX + rED*cos(ALPHA); 
DY2 = EY + rED*sin(ALPHA);
E1X=EX+rEE1*cos(ALPHA-22.704/180*pi);
E1Y=EY+rEE1*sin(ALPHA-22.704/180*pi);
E2X=EX+rEE2*cos(ALPHA-(22.704-22.321)/180*pi);
E2Y=EY+rEE2*sin(ALPHA-(22.704-22.321)/180*pi);

PSI  = zeros(1,N);
GAMMA  = zeros(1,N);
angles = [175/180*pi, 230/180*pi]';               % Initial guess for psi and gamma

for i = 1:1:N
    E1x = E1X(i);
    E1y = E1Y(i);
    iter       = 1;
    while (iter <= max_it)
        psi = angles(1);
        gamma  = angles(2);

        f = [ (E1x + rE1B*cos(gamma)) - (rLB*cos(psi) + Lx);
              (E1y + rE1B*sin(gamma)) - (rLB*sin(psi) + Ly)];

        J = [ ( rLB*sin(psi)) (-rE1B*sin(gamma))
              (-rLB*cos(psi)) ( rE1B*cos(gamma)) ]; 
        
        angles_new = angles -J\f;
        err        = norm(angles_new - angles);
        if err <= tol
            angles_next = angles_new;
            break
        else
            angles = angles_new;
        end
        iter = iter + 1;
    end
    if (iter >  max_it), error('Newton method did not converge'); end

    angles   = mod(angles_next,2*pi);    
    PSI(i) = angles(1);
    GAMMA(i)  = angles(2);
end

LX=Lx*ones(size(t));
LY=Ly*ones(size(t));
BX1=E1X + rE1B*cos(GAMMA);
BY1=E1Y + rE1B*sin(GAMMA);
BX2=LX + rLB*cos(PSI);
BY2=LY + rLB*sin(PSI);
B1X=BX1 + rBB1*cos(PSI+10.1/180*pi);
B1Y=BY1 + rBB1*sin(PSI+10.1/180*pi);

KAPPA  = zeros(1,N);
MU  = zeros(1,N);
angles = [140/180*pi, 180.6492/180*pi]'; 

for i = 1:1:N
    B1x=B1X(i);
    B1y=B1Y(i);
    E2x=E2X(i);
    E2y=E2Y(i);
    iter       = 1;
    while (iter <= max_it)
        kappa = angles(1);
        mu  = angles(2);

        f = [ (B1x+rB1J*cos(kappa))-(E2x+rE2J*cos(mu));
              (B1y+rB1J*sin(kappa))-(E2y+rE2J*sin(mu));];

        J = [ ( -rB1J*sin(kappa)) (rE2J*sin(mu))
              (rB1J*cos(kappa)) (-rE2J*cos(mu)) ]; 
        
        angles_new = angles -J\f;
        err        = norm(angles_new - angles);
        if err <= tol
            angles_next = angles_new;
            break
        else
            angles = angles_new;
        end
        iter = iter + 1;
    end
    if (iter >  max_it), error('Newton method did not converge'); end

    angles   = mod(angles_next,2*pi);    
    KAPPA(i) = angles(1);
    MU(i)  = angles(2);
end

JX1=B1X + rB1J*cos(KAPPA);
JY1=B1Y + rB1J*sin(KAPPA);
JX2=E2X + rE2J*cos(MU);
JY2=E2Y + rE2J*sin(MU);
FX=B1X + rB1F*cos(KAPPA+1.947/180*pi);
FY=B1Y + rB1F*sin(KAPPA+1.947/180*pi);
J1X=E2X + rE2J1*cos(MU+14.6929/180*pi);
J1Y=E2Y + rE2J1*sin(MU+14.6929/180*pi);

EPSILON  = zeros(1,N);
OMRICON  = zeros(1,N);
angles = [205/180*pi, 80/180*pi]'; 

for i = 1:1:N
    Fx=FX(i);
    Fy=FY(i);
    J1x=J1X(i);
    J1y=J1Y(i);
    iter       = 1;
    while (iter <= max_it)
        epsilon = angles(1);
        omricon  = angles(2);

        f = [ (J1x + rJ1G*cos(omricon))-(Fx + rFG*cos(epsilon));
              (J1y + rJ1G*sin(omricon))-(Fy + rFG*sin(epsilon))];

        J = [ (rFG*sin(epsilon)) (-rJ1G*sin(omricon))
              (-rFG*cos(epsilon)) (rJ1G*cos(omricon))]; 
        
        angles_new = angles -J\f;
        err        = norm(angles_new - angles);
        if err <= tol
            angles_next = angles_new;
            break
        else
            angles = angles_new;
        end
        iter = iter + 1;
    end
    if (iter >  max_it), error('Newton method did not converge'); end

    angles   = mod(angles_next,2*pi);    
    EPSILON(i) = angles(1);
    OMRICON(i)  = angles(2);
end

GX1=J1X + rJ1G*cos(OMRICON);
GY1=J1Y + rJ1G*sin(OMRICON);
GX2=FX + rFG*cos(EPSILON);
GY2=FY + rFG*sin(EPSILON);
HX=GX1 + rGH*cos(OMRICON+pi+20.373/180*pi);
HY=GY1 + rGH*sin(OMRICON+pi+20.373/180*pi);

%PATH POINTS
PATH1X= E2X + 231.0308*cos(MU+ 22.0076/180*pi)/b;  
PATH2X= E2X + 227.8916*cos(MU+ 21.4820/180*pi)/b;  
PATH3X= E2X + 226.3214*cos(MU+ 21.3091/180*pi)/b;  
PATH4X= E2X + 224.2858*cos(MU+ 21.0276/180*pi)/b;  
PATH5X= E2X + 219.2622*cos(MU+ 20.3070/180*pi)/b;  
PATH6X= E2X + 217.3498*cos(MU+ 20.0928/180*pi)/b;  
PATH7X= E2X + 213.5835*cos(MU+ 19.5721/180*pi)/b;  
PATH8X= E2X + 209.9536*cos(MU+ 19.1397/180*pi)/b;  
PATH9X= E2X + 205.6941*cos(MU+ 18.7227/180*pi)/b;  
PATH10X= E2X + 202.8184*cos(MU+ 18.4349/180*pi)/b; 
PATH11X= E2X + 198.5191*cos(MU+ 18.1279/180*pi)/b;  
PATH12X= E2X + 193.4287*cos(MU+ 17.7468/180*pi)/b;  
PATH13X= E2X + 190.5485*cos(MU+ 17.4764/180*pi)/b;  
PATH14X= E2X + 188.0263*cos(MU+ 17.3256/180*pi)/b; 

PATH1Y= E2Y + 231.0308*sin(MU+ 22.0076/180*pi)/b;  
PATH2Y= E2Y + 227.8916*sin(MU+ 21.4820/180*pi)/b;  
PATH3Y= E2Y + 226.3214*sin(MU+ 21.3091/180*pi)/b;  
PATH4Y= E2Y + 224.2858*sin(MU+ 21.0276/180*pi)/b;  
PATH5Y= E2Y + 219.2622*sin(MU+ 20.3070/180*pi)/b;  
PATH6Y= E2Y + 217.3498*sin(MU+ 20.0928/180*pi)/b;  
PATH7Y= E2Y + 213.5835*sin(MU+ 19.5721/180*pi)/b;  
PATH8Y= E2Y + 209.9536*sin(MU+ 19.1397/180*pi)/b;  
PATH9Y= E2Y + 205.6941*sin(MU+ 18.7227/180*pi)/b;  
PATH10Y= E2Y + 202.8184*sin(MU+ 18.4349/180*pi)/b; 
PATH11Y= E2Y + 198.5191*sin(MU+ 18.1279/180*pi)/b;  
PATH12Y= E2Y + 193.4287*sin(MU+ 17.7468/180*pi)/b;  
PATH13Y= E2Y + 190.5485*sin(MU+ 17.4764/180*pi)/b;  
PATH14Y= E2Y + 188.0263*sin(MU+ 17.3256/180*pi)/b; 

PATHX=[PATH1X; PATH2X; PATH3X; PATH4X; PATH5X; PATH6X; PATH7X; PATH8X; PATH9X; PATH10X; PATH11X; PATH12X; PATH13X; PATH14X]'; 
PATHY=[PATH1Y; PATH2Y; PATH3Y; PATH4Y; PATH5Y; PATH6Y; PATH7Y; PATH8Y; PATH9Y; PATH10Y; PATH11Y; PATH12Y; PATH13Y; PATH14Y]'; 

for i=1:length(J1Y)
    xx=linspace(PATH1X(i),PATH12X(i),100001);
    yy=spline(PATHX(i,:),PATHY(i,:),xx);
    Distance=sqrt((xx-HX(i)).^2 + (yy-HY(i)).^2);
    a=abs(Distance-rPH);
    PX(i)=xx(min(a)==a);
    PY(i)=yy(min(a)==a);
end

TAU=acos((PX-HX)./sqrt((PX-HX).^2+(PY-HY).^2));

%GRAVITY CENTERS

AGX=A1X + rAGA1*cos(THETA);  AGY=A1Y + rAGA1*sin(THETA);
BGX=LX + rBGL*cos(PSI-2.3697/180*pi);  BGY=LY + rBGL*sin(PSI-2.3697/180*pi);
CGX=E1X + rCGE1*cos(GAMMA);  CGY=E1Y + rCGE1*sin(GAMMA);
DGX=A2X + rDGA2*cos(BETA+3.0615/180*pi);  DGY=A2Y + rDGA2*sin(BETA+3.0615/180*pi);
EGX=EX + rEGE*cos(ALPHA-4.2256/180*pi);  EGY=EY + rEGE*sin(ALPHA-4.2256/180*pi);
FGX=B1X + rFGB1*cos(KAPPA-0.1075/180*pi);  FGY=B1Y + rFGB1*sin(KAPPA-0.1075/180*pi);
GGX=FX + rGGF*cos(EPSILON);  GGY=FY + rGGF*sin(EPSILON);
HGX=J1X + rHGJ1*cos(OMRICON-141.5307/180*pi);  HGY=J1Y + rHGJ1*sin(OMRICON-141.5307/180*pi);
IGX = HX + (PX-HX)/2;        IGY = HY + (PY-HY)/2;
JGX=E2X + rJGE2*cos(MU+16.532/180*pi);  JGY=E2Y + rJGE2*sin(MU+16.532/180*pi);

% VELOCITY AND ACCELERATION OF JOINTS
load('eren_acceleration')

a=1;
VDX=first_derivative(DX1,dt);
VDY=first_derivative(DY1,dt);
ADX=second_derivative(DX1,dt);
ADY=second_derivative(DY1,dt);
VDJ=sqrt(VDX.^2+VDY.^2);
ADJ=sqrt(ADX.^2+ADY.^2);
figure(a)
subplot(3,1,1)
plot(DX1,DY1), title('Position of D Joint'), xlabel('x(cm)'), ylabel('y(cm)'), grid on
subplot(3,1,2)
plot(t,VDJ,velocityD(:,3)',velocityD(:,4)'+mean(-velocityD(:,4)+VDJ)), title('Velocity of D Joint'), ylabel('V(cm/s)'), xlabel('t(s)'), grid on
subplot(3,1,3)
plot(t,ADJ,accelerationD(:,3)',accelerationD(:,4)'+mean(-accelerationD(:,4)+ADJ)), title('Acceleration of D Joint'), ylabel('A(cm/s^2)'), xlabel('t(s)'), grid on
a=a+1;

VB1X=first_derivative(B1X,dt);
VB1Y=first_derivative(B1Y,dt);
AB1X=second_derivative(B1X,dt);
AB1Y=second_derivative(B1Y,dt);
VB1J=sqrt(VB1X.^2+VB1Y.^2);
AB1J=sqrt(AB1X.^2+AB1Y.^2);
figure(a)
subplot(3,1,1)
plot(B1X,B1Y), title('Position of B1 Joint'), xlabel('x(cm)'), ylabel('y(cm)'), grid on
subplot(3,1,2)
plot(t,VB1J,velocityB1(:,3)',velocityB1(:,4)'+mean(-velocityB1(:,4)+VB1J)), title('Velocity of B1 Joint'), ylabel('V(cm/s)'), xlabel('t(s)'), grid on
subplot(3,1,3)
plot(t,AB1J,accelerationB1(:,3)',accelerationB1(:,4)'+mean(-accelerationB1(:,4)+AB1J)), title('Acceleration of B1 Joint'), ylabel('A(cm/s^2)'), xlabel('t(s)'), grid on
a=a+1;

VE2X=first_derivative(E2X,dt);
VE2Y=first_derivative(E2Y,dt);
AE2X=second_derivative(E2X,dt);
AE2Y=second_derivative(E2Y,dt);
VE2J=sqrt(VE2X.^2+VE2Y.^2);
AE2J=sqrt(AE2X.^2+AE2Y.^2);
figure(a)
subplot(3,1,1)
plot(E2X,E2Y), title('Position of E2 Joint'), xlabel('x(cm)'), ylabel('y(cm)'), grid on
subplot(3,1,2)
plot(t,VE2J,velocityE2(:,3)',velocityE2(:,4)'+mean(-velocityE2(:,4)+VE2J)), title('Velocity of E2 Joint'), ylabel('V(cm/s)'), xlabel('t(s)'), grid on
subplot(3,1,3)
plot(t,AE2J,accelerationE2(:,3)',accelerationE2(:,4)'+mean(-accelerationE2(:,4)+AE2J)), title('Acceleration of E2 Joint'), ylabel('A(cm/s^2)'), xlabel('t(s)'), grid on
a=a+1;

VJX=first_derivative(JX1,dt);
VJY=first_derivative(JY1,dt);
AJX=second_derivative(JX1,dt);
AJY=second_derivative(JY1,dt);
VJJ=sqrt(VJX.^2+VJY.^2);
AJJ=sqrt(AJX.^2+AJY.^2);
figure(a)
subplot(3,1,1)
plot(JX1,JY1), title('Position of J Joint'), xlabel('x(cm)'), ylabel('y(cm)'), grid on
subplot(3,1,2)
plot(t,VJJ,velocityJ(:,3)',velocityJ(:,4)'+mean(-velocityJ(:,4)+VJJ)), title('Velocity of J Joint'), ylabel('V(cm/s)'), xlabel('t(s)'), grid on
subplot(3,1,3)
plot(t,AJJ,accelerationJ(:,3)',accelerationJ(:,4)'+mean(-accelerationJ(:,4)+AJJ)), title('Acceleration of J Joint'), ylabel('A(cm/s^2)'), xlabel('t(s)'), grid on
a=a+1;

VHX=first_derivative(HX,dt);
VHY=first_derivative(HY,dt);
AHX=second_derivative(HX,dt);
AHY=second_derivative(HY,dt);
VHJ=sqrt(VHX.^2+VHY.^2);
AHJ=sqrt(AHX.^2+AHY.^2);
figure(a)
subplot(3,1,1)
plot(HX,HY), title('Position of H Joint'), xlabel('x(cm)'), ylabel('y(cm)'), grid on
subplot(3,1,2)
plot(t,VHJ,velocityH(:,3)',velocityH(:,4)'+mean(-velocityH(:,4)+VHJ)), title('Velocity of H Joint'), ylabel('V(cm/s)'), xlabel('t(s)'), grid on, legend('Matlab Calculation','Motion Track')
subplot(3,1,3)
plot(t,AHJ,accelerationH(:,3)',accelerationH(:,4)'+mean(-accelerationH(:,4)+AHJ)), title('Acceleration of H Joint'), ylabel('A(cm/s^2)'), xlabel('t(s)'), grid on
a=a+1;

%MOTION TRACK
factor=33.542/81.3749;

MT_A2=factor*load('kirmizi.dat')/b; MT_A2X=MT_A2(:,1)'-336.9747+202.5925;   MT_A2Y=-(MT_A2(:,2)'-69.7681)-40.7848;
MT_D=factor*load('mavi.dat')/b;     MT_DX=MT_D(:,1)'-336.9747+202.5925;     MT_DY=-(MT_D(:,2)'-69.7681)-40.7848;
MT_E1=factor*load('pembe.dat')/b;   MT_E1X=MT_E1(:,1)'-336.9747+202.5925;   MT_E1Y=-(MT_E1(:,2)'-69.7681)-40.7848;
MT_B=factor*load('siyah.dat')/b;    MT_BX=MT_B(:,1)'-336.9747+202.5925;     MT_BY=-(MT_B(:,2)'-69.7681)-40.7848;
MT_E2=factor*load('cyan.dat')/b;    MT_E2X=MT_E2(:,1)'-336.9747+202.5925;   MT_E2Y=-(MT_E2(:,2)'-69.7681)-40.7848;
MT_B1=factor*load('yesil.dat')/b;   MT_B1X=MT_B1(:,1)'-336.9747+202.5925;   MT_B1Y=-(MT_B1(:,2)'-69.7681)-40.7848;
MT_J=factor*load('sari.dat')/b;     MT_JX=MT_J(:,1)'-336.9747+202.5925;     MT_JY=-(MT_J(:,2)'-69.7681)-40.7848;
MT_F=factor*load('turuncu.dat')/b;  MT_FX=MT_F(:,1)'-336.9747+202.5925;     MT_FY=-(MT_F(:,2)'-69.7681)-40.7848;
MT_J1=factor*load('beyaz.dat')/b;   MT_J1X=MT_J1(:,1)'-336.9747+202.5925;   MT_J1Y=-(MT_J1(:,2)'-69.7681)-40.7848;
MT_H=factor*load('kahve.dat')/b;    MT_HX=MT_H(:,1)'-336.9747+202.5925;     MT_HY=-(MT_H(:,2)'-69.7681)-40.7848;
MT_G=factor*load('mor.dat')/b;      MT_GX=MT_G(:,1)'-336.9747+202.5925;     MT_GY=-(MT_G(:,2)'-69.7681)-40.7848;
MT_P=load('flap.dat')/b;            MT_PX=MT_P(:,1)'-(MT_P(1,1)-PX(1));      MT_PY=-(MT_P(:,2)'-(MT_H(1,2)-HY(1)));
pfactor=(PX(end)-PX(1))/(MT_PX(end)-MT_PX(1)); MT_PX=pfactor*MT_PX-77.94;  MT_PY=pfactor*MT_PY-12.58;

%FORCES

FA=mA*sqrt(second_derivative(AGX,dt).^2+second_derivative(AGY,dt).^2); FA=mean(FA)*ones(size(FA));
FB=mB*sqrt(second_derivative(BGX,dt).^2+second_derivative(BGY,dt).^2);
FC=mC*sqrt(second_derivative(CGX,dt).^2+second_derivative(CGY,dt).^2);
FD=mD*sqrt(second_derivative(DGX,dt).^2+second_derivative(DGY,dt).^2);
FE=mE*sqrt(second_derivative(EGX,dt).^2+second_derivative(EGY,dt).^2);
FF=mF*sqrt(second_derivative(FGX,dt).^2+second_derivative(FGY,dt).^2);
FG=mG*sqrt(second_derivative(GGX,dt).^2+second_derivative(GGY,dt).^2);
FH=mH*sqrt(second_derivative(HGX,dt).^2+second_derivative(HGY,dt).^2);
FI=mI*sqrt(second_derivative(IGX,dt).^2+second_derivative(IGY,dt).^2);
% FJ=mJ*sqrt(second_derivative(JGX,dt).^2+second_derivative(JGY,dt).^2);

TOTAL_FORCE=FA+FB+FC+FD+FE+FF+FG+FH+FI;

%MOMENTS

MA=abs(IA*second_derivative(THETA,dt)); MA=zeros(size(MA));
MB=abs(IB*second_derivative(PSI,dt));
MC=abs(IC*second_derivative(GAMMA,dt));
MD=abs(ID*second_derivative(BETA,dt));
ME=abs(IE*second_derivative(ALPHA,dt));
MF=abs(IF*second_derivative(KAPPA,dt));
MG=abs(IG*second_derivative(EPSILON,dt));
MH=abs(IH*second_derivative(OMRICON,dt));
MI=abs(IH*second_derivative(TAU,dt));
% MJ=abs(IJ*second_derivative(MU,dt));

TOTAL_MOMENT=MA+MB+MC+MD+ME+MF+MG+MH+MI;

%POWERS

PA= abs(FA .* sqrt(first_derivative(AGX,dt).^2 + first_derivative(AGY,dt).^2)) + abs(MA .* first_derivative(THETA,dt)); PA=mean(PA)*ones(size(PA)); 
PB= abs(FB .* sqrt(first_derivative(BGX,dt).^2 + first_derivative(BGY,dt).^2)) + abs(MB .* first_derivative(PSI,dt));
PC= abs(FC .* sqrt(first_derivative(CGX,dt).^2 + first_derivative(CGY,dt).^2)) + abs(MC .* first_derivative(GAMMA,dt));
PD= abs(FD .* sqrt(first_derivative(DGX,dt).^2 + first_derivative(DGY,dt).^2)) + abs(MD .* first_derivative(BETA,dt));
PE= abs(FE .* sqrt(first_derivative(EGX,dt).^2 + first_derivative(EGY,dt).^2)) + abs(ME .* first_derivative(ALPHA,dt));
PF= abs(FF .* sqrt(first_derivative(FGX,dt).^2 + first_derivative(FGY,dt).^2)) + abs(MF .* first_derivative(KAPPA,dt));
PG= abs(FG .* sqrt(first_derivative(GGX,dt).^2 + first_derivative(GGY,dt).^2)) + abs(MG .* first_derivative(EPSILON,dt));
PH= abs(FH .* sqrt(first_derivative(HGX,dt).^2 + first_derivative(HGY,dt).^2)) + abs(MH .* first_derivative(OMRICON,dt));
PI= abs(FI .* sqrt(first_derivative(IGX,dt).^2 + first_derivative(IGY,dt).^2)) + abs(MI .* first_derivative(TAU,dt));
%PJ= abs(FJ .* sqrt(first_derivative(JGX,dt).^2 + first_derivative(JGY,dt)^2)) + abs(MJ .* first_derivative(THETA,dt));

TOTAL_POWER=PA+PB+PC+PD+PE+PF+PG+PH+PI;

%ENERGY

UA=0.5*mA*(first_derivative(AGX,dt).^2 + first_derivative(AGY,dt).^2); UA=mean(UA)*ones(size(UA));
UE=0.5*mE*(first_derivative(EGX,dt).^2 + first_derivative(EGY,dt).^2);
UF=0.5*mF*(first_derivative(FGX,dt).^2 + first_derivative(FGY,dt).^2);
UH=0.5*mH*(first_derivative(HGX,dt).^2 + first_derivative(HGY,dt).^2);

% figure(a)
% subplot(3,1,1)
% plot(t,TOTAL_FORCE), title('Total Force'), xlabel('F(kg*cm/s^2)'), ylabel('t(s)'), grid on
% subplot(3,1,2)
% plot(t,TOTAL_MOMENT), title('Total Moment'), ylabel('V(cm/s)'), xlabel('t(s)'), grid on
% subplot(3,1,3)
% plot(t,TOTAL_POWER), title('Total Power'), ylabel('P(kg*cm^2/s^2)'), xlabel('t(s)'), grid on
% a=a+1;


figure(a)
subplot(2,2,1)
plot(t,FA), title('Force: Link A'), xlabel('t(s)'), ylabel('F(kg*cm/s^2)'), grid on
subplot(2,2,2)
plot(t,MA), title('Moment: Link A'), xlabel('t(s)'), ylabel('M(kg*cm/s^2)'), grid on
subplot(2,2,3)
plot(t,UA), title('Energy: Link A'), xlabel('t(s)'), ylabel('U(kg*cm^2/s^2)'), grid on
subplot(2,2,4)
plot(t,PA), title('Power: Link A'), xlabel('t(s)'), ylabel('P(kg*cm^2/s^3)'), grid on
a=a+1;

figure(a)
subplot(2,2,1)
plot(t,FE), title('Force: Link E'), xlabel('t(s)'), ylabel('F(kg*cm/s^2)'), grid on
subplot(2,2,2)
plot(t,ME), title('Moment: Link E'), xlabel('t(s)'), ylabel('M(kg*cm/s^2)'), grid on
subplot(2,2,3)
plot(t,UE), title('Energy: Link E'), xlabel('t(s)'), ylabel('U(kg*cm^2/s^2)'), grid on
subplot(2,2,4)
plot(t,PE), title('Power: Link E'), xlabel('t(s)'), ylabel('P(kg*cm^2/s^3)'), grid on
a=a+1;

figure(a)
subplot(2,2,1)
plot(t,FF), title('Force: Link F'), xlabel('t(s)'), ylabel('F(kg*cm/s^2)'), grid on
subplot(2,2,2)
plot(t,MF), title('Moment: Link F'), xlabel('t(s)'), ylabel('M(kg*cm/s^2)'), grid on
subplot(2,2,3)
plot(t,UF), title('Energy: Link F'), xlabel('t(s)'), ylabel('U(kg*cm^2/s^2)'), grid on
subplot(2,2,4)
plot(t,PF), title('Power: Link F'), xlabel('t(s)'), ylabel('P(kg*cm^2/s^3)'), grid on
a=a+1;

figure(a)
subplot(2,2,1)
plot(t,FH), title('Force: Link H'), xlabel('t(s)'), ylabel('F(kg*cm/s^2)'), grid on
subplot(2,2,2)
plot(t,MH), title('Moment: Link H'), xlabel('t(s)'), ylabel('M(kg*cm/s^2)'), grid on
subplot(2,2,3)
plot(t,UH), title('Energy: Link H'), xlabel('t(s)'), ylabel('U(kg*cm^2/s^2)'), grid on
subplot(2,2,4)
plot(t,PH), title('Power: Link H'), xlabel('t(s)'), ylabel('P(kg*cm^2/s^3)'), grid on
a=a+1;

figure(a)
subplot(3,4,1)
plot(MT_A2X,MT_A2Y,A2X,A2Y), title('A2'), grid on, 
subplot(3,4,2)
plot(MT_DX,MT_DY,DX1,DY1), title('D'), grid on
subplot(3,4,3)
plot(MT_E1X,MT_E1Y,E1X,E1Y), title('E'), grid on
subplot(3,4,4)
plot(MT_BX,MT_BY,BX1,BY1), title('B'), grid on
subplot(3,4,5)
plot(MT_E2X,MT_E2Y,E2X,E2Y), title('E2'), grid on
subplot(3,4,6)
plot(MT_B1X,MT_B1Y,B1X,B1Y), title('B1'), grid on
subplot(3,4,7)
plot(MT_JX,MT_JY,JX1,JY1), title('J'), grid on
subplot(3,4,8)
plot(MT_FX,MT_FY,FX,FY), title('F'), grid on
subplot(3,4,9)
plot(MT_J1X,MT_J1Y,J1X,J1Y), title('J1'), grid on
subplot(3,4,10)
plot(MT_HX,MT_HY,HX,HY), title('H'), grid on
subplot(3,4,11)
plot(MT_GX,MT_GY,GX1,GY1), title('G'), grid on
subplot(3,4,12)
plot(MT_PX,MT_PY,PX,PY), title('P'), grid on
a=a+1;

% figure(a)
% 
% pause(2)
% for i=1:3:N
%     text_plot = ['time = ',num2str(t(i),'%7.4f'),' s']; 
%     plot([A1X(i) A2X(i)],[A1Y(i) A2Y(i)],'r-', [A2X(i) DX1(i)],[A2Y(i) DY1(i)],'b-', [EX(i) DX1(i)],[EY(i) DY1(i)],'m-',[EX(i) E1X(i)],[EY(i) E1Y(i)],'m-',[E1X(i) E2X(i)],[E1Y(i) E2Y(i)],'m-',[E2X(i) DX1(i)],[E2Y(i) DY1(i)],'m-',[E1X(i) BX1(i)],[E1Y(i) BY1(i)],'g-',[LX(i) BX1(i)],[LY(i) BY1(i)],'y-',[B1X(i) BX1(i)],[B1Y(i) BY1(i)],'y-',[B1X(i) FX(i)],[B1Y(i) FY(i)],'r-',[JX1(i) FX(i)],[JY1(i) FY(i)],'r-',[E2X(i) JX1(i)],[E2Y(i) JY1(i)],'b-',[FX(i) GX1(i)],[FY(i) GY1(i)],'k-',[J1X(i) GX1(i)],[J1Y(i) GY1(i)],'m-',[J1X(i) HX(i)],[J1Y(i) HY(i)],'m-',[PX(i) HX(i)],[PY(i) HY(i)],'k-',[AGX(i) BGX(i) CGX(i) DGX(i) EGX(i) FGX(i) GGX(i) HGX(i) IGX(i)],[AGY(i) BGY(i) CGY(i) DGY(i) EGY(i) FGY(i) GGY(i) HGY(i) IGY(i)],'ro','linewidth',5)
%     grid on, xlabel('x(cm)'), ylabel('y(cm)'), title(text_plot)
%     axis ([-340/b 20/b -218.5/2/b 218.5/2/b])
%     drawnow
%     pause(0.01)
% end
% a=a+1;





