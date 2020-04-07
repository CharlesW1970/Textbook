%Integration Separation PID Controller
clear all;
close all;

ts=20;
%Delay plant
sys=tf([1],[60,1],'inputdelay',80);
dsys=c2d(sys,ts,'zoh');
[num,den]=tfdata(dsys,'v');

u_1=0;u_2=0;u_3=0;u_4=0;u_5=0;
y_1=0;y_2=0;y_3=0;
error_1=0;error_2=0;
ei=0;
for k=1:1:300
time(k)=k*ts;

%Delay plant
y(k)=-den(2)*y_1+num(2)*u_5;

%I separation
if k <= 150
    yd(k)=50;
else
    yd(k)=0;
end
error(k)=yd(k)-y(k);
ei=ei+error(k)*ts;

M=2;
if M==1            %Using integration separation
   if abs(error(k))>=30
      beta=0.0;
   elseif abs(error(k))>=20&abs(error(k))<=30
      beta=0.0;
   elseif abs(error(k))>=10&abs(error(k))<=20
      beta=0.9;
   else
      beta=1.0;
   end
elseif M==2
       beta=1.0;   %Not using integration separation
end

kp=0.80;
ki=0.005;
kd=3.0;
u(k)=kp*error(k)+kd*(error(k)-error_1)/ts+beta*ki*ei;
Iout(k)=beta*ki*ei;
Kout(k)=kp*error(k)
if u(k)>=110       % Restricting the output of controller
   u(k)=110;
end
if u(k)<=-110
   u(k)=-110;
end

u_5=u_4;u_4=u_3;u_3=u_2;u_2=u_1;u_1=u(k);   
y_3=y_2;y_2=y_1;y_1=y(k);
   
error_2=error_1;
error_1=error(k);
end
figure(1);
plot(time,yd,'r',time,y,'k','linewidth',1);
xlabel('time(s)');ylabel('yd,y');
legend('原始信号','控制曲线');
figure(2);
plot(time,u,'r',time,Iout,'k',time,Kout,'b','linewidth',1);
xlabel('time(s)');ylabel('Control input');
legend('PID输出','积分项输出','比例项输出');