clearvars -except adcMem nIterations
% close all

figure
hold on
% plot(adcMem)

% admMemMem = adcMem;
% adcMem = bitshift(bitshift(adcMem,-4), 4);

nPackets = nIterations/2;
adcPacketLength = length(adcMem) / nPackets;
t = zeros(nPackets, adcPacketLength);
packets = zeros(nPackets, adcPacketLength, 'uint16');
meanVal = zeros(nPackets, adcPacketLength, 'uint16');
meanVal2 = meanVal;
maxVal = zeros(nPackets, 1, 'uint16');
for i = 1 : nPackets
  t(i,:) = 1+(i-1)*adcPacketLength:i*adcPacketLength;
  packets(i,:) = uint16(adcMem(1+(i-1)*adcPacketLength:i*adcPacketLength));
  maxVal(i) = max(packets(i,:));
%   maxVal(i) = bitshift(bitshift(maxVal(i),-1),1);
    
  for j = 1 : adcPacketLength
    if packets(i, j) < maxVal(i)/2
%     if packets(i, j) < 20
      packets(i, j) = 0;
    else
      packets(i, j) = maxVal(i);
    end
  end
  
  iStart = 1;
  iEnd = adcPacketLength;
  if (packets(i,1) == 0)
    firstVal = maxVal(i);
    lastVal = 0;
  else
    firstVal = 0;
    lastVal = maxVal(i);
  end
  
  for j = 1 : adcPacketLength
    if packets(i,j) == firstVal && packets(i,j+1) == lastVal
      iStart = j+1;
      break;
    end
  end
  
  for j = iEnd-1:-1:1
    if (packets(i,j) == firstVal) && packets(i,j+1) == lastVal
      iEnd = j;
      break;
    end
  end
  newPacket = packets(i, iStart:iEnd);
  meanVal(i,:) = mean(newPacket);
  meanVal2(i,:) = mean(uint16(adcMem(1+(i-1)*adcPacketLength:i*adcPacketLength)));
%   plot(t(i,iStart:iEnd),newPacket)
  plot(t(i,:),meanVal(i,:))
%   plot(t(i,:),meanVal2(i,:))
end