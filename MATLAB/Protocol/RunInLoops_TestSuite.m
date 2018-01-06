clear all
close all


% CLASSIC_PSO               = uint8(0);
% PARALLEL_PSO              = uint8(1);
% PARALLEL_PSO_MULTI_SWARM  = uint8(2);
% MULTI_UNIT                = uint8(3);
% EXTREMUM_SEEKING          = uint8(4);
% CHARACTERIZATION          = uint8(5);
% PPSO_PNO                  = uint8(6);
% PNO                       = uint8(7);
% DEBUG_ADC                 = uint8(8);


algosToDo = [1, 1, 1, 1];   % P&O, OEPS, OEPP, OEPPC
% algosToDo = [0,0,0,0];   % P&O, OEPS, OEPP, OEPPC

nLoops = 10;

% nSections = 1;  % Must be equal to nPerturbToApply + 1
nSections = 3;  % Must be equal to nPerturbToApply + 1

oDoingLoops = 1;

nUnitsToTest = 15; % Must be equal to nUnits

convTime = zeros(nLoops, nUnitsToTest, nSections);
joulesMem = zeros(nLoops, nSections);
powersMem = zeros(nLoops, nSections);
efficiencyMem = zeros(nLoops, nSections);
precisionMem = zeros(nLoops, nSections);

meanTotalJoules = zeros(4,nSections);
meanConvTime = zeros(4,nSections);
meanPowers = zeros(4, nSections);
meanEfficiency = zeros(4, nSections);
meanPrecision = zeros(4, nSections);

for iLoop = 1 : nLoops
  clearvars -except iLoop nLoops nSections oDoingLoops convTime joulesMem powersMem meanTotalJoules meanConvTime meanPowers nSections efficiencyMem meanEfficiency precisionMem meanPrecision algosToDo
%   close all
  
  loopStr = ['\nDoing loop #' num2str(iLoop) '\n'];
  fprintf(loopStr)
  
  iAlgo = 1;
  if algosToDo(iAlgo) == 1
    algo = uint8(7);
    Pic2Matlab
    for j = 1 : nSections
      meanTotalJoules(iAlgo, j) = meanTotalJoules(iAlgo, j) + joulesMem(iLoop, j);
      meanConvTime(iAlgo, j) = meanConvTime(iAlgo, j) + mean(convTime(iLoop, :, j));
      meanPowers(iAlgo, j) = meanPowers(iAlgo, j) + powersMem(iLoop, j);
      meanEfficiency(iAlgo, j) = meanEfficiency(iAlgo, j) + efficiencyMem(iLoop, j);
      meanPrecision(iAlgo, j) = meanPrecision(iAlgo, j) + precisionMem(iLoop, j);
    end
    clearvars -except iLoop nLoops nSections oDoingLoops convTime joulesMem powersMem meanTotalJoules meanConvTime meanPowers nSections efficiencyMem meanEfficiency precisionMem meanPrecision algosToDo
    pause(2)
  end
  
  iAlgo = 2;
  if algosToDo(iAlgo) == 1
    algo = uint8(0);
    Pic2Matlab
    for j = 1 : nSections
      meanTotalJoules(iAlgo, j) = meanTotalJoules(iAlgo, j) + joulesMem(iLoop, j);
      meanConvTime(iAlgo, j) = meanConvTime(iAlgo, j) + mean(convTime(iLoop, :, j));
      meanPowers(iAlgo, j) = meanPowers(iAlgo, j) + powersMem(iLoop, j);
      meanEfficiency(iAlgo, j) = meanEfficiency(iAlgo, j) + efficiencyMem(iLoop, j);
      meanPrecision(iAlgo, j) = meanPrecision(iAlgo, j) + precisionMem(iLoop, j);
    end
    clearvars -except iLoop nLoops nSections oDoingLoops convTime joulesMem powersMem meanTotalJoules meanConvTime meanPowers nSections efficiencyMem meanEfficiency precisionMem meanPrecision algosToDo
    pause(2)
  end
  
  iAlgo = 3;
  if algosToDo(iAlgo) == 1
    algo = uint8(1);
    Pic2Matlab
    for j = 1 : nSections
      meanTotalJoules(iAlgo, j) = meanTotalJoules(iAlgo, j) + joulesMem(iLoop, j);
      meanConvTime(iAlgo, j) = meanConvTime(iAlgo, j) + mean(convTime(iLoop, :, j));
      meanPowers(iAlgo, j) = meanPowers(iAlgo, j) + powersMem(iLoop, j);
      meanEfficiency(iAlgo, j) = meanEfficiency(iAlgo, j) + efficiencyMem(iLoop, j);
      meanPrecision(iAlgo, j) = meanPrecision(iAlgo, j) + precisionMem(iLoop, j);
    end
    clearvars -except iLoop nLoops nSections oDoingLoops convTime joulesMem powersMem meanTotalJoules meanConvTime meanPowers nSections efficiencyMem meanEfficiency precisionMem meanPrecision algosToDo
    pause(2)
  end
  
  iAlgo = 4;
  if algosToDo(iAlgo) == 1
    algo = uint8(6);
    Pic2Matlab
    for j = 1 : nSections
      meanTotalJoules(iAlgo, j) = meanTotalJoules(iAlgo, j) + joulesMem(iLoop, j);
      meanConvTime(iAlgo, j) = meanConvTime(iAlgo, j) + mean(convTime(iLoop, :, j));
      meanPowers(iAlgo, j) = meanPowers(iAlgo, j) + powersMem(iLoop, j);
      meanEfficiency(iAlgo, j) = meanEfficiency(iAlgo, j) + efficiencyMem(iLoop, j);
      meanPrecision(iAlgo, j) = meanPrecision(iAlgo, j) + precisionMem(iLoop, j);
    end
    clearvars -except iLoop nLoops nSections oDoingLoops convTime joulesMem powersMem meanTotalJoules meanConvTime meanPowers nSections efficiencyMem meanEfficiency precisionMem meanPrecision algosToDo
    pause(2)
  end
  
