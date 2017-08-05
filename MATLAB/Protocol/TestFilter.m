clear all
close all

% load('./DbgData/v1v2v3.mat')

load('./DbgData/testv1_ffast.mat')
v1 = ts.Data(1,:);
clear ts
load('./DbgData/testv2_ffast.mat')
v2 = ts.Data(1,:);
clear ts
load('./DbgData/testv3_ffast.mat')
v3 = ts.Data(1,:);
clear ts
v = [v1' v2' v3'];

order = 1;
yk0 = zeros(order,3);
yk1 = yk0;
yk2 = yk1;
uk0 = yk2;
uk1 = uk0;
uk2 = uk1;

vout = zeros(length(v1), 3);

wl = 300*2*pi;
% wn = 1526*2*pi;
T = 0.000001*1;
% T = 0.001*15;
t = T;
f = 1/T;

for i = 1 : length(v1)
  for j = 1 : 3
%     if mod(i, length(v1)/4) == 0
%       for k = 1 : order
%         uk2(k,j) = 0;
%         uk1(k,j) = 0;
%         uk0(k,j) = 0;
%         yk2(k,j) = 0;
%         yk1(k,j) = 0;
%         yk0(k,j) = 0;
%       end
%     end
    for k = 1 : order
      uk2(k,j) = uk1(k,j);
      uk1(k,j) = uk0(k,j);
      if k == 1
        uk0(k,j) = v(i, j);
      else
        uk0(k,j) = yk0(k-1,j);
      end
      yk2(k,j) = yk1(k,j);
      yk1(k,j) = yk0(k,j);
      % lpf2 : [wl^(2)*t^(2),2*wl^(2)*t^(2),wl^(2)*t^(2)] / [(wl*t+2)^(2),2*(wl*t-2)*(wl*t+2),(wl*t-2)^(2)]
      yk0(k,j) = (wl^(2)*t^(2)*uk0(k,j) + 2*wl^(2)*t^(2)*uk1(k,j) + wl^(2)*t^(2)*uk2(k,j) - 2*(wl*t-2)*(wl*t+2)*yk1(k,j) - (wl*t-2)^(2)*yk2(k,j))/((wl*t+2)^(2));
    end
    
      % lpf 2
%     ((4*f^(2)*(2*yk1(j)-yk2(j))+4*f*wl*yk2(j)+(uk0(j)+2*uk1(j)+uk2(j)-2*yk1(j)-yk2(j))*wl^(2))/(4*f^(2)+4*f*wl+wl^(2)));    
      % npf 2
%     yk0(j) = ((4*f^2*(uk2(j)-yk2(j) + uk0(j) - 2*uk1(j) - 2*yk1(j))) + (4*f*wn*yk2(j)) + ( (uk2(j)-yk2(j) + uk0(j) + 2*uk1(j) - 2*yk1(j)) * wn^2) ) / (4*f^2 + 4*f*wn + wn^2);
%     yk0(j) = (0.002025*uk0(j) + 0.004051*uk1(j) + 0.002025*uk2(j) + 1.81999*yk1(j) - 0.828088*yk2(j));
    vout(i,j) = yk0(k,j);
  end
end

meanVal = zeros(length(v1),3);
len = length(v1)/4;
iStart = 1;
iEnd = len;
ratio = 1/2;
for i = 1:3
  for j = 1 : 4
%     meanVal(1+(j-1)*len:len*(j),i) = mean(vout(1+(j-1)*len:len*(j),i));
%     meanVal(1+(j-1)*len:len*(j),i) = max(v(1+(j-1)*len+len/2:len*(j),i));
%     meanVal(1+(j-1)*len:len*(j),i) = (max(vout(1+(j-1)*len+round(len*3/4,0):len*(j),i))-min(vout(1+(j-1)*len+round(len*3/4,0):len*(j),i)))/2+min(vout(1+(j-1)*len+round(len*3/4,0):len*(j),i));
    meanVal(1+(j-1)*len:len*(j),i) = mean(vout(1+(j-1)*len+round(len*ratio,0):len*(j),i));
%     val = v(1+(j-1)*len+len/2:len*j,i);
%     iStart = 1;
%     for k = 2:length(val)
%       if val(k) == 0 && val(k-1) ~= 0
%         iStart = k;
%         break;
%       end
%     end
%     
%     iEnd = length(val);
%     for k = length(val):-1:1
%       if val(k) == 0 && val(k-1) ~= 0
%         iEnd = k-1;
%         break;
%       end
%     end
%     
%     meanVal(1+(j-1)*len:len*j,i) = mean(val(iStart:iEnd));
  end
end
i=2;
% for i = 1 : 3
  figure
  hold on
  plot(v(:,i))
%   plot(vout(:,i))
%   plot(meanVal(:,i))
% end
    