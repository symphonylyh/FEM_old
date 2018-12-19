%##############������ϴ�����ģ��##################
clear all
%���������ƽ˳��
N=500; %������Ƶ����\\
npt=1; %������
v=250/3.6; %����\\
deta_d=0.05; %���벽��f\\
deta_t=deta_d/v; %ʱ�䲽��\\
S=1000;%���е�·�ܳ�\\
bridge_dis=-100;%�г�����ͷ����\\
tn=floor(S/(deta_t*v));%���㲽��\\
Nspan=3; Nbele=30;
Lb=32; Area=8.8889;  Iy=10.947; %10.947
prime=rand(npt,2*N);
DOF_OneSpan=2*(Nbele+1)-2;    
E=3.45E10;  Dens=2500;  Poisson=0.2;
ShearConstantY=6.12;  ShearConstantZ=1.5;
omega_1=5.12;  omega_2=17.04;  %32M����Ƶ��
% omega_1=8.62;  omega_2=26.4;  %24M����Ƶ��
btrack=0.75;
%  �����ƽ˳����
omegap_f=linspace(1/0.05,1/3,N+1); 
omegap=v./omegap_f;
deta_omega=omegap(1,2:N+1)-omegap(1,1:N);
%ƽ��������ѡ��

for i=1:npt
    for j=1:N
    omegai(i,j)=omegap(1,j)+deta_omega(1,j)*prime(i,j);
    omegai(i,N+j)=2*pi*prime(i,N+j);
    end
end

yNt_u=zeros(npt,tn);
yNt_v=zeros(npt,tn);
yNt_a=zeros(npt,tn);

for i=1:npt
    for t=1:tn
        for j=1:N 
        omega=omegai(i,j);
        Sw=PSD(omega,btrack,v);
%         Sw=PSDmg6(omega,v);        
%         Sw=PSDtk(omega,v);        
        A=sqrt(2*Sw(3)*deta_omega(1,j)/v);%�ߵͲ�ƽ˳
        yNt_u(i,t)=yNt_u(i,t)+A*cos(omegai(i,j)*deta_t*t+omegai(i,N+j));
        yNt_v(i,t)=yNt_v(i,t)-omegai(i,j)*A*sin(omegai(i,j)*deta_t*t+omegai(i,N+j));
        yNt_a(i,t)=yNt_a(i,t)-omegai(i,j)^2*A*cos(omegai(i,j)*deta_t*t+omegai(i,N+j));
        end
    end
end
% ���ƺ���
L0=20;
c=0.65;
g=zeros(npt,tn);
tn1=floor((S/2-Nspan*Lb/2-L0)/v/deta_t);
tn2=floor(L0/v/deta_t);
tn3=floor(Nspan*Lb/v/deta_t);
for i=1:tn
    if 1<=i & i<=tn1
        g(:,i)=1;
    elseif i>tn1 & i<=tn1+tn2
        g(:,i)=(1+c)/2+0.5*(1-c)*cos(pi*((i-tn1-tn2)*deta_t*v+L0)/L0); 
    elseif i>tn1+tn2 & i<=tn1+tn2+tn3
        g(:,i)=c;
    elseif i>tn1+tn2+tn3 & i<=tn1+2*tn2+tn3
        g(:,i)=0.5*(1+c)+0.5*(1-c)*cos(pi*((i-tn1-2*tn2-tn3)*deta_t*v)/L0);
    elseif tn1+2*tn2+tn3<i
        g(:,i)=1;    
    end  