end

totalJoules = meanTotalJoules ./ nLoops;
totalConvTime = meanConvTime ./ nLoops;
totalPower = meanPowers ./ nLoops .*1000;
totalEfficiency = meanEfficiency ./ nLoops;
totalPrecision = meanPrecision ./ nLoops;

fprintf('\n')
for i = 1 : nSections
  fprintf('Joules :      ')
  for j = 1 : 4
    if i == 2
      fprintf([num2str(totalJoules(j, i), '%.5f'), '\t'])
    else
      fprintf([num2str(totalJoules(j, i), '%.4f'), '\t'])
    end
  end
  fprintf('\n')
  fprintf('Convergence : ')
  for j = 1 : 4
    fprintf([num2str(totalConvTime(j, i),'%.3f'), '\t'])
  end
  fprintf('\n')
  fprintf('Power :       ')
  for j = 1 : 4
    fprintf([num2str(totalPower(j, i),'%.4f'), '\t'])
  end
  fprintf('\n')
  fprintf('Efficiency :  ')
  for j = 1 : 4
    fprintf([num2str(totalEfficiency(j, i),'%.4f'), '\t'])
  end
  fprintf('\n')
  fprintf('Precision :   ')
  for j = 1 : 4
    fprintf([num2str(totalPrecision(j, i),'%.4f'), '\t'])
  end
  fprintf('\n\n')
end

% for iSection = 1 : nSections
%   meanTotalJoules(iSection) = mean(joulesMem(:,iSection));
%   meanConvTime   (iSection) = mean(mean(convTime (:,:,iSection)));
%   meanPowers     (iSection) = mean(powersMem(:,iSection))*1000;
% end
% 
% format long g
% meanTotalJoules
% meanConvTime
% meanPowers

clearvars -except meanTotalJoules meanConvTime joulesMem convTime powersMem meanPowers meanEfficiency efficiencyMem precisionMem meanPrecision

% f1=.6; f2=.7; f3=.4; f4=.3; f5=.2;
f=[.6, .7, .4, .3, .2, .3, .4, .62, .65, .6];
T = 1900;
t=zeros(1,length(f));
warnWave = [];
amp = .1;
for i = 1 : length(f)
  t(i)=T*f(i);
  warnWave = [warnWave sin(1:f(i):t(i)).*amp]; %#ok<AGROW>
end

audio = audioplayer(warnWave, 22050);
timesToPlay = 1;
for i = 1 : timesToPlay
  play(audio)
  while(audio.isplaying)
    pause(.1)
  end
end

% beep

% load handel
% sound(y,Fs)
% clear y Fs