clearvars -except adcMem nIterations
% close all

figure
hold on
% plot(adcMem)

nPackets = nIterations/2;
adcPacketLength = length(adcMem) / nPackets;
t = zeros(nPackets, adcPacketLength);
packets = zeros(nPackets, adcPacketLength, 'uint16');
meanVal = zeros(nPackets, adcPacketLength, 'uint16');
maxVal = zeros(nPackets, 1, 'uint16');
for i = 1 : nPackets
  t(i,:) = 1+(i-1)*adcPacketLength:i*adcPacketLength;
  packets(i,:) = uint16(adcMem(1+(i-1)*adcPacketLength:i*adcPacketLength));
  maxVal(i) = max(packets(i,:));
    
  for j = 1 : adcPacketLength
    if packets(i, j) < maxVal(i)/2
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
  plot(t(i,iStart:iEnd),newPacket)
  plot(t(i,:),meanVal(i,:))
end