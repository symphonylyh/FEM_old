%##############��ά���������ģ��##################
%�����ڵ����ɶȣ�Ux,Uy,Uz,Rox,Roy,Roz %�������ɶȣ�Uy,Rox,Roz,Uz,Roy. %���أ�Fy,Mox,Fz
tic
clear all
N=500;          v=240/3.6;     npt=1;
DetaL=0.05;     deta_t=DetaL/v; %ʱ�䲽�� 
S=1000;     bridge_dis=-150;%�г�����ͷ����
tn=floor(S/(deta_t*v));%���㲽��
%******************������������****************************
% h4 ��ʾ�����ֶ�λ������������z����ľ��룬h4Ϊ������ʾ�ֶ�λ���������Ϸ���
%bb��ʾ�ֶ�λ������������y����ľ��룬bbΪ������ʾ�ֶ����������Ҳ࣬��y������ 
%������λ��{uy,rx,uz}==>�ֶ�λ��{wy,rx,wz}
Nspan=3;        btrack=0.75; %���֮��
Nbele=10;       DOF_OneSpan=6*(Nbele+1)-7;  %���ɶ�  
E=3.45E10;         Dens=2500;  %C60������
Poisson=0.2;    Lb=32;        Area=8.8889;
Iy=10.947;       Iz=86.216;      Jx=22.650;
ShearConstantY=6.12;    ShearConstantZ=1.5;
omega_1=5.12*2*pi;%32M����Ƶ��
omega_2=17.04*2*pi;
% omega_1=8.62*2*pi;%24M����Ƶ��
% omega_2=26.4*2*pi;
xi=0.05;    hbridge=1.14;
hrail=0.5;  hwheel=0.46;   %hbridge�����������ĵ�����ľ��룬hrail����ṹ�ĸ߶ȣ�hwheel�ֶԵİ뾶
h4=hbridge+hrail+hwheel;    bb=2.5;  %����������������ĵ�ƫ�ľ�
Le=Lb/Nbele;    G=E/(2*(1+Poisson));   
by=12*ShearConstantY*E*Iy/(G*Area*Le^2);
bz=12*ShearConstantZ*E*Iz/(G*Area*Le^2);
%***************������������********************************
Nvehicle=8;     distance_vehicle=4.9; %�Ը�����������˵
nv=15; nw=12; %ÿ�����ɶ�/�ֶ����ɶ�4*3

%***************���������ƽ˳********************************
prime=rand(1,2*N);
omegap_f=linspace(1/0.05,1/3,N+1); 
omegap=v./omegap_f;
deta_omega=omegap(1,2:N+1)-omegap(1,1:N);
for i=1:npt   %ƽ��������ѡ��
    for j=1:length(prime)
    xqi(i,j)=i*sqrt(prime(j))-floor(i*sqrt(prime(j)));
    end
end
for i=1:npt   %��ɢ�����任
    for j=1:N
    omegai(i,j)=omegap(1,j)+deta_omega(1,j)*xqi(i,j);
    omegai(i,N+j)=2*pi*xqi(i,N+j);
    end
end
yNt_u=zeros(3*npt,tn);
yNt_v=zeros(3*npt,tn);
yNt_a=zeros(3*npt,tn);
for i=1:npt
    i
    for t=1:tn
        for j=1:N 
        omega=omegai(i,j);
        Sw=PSD(omega,btrack,v);
% %         Sw=PSDmg6(omega,v);        
% %         Sw=PSDtk(omega,v);        
        A1=sqrt(2*Sw(1)*deta_omega(1,j)/v);%����ƽ˳
        yNt_u(3*i-2,t)=yNt_u(3*i-2,t)+A1*cos(omegai(i,j)*deta_t*t+omegai(i,N+j));
        yNt_v(3*i-2,t)=yNt_v(3*i-2,t)-omegai(i,j)*A1*sin(omegai(i,j)*deta_t*t+omegai(i,N+j));
        yNt_a(3*i-2,t)=yNt_a(3*i-2,t)-omegai(i,j)^2*A1*cos(omegai(i,j)*deta_t*t+omegai(i,N+j));
        A2=sqrt(2*Sw(2)*deta_omega(1,j)/v);%ˮƽ��ƽ˳
        yNt_u(3*i-1,t)=yNt_u(3*i-1,t)+A2*cos(omegai(i,j)*deta_t*t+omegai(i,N+j));
        yNt_v(3*i-1,t)=yNt_v(3*i-1,t)-omegai(i,j)*A2*sin(omegai(i,j)*deta_t*t+omegai(i,N+j));
        yNt_a(3*i-1,t)=yNt_a(3*i-1,t)-omegai(i,j)^2*A2*cos(omegai(i,j)*deta_t*t+omegai(i,N+j));
        A3=sqrt(2*Sw(3)*deta_omega(1,j)/v);%�ߵͲ�ƽ˳
        yNt_u(3*i,t)=yNt_u(3*i,t)+A3*cos(omegai(i,j)*deta_t*t+omegai(i,N+j));
        yNt_v(3*i,t)=yNt_v(3*i,t)-omegai(i,j)*A3*sin(omegai(i,j)*deta_t*t+omegai(i,N+j));
        yNt_a(3*i,t)=yNt_a(3*i,t)-omegai(i,j)^2*A3*cos(omegai(i,j)*deta_t*t+omegai(i,N+j));
        end
    end