end
yNt_u=yNt_u.*g;
yNt_v=yNt_v.*g;
yNt_a=yNt_a.*g;
%Newmark parameters 
tao=deta_t;
alpha=0.25;beta=0.5;
a0=1/(alpha*tao^2);a1=beta/(alpha*tao);
a2=1/(alpha*tao);a3=1/(2*alpha)-1;
a4=beta/alpha-1;a5=tao*(beta/alpha-2)/2;
a6=tao*(1-beta);a7=beta*tao;
%������������,btrack�������ֹ�֮������һ�룻
%T�ǳ�����ϲ�����������λ�����ֶ�λ�Ƶ�ת����ϵ��
%h4��ʾ�����ֶ�λ������������z����ľ��룬h4Ϊ������ʾ�ֶ�λ���������Ϸ���
%bb��ʾ�ֶ�λ������������y����ľ��룬bbΪ������ʾ�ֶ����������Ҳ࣬��y������
%������λ��{uy,rx,uz}==>�ֶ�λ��{wy,rx,wz}
%hbridge�����������ĵ�����ľ��룬hrail����ṹ�ĸ߶ȣ�hwheel�ֶԵİ뾶
xi=0.05; hbridge=0.78;  hrail=0.5;
hwheel=0.625;  h4=hbridge+hrail+hwheel;  bb=5; 
%��������
Le=Lb/Nbele;
G=E/(2*(1+Poisson));
by=12*ShearConstantY*E*Iy/(G*Area*Le^2);
[Mbsub,Kbsub,Mb,Cb,Kb]=BridgeMatrix(Nbele,DOF_OneSpan,E,Dens,by,Area,Iy,Le,Nspan,omega_1,omega_2,xi);
%�������
%ICE��������
Nvehicle=4;
distance_vehicle=4.9;
nv=6; %ÿ�����ɶ�
nw=4; 
%��������
Mc1=48000;   %��������             (��λ��kg)
Jcy1=2700000;%�����ͷ(��y��)����  ����λ��kg*m2)
Mt1=3200;   %ת�������           (��λ��kg)
Jty1=7200;  %ת��ܵ�ͷ(��y��)��������λ��kg*m2)
Mw1=2400;    %�ֶ�����             (��λ��kg)
lc1=17.375/2;     %���嶨��֮��    ����λ��m)
lt1=2.5/2;    %ת������֮��         ����λ��m)
h11=0.8;    %���嵽��ϵ���Ҵ������  ����λ��m)
h21=0.3;    %��ϵ���ҵ�ת��ܴ�����루��λ��m)
h31=0.05;   %ת��ܵ��ֶԴ������    ����λ��m)
ktv1=1040000;  %һϵ���򵯻ɸն�(һ��) ����λ��N/m)
kcv1=400000;    %��ϵ���򵯻ɸն�(һ��) ����λ��N/m)
ctv1=50000;     %һϵ��������ϵ��(һ��) ����λ��N/m)
ccv1=60000;    %��ϵ��������ϵ��(һ��) ����λ��N/m)

% �ͳ�����
Mc2=44000;    %��������             (��λ��kg)
Jcy2=2700000; %�����ͷ(��y��)����  ����λ��kg*m2)
Mt2=2400;     %ת�������           (��λ��kg)
Jty2=5400;    %ת��ܵ�ͷ(��y��)��������λ��kg*m2)
Mw2=2400;     %�ֶ�����             (��λ��kg)
lc2=17.375/2;     %���嶨��֮��           ����λ��m)
lt2=2.5/2;      %ת������֮��         ����λ��m)
h12=0.8;    %���嵽��ϵ���Ҵ������  ����λ��m)
h22=0.2;    %��ϵ���ҵ�ת��ܴ�����루��λ��m)
h32=0.1;    %ת��ܵ��ֶԴ������    ����λ��m)  
ktv2=700000; %һϵ���򵯻ɸն�(һ��) ����λ��N/m)
kcv2=300000;  %��ϵ���򵯻ɸն�(һ��) ����λ��N/m)
ctv2=50000;   %һϵ��������ϵ��(һ��) ����λ��N/m)
ccv2=60000;   %��ϵ��������ϵ��(һ��) ����λ��N/m)

[Mv1,Kv1,Cv1,Kw1,Cw1,Kh1,Ch1,log_w1]=VehicleMatrix(Mc1,Jcy1,Mt1,Jty1,Mw1,lc1,lt1,h11,h21,h31,kcv1,ktv1,ccv1,ctv1,h4,bb);
[Mv2,Kv2,Cv2,Kw2,Cw2,Kh2,Ch2,log_w2]=VehicleMatrix(Mc2,Jcy2,Mt2,Jty2,Mw2,lc2,lt2,h12,h22,h32,kcv2,ktv2,ccv2,ctv2,h4,bb);

Mv=sparse(zeros(nv*Nvehicle,nv*Nvehicle));
Kv=sparse(zeros(nv*Nvehicle,nv*Nvehicle));
Cv=sparse(zeros(nv*Nvehicle,nv*Nvehicle));
Mw=sparse(zeros(nw*Nvehicle,nw*Nvehicle));
Kw=sparse(zeros(nv*Nvehicle,nw*Nvehicle));
Cw=sparse(zeros(nv*Nvehicle,nw*Nvehicle));
Kh=sparse(zeros(nw*Nvehicle,nw*Nvehicle));
Ch=sparse(zeros(nw*Nvehicle,nw*Nvehicle));
    
