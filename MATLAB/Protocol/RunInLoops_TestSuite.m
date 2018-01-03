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

nLoops = 10;

nSections = 1;  % Must be equal to nPerturbToApply + 1
% nSections = 3;  % Must be equal to nPerturbToApply + 1

oDoingLoops = 1;

nUnitsToTest = 15; % Must be equal to nUnits

convTime = zeros(nLoops, nUnitsToTest, nSections);
joulesMem = zeros(nLoops, nSections);
powersMem = zeros(nLoops, nSections);

meanTotalJoules = zeros(4,nSections);
meanConvTime = zeros(4,nSections);
meanPowers = zeros(4, nSections);

for iLoop = 1 : nLoops
  clearvars -except iLoop nLoops nSections oDoingLoops convTime joulesMem powersMem meanTotalJoules meanConvTime meanPowers nSections
%   close all
  
  loopStr = ['\nDoing loop #' num2str(iLoop) '\n'];
  fprintf(loopStr)
  
  algo = uint8(7);
  iAlgo = 1;
  Pic2Matlab
  for j = 1 : nSections
    meanTotalJoules(iAlgo, j) = meanTotalJoules(iAlgo, j) + joulesMem(iLoop, j);
    meanConvTime(iAlgo, j) = meanConvTime(iAlgo, j) + mean(convTime(iLoop, :, j));
    meanPowers(iAlgo, j) = meanPowers(iAlgo, j) + powersMem(iLoop, j);
  end
  clearvars -except iLoop nLoops nSections oDoingLoops convTime joulesMem powersMem meanTotalJoules meanConvTime meanPowers nSections
  
  pause(2)
  
  algo = uint8(0);
  iAlgo = 2;
  Pic2Matlab
  for j = 1 : nSections
    meanTotalJoules(iAlgo, j) = meanTotalJoules(iAlgo, j) + joulesMem(iLoop, j);
    meanConvTime(iAlgo, j) = meanConvTime(iAlgo, j) + mean(convTime(iLoop, :, j));
    meanPowers(iAlgo, j) = meanPowers(iAlgo, j) + powersMem(iLoop, j);
  end
  clearvars -except iLoop nLoops nSections oDoingLoops convTime joulesMem powersMem meanTotalJoules meanConvTime meanPowers nSections
  
  pause(2)
  
  algo = uint8(1);
  iAlgo = 3;
  Pic2Matlab
  for j = 1 : nSections
    meanTotalJoules(iAlgo, j) = meanTotalJoules(iAlgo, j) + joulesMem(iLoop, j);
    meanConvTime(iAlgo, j) = meanConvTime(iAlgo, j) + mean(convTime(iLoop, :, j));
    meanPowers(iAlgo, j) = meanPowers(iAlgo, j) + powersMem(iLoop, j);
  end
  clearvars -except iLoop nLoops nSections oDoingLoops convTime joulesMem powersMem meanTotalJoules meanConvTime meanPowers nSections
  
  pause(2)
  
  algo = uint8(6);
  iAlgo = 4;
  Pic2Matlab
  for j = 1 : nSections
    meanTotalJoules(iAlgo, j) = meanTotalJoules(iAlgo, j) + joulesMem(iLoop, j);
    meanConvTime(iAlgo, j) = meanConvTime(iAlgo, j) + mean(convTime(iLoop, :, j));
    meanPowers(iAlgo, j) = meanPowers(iAlgo, j) + powersMem(iLoop, j);
  end
  clearvars -except iLoop nLoops nSections oDoingLoops convTime joulesMem powersMem meanTotalJoules meanConvTime meanPowers nSections
  
  pause(2)
end

totalJoules = meanTotalJoules ./ 10;
totalConvTime = meanConvTime ./ 10;
totalPower = meanPowers ./ 10 .*1000;

fprintf('\n')
for i = 1 : nSections
  for j = 1 : 4
    if i == 2
      fprintf([num2str(totalJoules(j, i), '%.5f'), '\t'])
    else
      fprintf([num2str(totalJoules(j, i), '%.4f'), '\t'])
    end
  end
  fprintf('\n')
  for j = 1 : 4
    fprintf([num2str(totalConvTime(j, i),'%.3f'), '\t'])
  end
  fprintf('\n')
  for j = 1 : 4
    fprintf([num2str(totalPower(j, i),'%.4f'), '\t'])
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

clearvars -except meanTotalJoules meanConvTime joulesMem convTime powersMem meanPowers