end
%Newmark parameters
alpha=0.25;beta=0.5;
a0=1/(alpha*deta_t^2);a1=beta/(alpha*deta_t);
a2=1/(alpha*deta_t);a3=1/(2*alpha)-1;
a4=beta/alpha-1;a5=deta_t*(beta/alpha-2)/2;
a6=deta_t*(1-beta);a7=beta*deta_t;

[Mbsub,Kbsub,Mb,Cb,Kb]=BridgeMatrix(Nbele,DOF_OneSpan,E,Dens,G,by,bz,Area,Jx,Iy,Iz,Le,Nspan,omega_1,omega_2,xi);
T=zeros(3,3);%������ϲ�����������λ�����ֶ�λ�Ƶ�ת����ϵ 
T(1,1)=1;T(1,2)=h4;T(2,2)=1;T(3,2)=bb;T(3,3)=1;

Mv=sparse(zeros(nv*Nvehicle,nv*Nvehicle));
Kv=sparse(zeros(nv*Nvehicle,nv*Nvehicle));
Cv=sparse(zeros(nv*Nvehicle,nv*Nvehicle));
Mw=sparse(zeros(nw*Nvehicle,nw*Nvehicle));
Kw=sparse(zeros(nv*Nvehicle,nw*Nvehicle));
Cw=sparse(zeros(nv*Nvehicle,nw*Nvehicle));
Kh=sparse(zeros(nw*Nvehicle,nw*Nvehicle));
Ch=sparse(zeros(nw*Nvehicle,nw*Nvehicle));

location=zeros(4*Nvehicle,1);

%*************ICE�г�����************************
%������δ�����ֶԣ�
Mc1=48000;   %��������             (��λ��kg)
Jcx1=115000; %������(��x��)����  ����λ��kg*m2)
Jcy1=2700000;%�����ͷ(��y��)����  ����λ��kg*m2)
Jcz1=2700000;%����ҡͷ(��z��)����  ����λ��kg*m2)
Mt1=3200;   %ת�������           (��λ��kg)
Jtx1=3200;   %ת��ܲ��(��x��)��������λ��kg*m2)
Jty1=7200;  %ת��ܵ�ͷ(��y��)��������λ��kg*m2)
Jtz1=6800;  %ת���ҡͷ(��z��)��������λ��kg*m2)
Mw1=2400;    %�ֶ�����             (��λ��kg)
Jx1=1200;    %�ֶԲ��(��x��)����  ����λ��kg*m2)
lc1=17.375/2; %���嶨��֮��           ����λ��m)
lt1=1.25;    %ת������֮��         ����λ��m)
bc1=1.0;    %һϵ���Һ�����֮��    ����λ��m)
bt1=0.95;    %��ϵ���Һ�����֮��    ����λ��m)
h11=0.8;    %���嵽��ϵ���Ҵ������  ����λ��m)
h21=0.3;    %��ϵ���ҵ�ת��ܴ�����루��λ��m)
h31=-0.05;   %ת��ܵ��ֶԴ������    ����λ��m)
ktv1=1.04E6;  %һϵ���򵯻ɸն�(һ��) ����λ��N/m)
ktl1=3.0E6;   %һϵ���򵯻ɸն�(һ��) ����λ��N/m)
kcv1=4.0E5;    %��ϵ���򵯻ɸն�(һ��) ����λ��N/m)
kcl1=4.8E5;   %��ϵ���򵯻ɸն�(һ��) ����λ��N/m)
ctv1=5.0E4;     %һϵ��������ϵ��(һ��) ����λ��N/m)
ctl1=0.0E4;    %һϵ��������ϵ��(һ��) ����λ��N/m)
ccv1=6.0E4;    %��ϵ��������ϵ��(һ��) ����λ��N/m)
ccl1=3.0E4;     %��ϵ��������ϵ��(һ��) ����λ��N/m)

