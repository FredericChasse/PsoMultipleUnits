clear all
close all

nLoops = 10;

% nSections = 1;  % Must be equal to nPerturbToApply + 1
nSections = 1;  % Must be equal to nPerturbToApply + 1

oDoingLoops = 1;

nUnitsToTest = 15; % Must be equal to nUnits

convTime = zeros(nLoops, nUnitsToTest, nSections);
joulesMem = zeros(nLoops, nSections);
powersMem = zeros(nLoops, nSections);

for iLoop = 1 : nLoops
  clearvars -except iLoop nLoops nSections oDoingLoops convTime joulesMem powersMem
%   close all
  
  loopStr = ['\nDoing loop #' num2str(iLoop) '\n'];
  fprintf(loopStr)
  Pic2Matlab
  
  pause(.1)
end

meanTotalJoules = zeros(1,nSections);
meanConvTime = zeros(1,nSections);
meanPowers = zeros(1, nSections);
for iSection = 1 : nSections
  meanTotalJoules(iSection) = mean(joulesMem(:,iSection));
  meanConvTime   (iSection) = mean(mean(convTime (:,:,iSection)));
  meanPowers     (iSection) = mean(powersMem(:,iSection))*1000;
end

format long g
meanTotalJoules
meanConvTime
meanPowers

clearvars -except meanTotalJoules meanConvTime joulesMem convTime powersMem meanPowers