location=zeros(4*Nvehicle,1);

for k=1:Nvehicle
    if k==1 || k==Nvehicle
       Substitution1=Kw1;
       Substitution2=Cw1;
       Substitution3=diag([Mw1,Mw1,Mw1,Mw1]);
       Substitution4=Kh1;
       Substitution5=Ch1;
       Mv((k-1)*nv+1:k*nv,(k-1)*nv+1:k*nv)=Mv1;
       Kv((k-1)*nv+1:k*nv,(k-1)*nv+1:k*nv)=Kv1;
       Cv((k-1)*nv+1:k*nv,(k-1)*nv+1:k*nv)=Cv1;
    else
       Substitution1=Kw2;
       Substitution2=Cw2;
       Substitution3=diag([Mw1,Mw1,Mw1,Mw1]);
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

    if k==1
       x=bridge_dis;%4*(lc1+lt1)+distance_vehicle+(Nvehicle-2)*(2*(lt2+lc2)+distance_vehicle)-(S-Lb*Nspan)/2;  %������һ�ֶ�����ͷ����
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
nstep=floor((Nvehicle*26+Nspan*Lb+S/2)/deta_t/v)-floor((location(1,1)+(S-Lb*Nspan)/2)/deta_t/v);
length1=length(Mv);  
length2=length(Mv)+length(Mb);
u_u=zeros(length2,nstep);
v_v=zeros(length2,nstep);
a_a=zeros(length2,nstep);
Firr_u=zeros(4*Nvehicle,nstep);
Firr_v=zeros(4*Nvehicle,nstep);
Firr_a=zeros(4*Nvehicle,nstep);
Firr_g=zeros(4*Nvehicle,nstep);
location=location+v*tao; 
for nt=1:npt
    for n=1:nstep-1
        n
        for i=1:4*Nvehicle
    Firr_u(i,n+1)=yNt_u(nt,floor((location_0(i,1)+(S-Lb*Nspan)/2)/deta_t/v)+n+1);
    Firr_v(i,n+1)=yNt_v(nt,floor((location_0(i,1)+(S-Lb*Nspan)/2)/deta_t/v)+n+1);
    Firr_a(i,n+1)=yNt_a(nt,floor((location_0(i,1)+(S-Lb*Nspan)/2)/deta_t/v)+n+1);
    if i<=4
        Firr_g(i,n+1)=-(Mc1/4+Mt1/2+Mw1)*9.8;
    elseif i>=4*Nvehicle-3
        Firr_g(i,n+1)=-(Mc1/4+Mt1/2+Mw1)*9.8;
        else
        Firr_g(i,n+1)=-(Mc2/4+Mt2/2+Mw2)*9.8;
        end
        end     
    Nshapeb=NshapeBridge(DOF_OneSpan,Nspan,Nvehicle,location,Lb,Le,Nbele);
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
    F(1:length1,n+1)=Kw*Firr_u(:,n+1)+Cw*Firr_v(:,n+1);
    F(length1+1:length2,n+1)=Nshapeb'*Kh*Firr_u(:,n+1)+Nshapeb'*Ch*Firr_v(:,n+1)...
       +Nshapeb'*Mw*Firr_a(:,n+1)+Nshapeb'*Firr_g(:,n+1);   
    F_bar(:,n+1)=F(:,n+1)+M*(a0*u_u(:,n)+a2*v_v(:,n)+a3*a_a(:,n))+...
        C*(a1*u_u(:,n)+a4*v_v(:,n)+a5*a_a(:,n));
  
    u_u(:,n+1)=K_bar\F_bar(:,n+1); %�������������λ�Ʋ�׼ȷ��
    a_a(:,n+1)=a0*(u_u(:,n+1)-u_u(:,n))-a2*v_v(:,n)-a3*a_a(:,n);
    v_v(:,n+1)=v_v(:,n)+a6*a_a(:,n)+a7*a_a(:,n+1);
    location=location+v*tao; 
    end
    % �ֹ���
     for j=1:nstep
        if j<floor(-bridge_dis/deta_d)
            u_bridge(1,j)=0;
        elseif j>=floor(-bridge_dis/deta_d) & j<=floor((-bridge_dis+Nspan*Lb)/deta_d)
            Nb=ceil((j*deta_d+bridge_dis)/Lb);
            Nbe=ceil((j*deta_d+bridge_dis-(Nb-1)*Lb)/Le);
            u_bridge(1,j)=u_u(Nvehicle*6+2*Nbe-2,j)-(u_u(Nvehicle*6+2*Nbe,j)-u_u(Nvehicle*6+2*Nbe-2,j))/Le*(j*deta_d+bridge_dis-(Nb-1)*Lb-(Nbe-1)*Le);
        else
            u_bridge(1,j)=0;
        end
     end
     u_bridge=smooth(u_bridge,20);
     u_bridge=u_bridge';
     for i=1:4*Nvehicle
         N_v=ceil(i/4);
         switch N_v
             case 1 || Nvehicle
            if i-(N_v-1)*4==1
         F_rail(i,:)=2*(ktv1*(u_u((N_v-1)*6+3,:)+lt1*u_u((N_v-1)*6+4,:))+ctv1*(v_v((N_v-1)*6+3,:)+lt1*v_v((N_v-1)*6+4,:)))+(Mc1/4+Mt1/2+Mw1)*9.8;
            elseif i-(N_v-1)*4==2
         F_rail(i,:)=2*(ktv1*(u_u((N_v-1)*6+3,:)-lt1*u_u((N_v-1)*6+4,:))+ctv1*(v_v((N_v-1)*6+3,:)-lt1*v_v((N_v-1)*6+4,:)))+(Mc1/4+Mt1/2+Mw1)*9.8;
          elseif i-(N_v-1)*4==3
         F_rail(i,:)=2*(ktv1*(u_u((N_v-1)*6+5,:)+lt1*u_u((N_v-1)*6+6,:))+ctv1*(v_v((N_v-1)*6+5,:)+lt1*v_v((N_v-1)*6+6,:)))+(Mc1/4+Mt1/2+Mw1)*9.8;
          elseif i-(N_v-1)*4==4
         F_rail(i,:)=2*(ktv1*(u_u((N_v-1)*6+5,:)-lt1*u_u((N_v-1)*6+6,:))+ctv1*(v_v((N_v-1)*6+5,:)-lt1*v_v((N_v-1)*6+6,:)))+(Mc1/4+Mt1/2+Mw1)*9.8;
            end
             otherwise
             if i-(N_v-1)*4==1
         F_rail(i,:)=2*(ktv1*(u_u((N_v-1)*6+3,:)+lt2*u_u((N_v-1)*6+4,:))+ctv1*(v_v((N_v-1)*6+3,:)+lt2*v_v((N_v-1)*6+4,:)))+(Mc1/4+Mt1/2+Mw1)*9.8;
            elseif i-(N_v-1)*4==2
         F_rail(i,:)=2*(ktv1*(u_u((N_v-1)*6+3,:)-lt2*u_u((N_v-1)*6+4,:))+ctv1*(v_v((N_v-1)*6+3,:)-lt2*v_v((N_v-1)*6+4,:)))+(Mc1/4+Mt1/2+Mw1)*9.8;
          elseif i-(N_v-1)*4==3
         F_rail(i,:)=2*(ktv1*(u_u((N_v-1)*6+5,:)+lt2*u_u((N_v-1)*6+6,:))+ctv1*(v_v((N_v-1)*6+5,:)+lt2*v_v((N_v-1)*6+6,:)))+(Mc1/4+Mt1/2+Mw1)*9.8;
          elseif i-(N_v-1)*4==4
         F_rail(i,:)=2*(ktv1*(u_u((N_v-1)*6+5,:)-lt2*u_u((N_v-1)*6+6,:))+ctv1*(v_v((N_v-1)*6+5,:)-lt2*v_v((N_v-1)*6+6,:)))+(Mc1/4+Mt1/2+Mw1)*9.8;
             end
         end
     end
     F_rail=F_rail/1000;%��λ��kN    
     
    %���δ洢
    save(['E:\','uva_',num2str(v*3.6),'_',num2str(N),'_',num2str(Nvehicle),'_',num2str(nt),'.mat'],...
        'u_u','v_v','a_a','F_rail','nstep','bridge_dis','Nvehicle');
    %��Ӧ���㣬���¿�ʼ��һ������
%     u_u=zeros(length2,nstep);
%     v_v=zeros(length2,nstep);
%     a_a=zeros(length2,nstep); 
    %����λ�ûظ���ʼλ��
    location=location_0;
end
disp(['����',num2str(v*3.6),'km/h,�г�',num2str(Nvehicle),'�ڣ���������',num2str(nt),'��'])