% �ͳ�����
Mc2=44000;Jcx2=100000;Jcy2=2700000;Jcz2=2700000;
Mt2=2400;Jtx2=2400;Jty2=5400;Jtz2=5100;
Mw2=2400;Jx2=1200;
lc2=17.375/2;lt2=1.25;bc2=1.0;bt2=0.95;h12=0.8;h22=0.2;h32=0.1;  
kcv2=7.0E5;kcl2=5.0E6;ktv2=3.0E5;ktl2=5.6E5;
ccv2=5.0E4;ccl2=0.0E4;ctv2=6.0E4;ctl2=2.5E4;

%***************************************************************

[Mv1,Kv1,Cv1,Kw1,Cw1,Kh1,Ch1,log_w1]=VehicleMatrix(Mc1,Jcx1,Jcy1,Jcz1,Mt1,Jtx1,Jty1,Jtz1,Mw1,Jx1,lc1,lt1,bc1,bt1,h11,h21,h31,kcv1,kcl1,ktv1,ktl1,ccv1,ccl1,ctv1,ctl1,h4,bb);
[Mv2,Kv2,Cv2,Kw2,Cw2,Kh2,Ch2,log_w2]=VehicleMatrix(Mc2,Jcx2,Jcy2,Jcz2,Mt2,Jtx2,Jty2,Jtz2,Mw2,Jx2,lc2,lt2,bc2,bt2,h12,h22,h32,kcv2,kcl2,ktv2,ktl2,ccv2,ccl2,ctv2,ctl2,h4,bb);

for k=1:Nvehicle
    if k==1 || k==Nvehicle
       Substitution1=Kw1;
       Substitution2=Cw1;
       Substitution3=diag([Mw1,Jx1,Mw1,Mw1,Jx1,Mw1,Mw1,Jx1,Mw1,Mw1,Jx1,Mw1]);
       Substitution4=Kh1;
       Substitution5=Ch1;
       Mv((k-1)*nv+1:k*nv,(k-1)*nv+1:k*nv)=Mv1;
       Kv((k-1)*nv+1:k*nv,(k-1)*nv+1:k*nv)=Kv1;
       Cv((k-1)*nv+1:k*nv,(k-1)*nv+1:k*nv)=Cv1;
    else
       Substitution1=Kw2;
       Substitution2=Cw2;
       Substitution3=diag([Mw2,Jx2,Mw2,Mw2,Jx2,Mw2,Mw2,Jx2,Mw2,Mw2,Jx2,Mw2]);
       Substitution4=Kh2;
       Substitution5=Ch2;
       Mv((k-1)*nv+1:k*nv,(k-1)*nv+1:k*nv)=Mv2;
       Kv((k-1)*nv+1:k*nv,(k-1)*nv+1:k*nv)=Kv2;
       Cv((k-1)*nv+1:k*nv,(k-1)*nv+1:k*nv)=Cv2;
    end
    Kw((k-1)*nv+1:k*nv,(k-1)*nw+1:k*nw)=Substitution1;
    Cw((k-1)*nv+1:k*nv,(k-1)*nw+1:k*nw)=Substitution2;
    Mw((k-1)*nw+1:k*nw,(k-1)*nw+1:k*nw)=Substitution3;
    Kh((k-1)*nw+1:k*nw,(k-1)*nw+1:k*nw)=Substitution4;
    Ch((k-1)*nw+1:k*nw,(k-1)*nw+1:k*nw)=Substitution5;

    %��������λ�ú�������ͷ����Ϊ�����򣬵�һ������Ϊ0���ꡣ
    if k==1
       x=bridge_dis;
       location(1:4)=[x,x-2*lt1,x-2*lc1,x-2*(lt1+lc1)]; 
    elseif k==Nvehicle
       x=location((k-1)*4)-distance_vehicle;
       location((k-1)*4+1:k*4)=[x,x-2*lt1,x-2*lc1,x-2*(lt1+lc1)];              
    else
       x=location((k-1)*4)-distance_vehicle;
       location((k-1)*4+1:k*4)=[x,x-2*lt2,x-2*lc2,x-2*(lt2+lc2)];    
    end 
