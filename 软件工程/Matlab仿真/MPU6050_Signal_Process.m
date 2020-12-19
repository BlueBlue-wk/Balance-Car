clc;
clear;
% ��������Դ�ڴ������ֵ�ץȡ��Ϊ�������������У���Ҫ��֤����ÿһ֡�������ԣ�
% ��Ҫɾ�����ݿ�ʼ�ͽ������ֵĲ�������֡����
data=textread('fff.txt','%s');
Size  = 32;
g     = 9.18;
Delta = 0.02;       % �ò���Ϊʵ�ʳ��������ݶ�ȡ����������
len   = floor(length(data) / 32);
Accx  = zeros(1,len);
Accy  = zeros(1,len);
Accz  = zeros(1,len);
Gyrx  = zeros(1,len);
Gyry  = zeros(1,len);
Gyrz  = zeros(1,len);
Pitch = zeros(1,len);
Roll  = zeros(1,len);
Yaw   = zeros(1,len);

%% %%%%%%%%%%%%%%%%%%%%%%%��֡�Ľ���ԭʼ����%%%%%%%%%%%%%%%%%%%%%%%%
for index = 1 : len
    Accx(index) = hex2dec(data(Size * (index - 1) + 4)) * 256 + hex2dec(data(Size * (index - 1) + 5));
    if(Accx(index) > ((2^15) - 1))
        Accx(index) = Accx(index) - 2^16;
    end
    Accy(index) = hex2dec(data(Size * (index - 1) + 6)) * 256 + hex2dec(data(Size * (index - 1) + 7));
    if(Accy(index) > ((2^15) - 1))
        Accy(index) = Accy(index) - 2^16;
    end
        Accz(index) = hex2dec(data(Size * (index - 1) + 8)) * 256 + hex2dec(data(Size * (index - 1) + 9));
    if(Accz(index) > ((2^15) - 1))
        Accz(index) = Accz(index) - 2^16;
    end
        Gyrx(index) = hex2dec(data(Size * (index - 1) + 10)) * 256 + hex2dec(data(Size * (index - 1) + 11));
    if(Gyrx(index) > ((2^15) - 1))
        Gyrx(index) = Gyrx(index) - 2^16;
    end
        Gyry(index) = hex2dec(data(Size * (index - 1) + 12)) * 256 + hex2dec(data(Size * (index - 1) + 13));
    if(Gyry(index) > ((2^15) - 1))
        Gyry(index) = Gyry(index) - 2^16;
    end
        Gyrz(index) = hex2dec(data(Size * (index - 1) + 14)) * 256 + hex2dec(data(Size * (index - 1) + 15));
    if(Gyrz(index) > ((2^15) - 1))
        Gyrz(index) = Gyrz(index) - 2^16;
    end
        Roll(index) = hex2dec(data(Size * (index - 1) + 22)) * 256 + hex2dec(data(Size * (index - 1) + 23));
    if(Roll(index) > ((2^15) - 1))
        Roll(index) = (Roll(index) - 2^16);
    end
        Pitch(index) = hex2dec(data(Size * (index - 1) + 24)) * 256 + hex2dec(data(Size * (index - 1) + 25));
    if(Pitch(index) > ((2^15) - 1))
        Pitch(index) = (Pitch(index) - 2^16);
    end
        Yaw(index) = hex2dec(data(Size * (index - 1) + 26)) * 256 + hex2dec(data(Size * (index - 1) + 27));
    if(Yaw(index) > ((2^15) - 1))
        Yaw(index) = (Yaw(index) - 2^16);
    end
end
Accx  = Accx * 2 * g / (2^15);
Accy  = Accy * 2 * g / (2^15);
Accz  = Accz * 2 * g / (2^15);
Gyrx  = Gyrx * 2000 / (2^15);
Gyry  = Gyry * 2000 / (2^15);
Gyrz  = Gyrz * 2000 / (2^15);
Pitch1 = 180 / pi * atan(Accx ./ sqrt(Accy .^ 2 + Accz .^ 2));
Roll1  = 180 / pi * atan(Accy ./ sqrt(Accx .^ 2 + Accz .^ 2));
Yaw1   = 180 / pi * atan(Accz ./ sqrt(Accy .^ 2 + Accx .^ 2));
Pitch  = Pitch / 100;
Roll   = Roll / 100;
Yaw    = Yaw / 10;

%% %%%%%%%%%%%%%%%%%%%%%%%%%�������ں��˲�%%%%%%%%%%%%%%%%%%%%%%%%%
R  = 1;          % �ɵ��ڸò���
P0 = 0.1;
Q  = 0.001;      % �ɵ��ڸò���
P1 = P0;
E0 = ones(1,len);
kg = ones(1,len);
for k = 2 : len
   E  = E0(k - 1) + Delta * Gyrx(k);
   Pk = P0 + Q;
   Z  = Roll1(k);
   Kg = Pk / (Pk + R);
   E0(k) = E + Kg * (Z - E);
   P0 = (1 - Kg) * Pk;
   kg(k) = Kg;
end


%% %%%%%%%%%%%%%%%%%%%һ�׻����˲�%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
E1 = ones(1,len);
alpha = 0.05;   % �ɵ��ڸò���
for k = 2 : len
   E1(k)  = (1 - alpha) * (E1(k - 1) + Delta * Gyrx(k)) + alpha * Roll1(k);
end
index = (1 : len) * Delta;
figure(1)
plot(index,Roll1,'r-',index,E0(1,:),'b-',index,E1(1,:),'y-',index,Roll,'g-');
legend('���ٶȽ���','�������˲��ں�','һ�׻����˲�','��Ԫ���ں�');
xlabel('ʱ�䣨s��');
ylabel('�Ƕȣ��㣩');



        
