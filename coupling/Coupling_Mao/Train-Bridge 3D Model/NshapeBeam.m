function Nshape_beam=NshapeBeam(X,Le)

Nshape_beam=zeros(1,12);
Nshape_beam(1)=1-X; %����
Nshape_beam(2)=1-3*X^2+2*X^3;%����
Nshape_beam(3)=1-3*X^2+2*X^3;%����
Nshape_beam(4)=1-X;%Ť��
Nshape_beam(5)=(X-2*X^2+X^3)*Le;%�������
Nshape_beam(6)=(X-2*X^2+X^3)*Le;%�������

Nshape_beam(7)=X;%����
Nshape_beam(8)=3*X^2-2*X^3;%����
Nshape_beam(9)=3*X^2-2*X^3;%����
Nshape_beam(10)=X;
Nshape_beam(11)=(X^3-X^2)*Le;
Nshape_beam(12)=(X^3-X^2)*Le;