end    
location_0=location;
nstep=floor((Nvehicle*24.775+Nspan*Lb+S/2)/deta_t/v)-floor((location(1,1)+(S-Lb*Nspan)/2)/deta_t/v);
length1=length(Mv);
length2=length(Mv)+length(Mb);
u_u=zeros(length2,nstep);
v_v=zeros(length2,nstep);
a_a=zeros(length2,nstep);
Firr_u=zeros(nw*Nvehicle,nstep);
Firr_v=zeros(nw*Nvehicle,nstep);
Firr_a=zeros(nw*Nvehicle,nstep);
Firr_g=zeros(nw*Nvehicle,nstep);
%

location=location+v*deta_t;
for nt=1:npt
    nt
for n=1:nstep-1
    for i=1:4*Nvehicle
    Firr_u(3*i-2:3*i,n+1)=yNt_u(3*nt-2:3*nt,floor((location_0(i,1)+(S-Lb*Nspan)/2)/deta_t/v)+n+1);
    Firr_v(3*i-2:3*i,n+1)=yNt_v(3*nt-2:3*nt,floor((location_0(i,1)+(S-Lb*Nspan)/2)/deta_t/v)+n+1);
    Firr_a(3*i-2:3*i,n+1)=yNt_a(3*nt-2:3*nt,floor((location_0(i,1)+(S-Lb*Nspan)/2)/deta_t/v)+n+1);
    Firr_u(:,1)=Firr_u(:,2)*0.9;Firr_v(:,1)=Firr_v(:,2)*0.9;Firr_a(:,1)=Firr_a(:,2)*0.9;
    if i<=4
        Firr_g(3*i,n+1)=-(Mv((ceil(i/4)-1)*nv+1,(ceil(i/4)-1)*nv+1)/4+Mt1/2+Mw1)*9.8;
    elseif i>=4*Nvehicle-3
        Firr_g(3*i,n+1)=-(Mv((ceil(i/4)-1)*nv+1,(ceil(i/4)-1)*nv+1)/4+Mt1/2+Mw1)*9.8;
    else
        Firr_g(3*i,n+1)=-(Mv((ceil(i/4)-1)*nv+1,(ceil(i/4)-1)*nv+1)/4+Mt2/2+Mw2)*9.8;
    end
    end

    Nshapeb=NshapeBridge(DOF_OneSpan,Nspan,Nvehicle,location,Lb,Le,Nbele,T);
    Md=Nshapeb'*Mw*Nshapeb;
    Kd=Nshapeb'*Kh*Nshapeb;
    Cd=Nshapeb'*Ch*Nshapeb;
    
    M(1:length1,1:length1)=Mv;
    C(1:length1,1:length1)=Cv;
    K(1:length1,1:length1)=Kv;
    
    M(length1+1:length2,length1+1:length2)=Mb+Md;
    C(length1+1:length2,length1+1:length2)=Cb-Cd;
    K(length1+1:length2,length1+1:length2)=Kb-Kd;
    
    C(1:length1,length1+1:length2)=-Cw*Nshapeb; 
    C(length1+1:length2,1:length1)=-Nshapeb'*Cw';
    K(1:length1,length1+1:length2)=-Kw*Nshapeb;
    K(length1+1:length2,1:length1)=-Nshapeb'*Kw';
    
    K_bar=K+a0*M+a1*C;
    %L=chol(K_bar);
    
    F(1:length1,n+1)=Kw*Firr_u(:,n+1)+Cw*Firr_v(:,n+1);
    F(length1+1:length2,n+1)=Nshapeb'*Kh*Firr_u(:,n+1)+Nshapeb'*Ch*Firr_v(:,n+1)...
       +Nshapeb'*Mw*Firr_a(:,n+1)+Nshapeb'*Firr_g(:,n+1);
    
    F_bar(:,n+1)=F(:,n+1)+M*(a0*u_u(:,n)+a2*v_v(:,n)+a3*a_a(:,n))+...
        C*(a1*u_u(:,n)+a4*v_v(:,n)+a5*a_a(:,n));
   
    u_u(:,n+1)=K_bar\F_bar(:,n+1);
    a_a(:,n+1)=a0*(u_u(:,n+1)-u_u(:,n))-a2*v_v(:,n)-a3*a_a(:,n);
    v_v(:,n+1)=v_v(:,n)+a6*a_a(:,n)+a7*a_a(:,n+1);
 
    location=location+v*deta_t; 

