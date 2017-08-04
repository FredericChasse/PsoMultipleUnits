clear all
close all

% load('./DbgData/v1v2v3.mat')

load('./DbgData/testv1_norcfast.mat')
v1 = ts.Data(1,:);
clear ts
load('./DbgData/testv2_norcfast.mat')
v2 = ts.Data(1,:);
clear ts
load('./DbgData/testv3_norcfast.mat')
v3 = ts.Data(1,:);
clear ts
v = [v1' v2' v3'];

yk0 = zeros(1,3);
yk1 = yk0;
yk2 = yk1;
uk0 = yk2;
uk1 = uk0;
uk2 = uk1;
yk0_2 = zeros(1,3);
yk1_2 = yk0_2;
yk2_2 = yk1_2;
uk0_2 = yk2_2;
uk1_2 = uk0_2;
uk2_2 = uk1_2;
yk0_3 = zeros(1,3);
yk1_3 = yk0_3;
yk2_3 = yk1_3;
uk0_3 = yk2_3;
uk1_3 = uk0_3;
uk2_3 = uk1_3;

vout = zeros(length(v1), 3);

wl = 100*2*pi;
% wn = 1526*2*pi;
T = 0.00015;
t = T;
f = 1/T;

for i = 1 : length(v1)
  for j = 1 : 3
    uk2(j) = uk1(j);
    uk1(j) = uk0(j);
    uk0(j) = v(i, j);
    yk2(j) = yk1(j);
    yk1(j) = yk0(j);
    % lpf2 : [wl^(2)*t^(2),2*wl^(2)*t^(2),wl^(2)*t^(2)] / [(wl*t+2)^(2),2*(wl*t-2)*(wl*t+2),(wl*t-2)^(2)]
    yk0(j) = (wl^(2)*t^(2)*uk0(j) + 2*wl^(2)*t^(2)*uk1(j) + wl^(2)*t^(2)*uk2(j) - 2*(wl*t-2)*(wl*t+2)*yk1(j) - (wl*t-2)^(2)*yk2(j))/((wl*t+2)^(2));
    
    uk2_2(j) = uk1_2(j);
    uk1_2(j) = uk0_2(j);
    uk0_2(j) = yk0(j);
    yk2_2(j) = yk1_2(j);
    yk1_2(j) = yk0_2(j);
    yk0_2(j) = (wl^(2)*t^(2)*uk0_2(j) + 2*wl^(2)*t^(2)*uk1_2(j) + wl^(2)*t^(2)*uk2_2(j) - 2*(wl*t-2)*(wl*t+2)*yk1_2(j) - (wl*t-2)^(2)*yk2_2(j))/((wl*t+2)^(2));
    
    uk2_3(j) = uk1_3(j);
    uk1_3(j) = uk0_3(j);
    uk0_3(j) = yk0_2(j);
    yk2_3(j) = yk1_3(j);
    yk1_3(j) = yk0_3(j);
    yk0_3(j) = (wl^(2)*t^(2)*uk0_3(j) + 2*wl^(2)*t^(2)*uk1_3(j) + wl^(2)*t^(2)*uk2_3(j) - 2*(wl*t-2)*(wl*t+2)*yk1_3(j) - (wl*t-2)^(2)*yk2_3(j))/((wl*t+2)^(2));
      % lpf 2
%     ((4*f^(2)*(2*yk1(j)-yk2(j))+4*f*wl*yk2(j)+(uk0(j)+2*uk1(j)+uk2(j)-2*yk1(j)-yk2(j))*wl^(2))/(4*f^(2)+4*f*wl+wl^(2)));    
      % npf 2
%     yk0(j) = ((4*f^2*(uk2(j)-yk2(j) + uk0(j) - 2*uk1(j) - 2*yk1(j))) + (4*f*wn*yk2(j)) + ( (uk2(j)-yk2(j) + uk0(j) + 2*uk1(j) - 2*yk1(j)) * wn^2) ) / (4*f^2 + 4*f*wn + wn^2);
%     yk0(j) = (0.002025*uk0(j) + 0.004051*uk1(j) + 0.002025*uk2(j) + 1.81999*yk1(j) - 0.828088*yk2(j));
    vout(i,j) = yk0_3(j);
  end
end

meanVal = zeros(length(v1),3);
len = length(v1)/4;
for i = 1:3
  for j = 1 : 4
%     meanVal(1+(j-1)*len:len*(j),i) = mean(vout(1+(j-1)*len:len*(j),i));
%     meanVal(1+(j-1)*len:len*(j),i) = max(v(1+(j-1)*len+len/2:len*(j),i));
    meanVal(1+(j-1)*len:len*(j),i) = (max(v(1+(j-1)*len+len/2:len*(j),i))-min(v(1+(j-1)*len+len/2:len*(j),i)))/2+min(v(1+(j-1)*len+len/2:len*(j),i));
  end
end

for i = 1 : 3
  figure
  hold on
  plot(v(:,i))
  plot(vout(:,i))
  plot(meanVal(:,i))
end
    