end

    u_bridge_y=zeros(1,nstep);
    u_bridge_z=zeros(1,nstep);
    v_bridge_y=zeros(1,nstep);
    v_bridge_z=zeros(1,nstep);% ����λ��
     for j=1:nstep
        if j<floor(-bridge_dis/DetaL)
            u_bridge_y(1,j)=0;
            u_bridge_z(1,j)=0;
            v_bridge_y(1,j)=0;
            v_bridge_z(1,j)=0;            
        elseif j>=floor(-bridge_dis/DetaL) & j<=floor((-bridge_dis+Nspan*Lb)/DetaL)
            Nb=ceil((j*DetaL+bridge_dis)/Lb);
            Nbe=ceil((j*DetaL+bridge_dis-(Nb-1)*Lb)/Le);
            switch Nbe
                case 1
                    Nbe=2;
                case 10
                    Nbe=9;
                otherwise
                    Nbe;
            end
            u_bridge_y(1,j)=u_u(Nvehicle*15+6*Nbe-8,j)+(u_u(Nvehicle*15+6*Nbe-2,j)-u_u(Nvehicle*15+6*Nbe-8,j))/Le*(j*DetaL+bridge_dis-(Nb-1)*Lb-(Nbe-1)*Le)...
                         +(hbridge+hrail)*(u_u(Nvehicle*15+6*Nbe-6,j)+(u_u(Nvehicle*15+6*Nbe,j)-u_u(Nvehicle*15+6*Nbe-6,j))/Le*(j*DetaL+bridge_dis-(Nb-1)*Lb-(Nbe-1)*Le));
            u_bridge_z(1,j)=u_u(Nvehicle*15+6*Nbe-7,j)+(u_u(Nvehicle*15+6*Nbe-1,j)-u_u(Nvehicle*15+6*Nbe-7,j))/Le*(j*DetaL+bridge_dis-(Nb-1)*Lb-(Nbe-1)*Le)...
                         +bb*(u_u(Nvehicle*15+6*Nbe-6,j)+(u_u(Nvehicle*15+6*Nbe,j)-u_u(Nvehicle*15+6*Nbe-6,j))/Le*(j*DetaL+bridge_dis-(Nb-1)*Lb-(Nbe-1)*Le));
            v_bridge_y(1,j)=v_v(Nvehicle*15+6*Nbe-8,j)+(v_v(Nvehicle*15+6*Nbe-2,j)-v_v(Nvehicle*15+6*Nbe-8,j))/Le*(j*DetaL+bridge_dis-(Nb-1)*Lb-(Nbe-1)*Le)...
                         +(hbridge+hrail)*(v_v(Nvehicle*15+6*Nbe-6,j)+(v_v(Nvehicle*15+6*Nbe,j)-v_v(Nvehicle*15+6*Nbe-6,j))/Le*(j*DetaL+bridge_dis-(Nb-1)*Lb-(Nbe-1)*Le));
            v_bridge_z(1,j)=v_v(Nvehicle*15+6*Nbe-7,j)+(v_v(Nvehicle*15+6*Nbe-1,j)-v_v(Nvehicle*15+6*Nbe-7,j))/Le*(j*DetaL+bridge_dis-(Nb-1)*Lb-(Nbe-1)*Le)...
                         +bb*(v_v(Nvehicle*15+6*Nbe-6,j)+(v_v(Nvehicle*15+6*Nbe,j)-v_v(Nvehicle*15+6*Nbe-6,j))/Le*(j*DetaL+bridge_dis-(Nb-1)*Lb-(Nbe-1)*Le));
        else
        end
     end
     u_bridge_y=smooth(u_bridge_y,20)';
     u_bridge_z=smooth(u_bridge_z,20)';
     v_bridge_y=smooth(v_bridge_y,20)';
     v_bridge_z=smooth(v_bridge_z,20)';
     F_rail=zeros(4,nstep);
     
     for i=1 %��i���ֶ�   %F_rail  1:4 ����������򡢺����ұ����򡢺���
         Nv=ceil(i/4);
         if Nv==1 || Nvehicle
             bt=bt1;lt=lt1;ktl=ktl1;ctl=ctl1;ktv=ktv1;ctv=ctv1;Mc=Mc1;Mt=Mt1;Mw=Mw1;
         else
             bt=bt2;lt=lt2;ktl=ktl2;ctl=ctl2;ktv=ktv2;ctv=ctv2;Mc=Mc2;Mt=Mt2;Mw=Mw2;
         end
         Nvv=i-Nv*4+4;
         
        switch Nvv
            case 1     
                F_rail(1,:)=ktv*(u_u(Nv*15-6,:)+lt*u_u(Nv*15-5,:)-bt*u_u(Nv*15-8,:)-Firr_u(3*i,:)+btrack*Firr_u(3*i-1,:)-u_bridge_z(1,:))+(Mc/8+Mt/4+Mw/2)*9.8...
                           +ctv*(v_v(Nv*15-6,:)+lt*v_v(Nv*15-5,:)-bt*v_v(Nv*15-8,:)-Firr_v(3*i,:)+btrack*Firr_v(3*i-1,:)-v_bridge_z(1,:));
                F_rail(2,:)=ktl*(u_u(Nv*15-9,:)+u_u(Nv*15-7,:)-Firr_u(3*i-2,:)-u_bridge_y(1,:))...
                           +ctl*(v_v(Nv*15-9,:)+v_v(Nv*15-7,:)-Firr_v(3*i-2,:)-v_bridge_y(1,:));
                F_rail(3,:)=ktv*(u_u(Nv*15-6,:)+lt*u_u(Nv*15-5,:)+bt*u_u(Nv*15-8,:)-Firr_u(3*i,:)-btrack*Firr_u(3*i-1,:)-u_bridge_z(1,:))+(Mc/8+Mt/4+Mw/2)*9.8...
                           +ctv*(v_v(Nv*15-6,:)+lt*v_v(Nv*15-5,:)+bt*v_v(Nv*15-8,:)-Firr_v(3*i,:)-btrack*Firr_v(3*i-1,:)-v_bridge_z(1,:));
                F_rail(4,:)=ktl*(u_u(Nv*15-9,:)+u_u(Nv*15-7,:)-Firr_u(3*i-2,:)-u_bridge_y(1,:))...
                           +ctl*(v_v(Nv*15-9,:)+v_v(Nv*15-7,:)-Firr_v(3*i-2,:)-v_bridge_y(1,:));
            case 2     
                F_rail(1,:)=ktv*(u_u(Nv*15-6,:)-lt*u_u(Nv*15-5,:)-bt*u_u(Nv*15-8,:)-Firr_u(3*i,:)+btrack*Firr_u(3*i-1,:)-u_bridge_z(1,:))+(Mc/8+Mt/4+Mw/2)*9.8...
                           +ctv*(v_v(Nv*15-6,:)-lt*v_v(Nv*15-5,:)-bt*v_v(Nv*15-8,:)-Firr_v(3*i,:)+btrack*Firr_v(3*i-1,:)-v_bridge_z(1,:));
                F_rail(2,:)=ktl*(u_u(Nv*15-9,:)-u_u(Nv*15-7,:)-Firr_u(3*i-2,:)-u_bridge_y(1,:))...
                           +ctl*(v_v(Nv*15-9,:)-v_v(Nv*15-7,:)-Firr_v(3*i-2,:)-v_bridge_y(1,:));
                F_rail(3,:)=ktv*(u_u(Nv*15-6,:)-lt*u_u(Nv*15-5,:)+bt*u_u(Nv*15-8,:)-Firr_u(3*i,:)-btrack*Firr_u(3*i-1,:)-u_bridge_z(1,:))+(Mc/8+Mt/4+Mw/2)*9.8...
                           +ctv*(v_v(Nv*15-6,:)-lt*v_v(Nv*15-5,:)+bt*v_v(Nv*15-8,:)-Firr_v(3*i,:)-btrack*Firr_v(3*i-1,:)-v_bridge_z(1,:));
                F_rail(4,:)=ktl*(u_u(Nv*15-9,:)-u_u(Nv*15-7,:)-Firr_u(3*i-2,:)-u_bridge_y(1,:))...
                           +ctl*(v_v(Nv*15-9,:)-v_v(Nv*15-7,:)-Firr_v(3*i-2,:)-v_bridge_y(1,:));
            case 3     
                F_rail(1,:)=ktv*(u_u(Nv*15-1,:)+lt*u_u(Nv*15,:)-bt*u_u(Nv*15-3,:)-Firr_u(3*i,:)+btrack*Firr_u(3*i-1,:)-u_bridge_z(1,:))+(Mc/8+Mt/4+Mw/2)*9.8...
                           +ctv*(v_v(Nv*15-1,:)+lt*v_v(Nv*15,:)-bt*v_v(Nv*15-3,:)-Firr_v(3*i,:)+btrack*Firr_v(3*i-1,:)-v_bridge_z(1,:));
                F_rail(2,:)=ktl*(u_u(Nv*15-4,:)+u_u(Nv*15-2,:)-Firr_u(3*i-2,:)-u_bridge_y(1,:))...
                           +ctl*(v_v(Nv*15-4,:)+v_v(Nv*15-2,:)-Firr_v(3*i-2,:)-v_bridge_y(1,:));
                F_rail(3,:)=ktv*(u_u(Nv*15-1,:)+lt*u_u(Nv*15,:)+bt*u_u(Nv*15-3,:)-Firr_u(3*i,:)-btrack*Firr_u(3*i-1,:)-u_bridge_z(1,:))+(Mc/8+Mt/4+Mw/2)*9.8...
                           +ctv*(v_v(Nv*15-1,:)+lt*v_v(Nv*15,:)+bt*v_v(Nv*15-3,:)-Firr_v(3*i,:)-btrack*Firr_v(3*i-1,:)-v_bridge_z(1,:));
                F_rail(4,:)=ktl*(u_u(Nv*15-4,:)+u_u(Nv*15-2,:)-Firr_u(3*i-2,:)-u_bridge_y(1,:))...
                           +ctl*(v_v(Nv*15-4,:)+v_v(Nv*15-2,:)-Firr_v(3*i-2,:)-v_bridge_y(1,:));
            case 4     
                F_rail(1,:)=ktv*(u_u(Nv*15-1,:)-lt*u_u(Nv*15,:)-bt*u_u(Nv*15-3,:)-Firr_u(3*i,:)+btrack*Firr_u(3*i-1,:)-u_bridge_z(1,:))+(Mc/8+Mt/4+Mw/2)*9.8...
                           +ctv*(v_v(Nv*15-1,:)-lt*v_v(Nv*15,:)-bt*v_v(Nv*15-3,:)-Firr_v(3*i,:)+btrack*Firr_v(3*i-1,:)-v_bridge_z(1,:));
                F_rail(2,:)=ktl*(u_u(Nv*15-4,:)-u_u(Nv*15-2,:)-Firr_u(3*i-2,:)-u_bridge_y(1,:))...
                           +ctl*(v_v(Nv*15-4,:)-v_v(Nv*15-2,:)-Firr_v(3*i-2,:)-v_bridge_y(1,:));
                F_rail(3,:)=ktv*(u_u(Nv*15-1,:)-lt*u_u(Nv*15,:)+bt*u_u(Nv*15-3,:)-Firr_u(3*i,:)-btrack*Firr_u(3*i-1,:)-u_bridge_z(1,:))+(Mc/8+Mt/4+Mw/2)*9.8...
                           +ctv*(v_v(Nv*15-1,:)-lt*v_v(Nv*15,:)+bt*v_v(Nv*15-3,:)-Firr_v(3*i,:)-btrack*Firr_v(3*i-1,:)-v_bridge_z(1,:));
                F_rail(4,:)=ktl*(u_u(Nv*15-4,:)-u_u(Nv*15-2,:)-Firr_u(3*i-2,:)-u_bridge_y(1,:))...
                           +ctl*(v_v(Nv*15-4,:)-v_v(Nv*15-2,:)-Firr_v(3*i-2,:)-v_bridge_y(1,:));
        end
     end
        F_rail=F_rail/1000;%��λ��kN
    u_u=u_u([1:30,148:152,207:211,266:270],:);
    a_a=a_a([1:30,148:152,207:211,266:270],:);
    %���δ洢
%     save(['E:\����\','uva_',num2str(v*3.6),'_',num2str(Nvehicle),'_',num2str(nt),'.mat']);
% 
    %��Ӧ���㣬���¿�ʼ��һ������
%     u_u=zeros(length2,nstep);
%     v_v=zeros(length2,nstep);
%     a_a=zeros(length2,nstep); 
    %����λ�ûظ���ʼλ��
    location=location_0;

end
toc
