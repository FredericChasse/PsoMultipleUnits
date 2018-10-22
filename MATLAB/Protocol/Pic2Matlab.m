if ~exist('oDoingLoops', 'var')
  clear
%   close all
end

% Next 2 lines are to close any open waitbar
f = findall(0,'tag','TMWWaitbar');
delete(f);

%% Constants

PROTOCOL_DELIMITER        = uint8(126);

NEW_RNG_SEED              = uint8(0);
START_ACQ                 = uint8(1);
STOP_ACQ                  = uint8(2);
SET_PERTURB               = uint8(3);
SET_DEBUG_DATA            = uint8(4);
INIT_PERTURB              = uint8(5);
UNITS_DATA                = uint8(6);
PSO_DATA                  = uint8(7);
ADC_DATA                  = uint8(8);
PPSO_PNO_DATA             = uint8(9);

CLASSIC_PSO               = uint8(0);
PARALLEL_PSO              = uint8(1);
PARALLEL_PSO_MULTI_SWARM  = uint8(2);
MULTI_UNIT                = uint8(3);
EXTREMUM_SEEKING          = uint8(4);
CHARACTERIZATION          = uint8(5);
PPSO_PNO                  = uint8(6);
PNO                       = uint8(7);
DEBUG_ADC                 = uint8(8);
PPSOCD                    = uint8(9);

PROTOCOL_START_ALGO       = uint8('!' - 0);
PROTOCOL_STOP_ALGO        = uint8('x' - 0);

SIZE_OF_PROTOCOL_HEADER   = 4;

N_UNITS_TOTAL             = 15;

%% Port setup

if ~exist('oDoingLoops', 'var')
  % Wait for the user to start the program
  reply = 'N';
  while reply == 'N'
    reply = upper(input('Start? Y/N [N]:','s'));
    if isempty(reply)
      reply = 'Y'
    end
  end
end

% Create a serial port object.
comPort = 'COM4';
% comPort = 'COM3';
port = instrfind('Type', 'serial', 'Port', comPort, 'Tag', '');

if isempty(port)
    port = serial(comPort);
else
    % Remove contents of input buffer
    flushinput(port);
    % Remove contents of output buffer
    flushoutput(port);
    fclose(port);
    port = port(1);
end

% port.BytesAvailableFcnMode = 'byte';
% port.BytesAvailableFcn = {@myCallback};

port.BaudRate = 115200;
% port.BaudRate = 57600;
port.DataBits = 8;
port.Parity = 'none';
port.StopBits = 1;
port.Terminator = '';

% Connect to instrument object, port.
fopen(port);

%% Data setup

% RNG Seed
delimiter = PROTOCOL_DELIMITER;
typeOfMsg = NEW_RNG_SEED;
lengthOfPayload = typecast(uint16(16), 'uint8');
[seed1, seed2] = GenerateNewSeeds;
% seed1 = uint64(3687972931876243762);
% seed2 = uint64(3111786400007884411);
seeds = typecast([seed1, seed2], 'uint8');

buf = [delimiter, typeOfMsg, lengthOfPayload, seeds];
fwrite(port, buf);

% Initial intensity
initLedIntensity = 500;
delimiter = PROTOCOL_DELIMITER;
typeOfMsg = INIT_PERTURB;
lengthOfPayload = typecast(uint16(2), 'uint8');
payload = typecast(uint16(initLedIntensity), 'uint8');

buf = [delimiter, typeOfMsg, lengthOfPayload, payload];
fwrite(port, buf);

% Perturb
nPerturbs = 2;
% nPerturbs = 1;
% nPerturbs = 0;
oDoPerturb = 0;
% oSlowPerturb = 1;
oSlowPerturb = 0;

if nPerturbs > 0
  if oSlowPerturb
    oDoPerturb = 1;
    perturbAmps = zeros(1, nPerturbs);
    perturbUnits = cell(1, nPerturbs);
    perturbIterations = zeros(nPerturbs, 2);
    perturbAmps(1) = -1;
    perturbUnits{1} = [0:1:14];
%     perturbUnits{1} = [0:1:7];
    perturbIterations(1, :) = [100 300];

    if nPerturbs >= 2
%       perturbUnits{2} = [0:1:7];
      perturbUnits{2} = [0:1:14];
      perturbAmps(2) = 1;
      perturbIterations(2, :) = [500 700];
    end
  else
    oDoPerturb = 1;
    perturbAmps = zeros(1, nPerturbs);
    perturbUnits = cell(1, nPerturbs);
    perturbIterations = zeros(nPerturbs, 2);
    perturbAmps(1) = -200;
    perturbUnits{1} = [0:1:14];
%     perturbUnits{1} = [0:1:7];
    perturbIterations(1, :) = [300 301];

    if nPerturbs >= 2
%       perturbUnits{2} = [0:1:7];
      perturbUnits{2} = [0:1:14];
      perturbAmps(2) = 200;
      perturbIterations(2, :) = [600 601];
    end
  end
end

perturbPayloadBaseLength = 4 + 4 + 2 + 1;
delimiter = PROTOCOL_DELIMITER;
typeOfMsg = SET_PERTURB;
for i = 1 : nPerturbs
  startIteration = typecast(uint32(perturbIterations(i, 1)), 'uint8');
  endIteration = typecast(uint32(perturbIterations(i, 2)), 'uint8');
  amplitude = typecast(int16(perturbAmps(i)), 'uint8');
  nUnits = uint8(length(perturbUnits{i}));
  units = uint8(perturbUnits{i});
  
  lengthOfPayload = typecast(uint16(perturbPayloadBaseLength + length(perturbUnits{i})), 'uint8');
  buf = [delimiter, typeOfMsg, lengthOfPayload, startIteration, endIteration, amplitude, nUnits, units];
  fwrite(port, buf)
end

% Start algo
typeOfMsg = START_ACQ;
startAlgoChar = PROTOCOL_START_ALGO;

if ~exist('algo', 'var')
%   algo = CHARACTERIZATION;
%   algo = PNO;
  algo = PPSOCD;
%   algo = CLASSIC_PSO;
  % algo = PARALLEL_PSO;
%   algo = PPSO_PNO;
  
  % algo = DEBUG_ADC;
  % algo = PARALLEL_PSO_MULTI_SWARM;
  % algo = MULTI_UNIT;
  % algo = EXTREMUM_SEEKING;
end

% units = uint8(3:1:10);
% units = uint8([3:6 11:14]);
units = uint8(0:1:14);
% units = uint8(11:1:14);
% units = uint8([0, 2, 4, 7:14]);
% units = uint8([0:1:4 7:1:14]);
unitsMem = units;
nUnits = uint8(length(units));
% lengthOfPayload = fliplr(typecast(uint16(3 + nUnits), 'uint8'));
lengthOfPayload = typecast(uint16(3 + nUnits), 'uint8');

buf = [delimiter, typeOfMsg, lengthOfPayload, startAlgoChar, algo, nUnits, units];
fwrite(port, buf);

nIterations = 900;
if algo == CHARACTERIZATION
  oSendDebugData = uint8(0);
  nIterations = 256;
%   nIterations = 60;
elseif algo == CLASSIC_PSO
  oSendDebugData = uint8(0);
  algoStr = 'de l''OEPS';
elseif algo == PARALLEL_PSO
  oSendDebugData = uint8(0);
  algoStr = 'de l''OEPP';
elseif algo == PARALLEL_PSO_MULTI_SWARM
  oSendDebugData = uint8(0);
  nIterations = 130;
elseif algo == PPSO_PNO
  oSendDebugData = uint8(1);
  algoStr = 'de l''OEPPC';
elseif algo == PPSOCD
  oSendDebugData = uint8(1);
  algoStr = 'de l''OEPPCD';
elseif algo == PNO
  oSendDebugData = uint8(0);
  algoStr = 'du P&O';
elseif algo == DEBUG_ADC
  oSendDebugData = uint8(1);
  nIterations = 25;
else
  oSendDebugData = uint8(0);
  nIterations = 20;
end


% Send debug data or not
delimiter = PROTOCOL_DELIMITER;
typeOfMsg = SET_DEBUG_DATA;
lengthOfPayload = typecast(uint16(1), 'uint8');
oSendDebugData = typecast(oSendDebugData, 'uint8');
buf = [delimiter, typeOfMsg, lengthOfPayload, oSendDebugData];
fwrite(port, buf);

%% Algo run

% tsMem  = [];
% posMem = [];
% powMem = [];
tsMem  = zeros(1, nIterations);
posMem = zeros(1, nIterations * double(nUnits));
powMem = zeros(1, nIterations * double(nUnits));

sIteration = [];
pSpeedMem = [];
pPosMem = [];
pPowMem = [];

adcMem = [];

tic
wbh = waitbar(0, ['Iteration : ' num2str(0) '/' num2str(nIterations)]);  % Waitbar handle

iIteration = 0;
idx = 1;
while iIteration < nIterations
  waitbar(iIteration/nIterations, wbh, ['Iteration: ' num2str(iIteration) '/' num2str(nIterations)])
  buf = fread(port, SIZE_OF_PROTOCOL_HEADER);
  delimiter = buf(1);
  typeOfMsg = buf(2);
  lengthOfPayload = double(typecast(uint8(buf(3:4)'), 'uint16'));
  
  %payload
  buf = fread(port, lengthOfPayload, 'uint8');
  if typeOfMsg == UNITS_DATA
    iIteration = iIteration + 1;
    timestamp = typecast(typecast(uint8(buf(1:4)'), 'uint32'), 'single');
    nUnits = buf(5);
    nData = buf(6);
    data = typecast(typecast(uint8(buf(7:end)'), 'uint32'), 'single');
    positions = data(1:end/2);
    powers = data(end/2+1:end);
  
%     tsMem  = [tsMem timestamp];
%     posMem = [posMem positions];
%     powMem = [powMem powers];
    tsMem(iIteration) = timestamp;
    posMem((iIteration-1)*double(nUnits)+1 : (iIteration)*double(nUnits)) = positions;
    powMem((iIteration-1)*double(nUnits)+1 : (iIteration)*double(nUnits)) = powers;
  elseif typeOfMsg == PSO_DATA
    swarmIteration = double(typecast(uint8(buf(1:2)'), 'uint16'));
    nParticles = buf(3);
    nData = buf(4);
    data = typecast(typecast(uint8(buf(5:end)'), 'uint32'), 'single');
    
    particlesSpeed = data(1:end/3);
    particlesPos = data(end/3+1 : 2*end/3);
    particlesPow = data(2*end/3 + 1 : end);
    sIteration = [sIteration; swarmIteration];
    pSpeedMem = [pSpeedMem; particlesSpeed];
    pPosMem = [pPosMem; particlesPos];
    pPowMem = [pPowMem; particlesPow];
    
  elseif typeOfMsg == ADC_DATA
    oNewPacket = buf(1);
    nUnits = buf(2);
    nData = typecast(uint8(buf(3:4)'), 'uint16');
    adcPacketLength = nData;
    data = typecast(uint8(buf(5:end)'), 'uint16');
    adcMem = [adcMem data];
    while(oNewPacket == uint8(1))
      buf = fread(port, SIZE_OF_PROTOCOL_HEADER);
      delimiter = buf(1);
      typeOfMsg = buf(2);
      lengthOfPayload = double(typecast(uint8(buf(3:4)'), 'uint16'));
  
      %payload
      buf = fread(port, lengthOfPayload, 'uint8');
      oNewPacket = buf(1);
      nUnits = buf(2);
      nData = typecast(uint8(buf(3:4)'), 'uint16');
      adcPacketLength = nData;
      data = typecast(uint8(buf(5:end)'), 'uint16');
      adcMem = [adcMem data];
    end
      
  elseif typeOfMsg == PPSO_PNO_DATA
    algoIteration = double(typecast(uint8(buf(1:2)'), 'uint16'));
    nGroups = buf(3);
    groups = buf(4:3+N_UNITS_TOTAL);
    groupLengths = buf(N_UNITS_TOTAL+4 : 3 + N_UNITS_TOTAL + N_UNITS_TOTAL);
    
    if nGroups > 0
      groups = groups';
      groupOffset = 0;
      fprintf(['Sim iteration = ' num2str(iIteration) '\n']);
      fprintf(['Algo iteration = ' num2str(algoIteration) '\n']);
      for iGroup = 1 : nGroups
        fprintf(['Group ' num2str(iGroup) ' = ' num2str(groups(1 + groupOffset : groupOffset + groupLengths(iGroup))) '\n'])
        groupOffset = groupOffset + groupLengths(iGroup);
      end
      fprintf('\n')
    end
  end
end
toc
close(wbh)

% Stop algo
typeOfMsg = STOP_ACQ;
% lengthOfPayload = fliplr(typecast(uint16(1), 'uint8'));
lengthOfPayload = typecast(uint16(1), 'uint8');
stopAlgoChar = PROTOCOL_STOP_ALGO;
buf = [delimiter, typeOfMsg, lengthOfPayload, stopAlgoChar];
fwrite(port, buf);

% Remove contents of input buffer
flushinput(port);

% Disconnect from instrument object.
fclose(port);

% Clean up all objects.
delete(port);

%% Figures

if algo == CHARACTERIZATION
  optPos = zeros(1, nUnits);
  optPow = zeros(1, nUnits);
  fig = figure;
  set(gcf, 'Color', 'w')
  legendStr = {};
  subplot(2,1,1)
  plot(posMem(1:nUnits:end));
  subplot(2,1,2)
  hold on
  for i = 1 : nUnits
    set(gcf, 'Position', get(0,'Screensize'));
    lengthOfData = length(posMem) / nData;
    plot(powMem(i:nUnits:end));
    legendStr{i} = ['Unit ' num2str(unitsMem(i))];
    
    tmpPow = powMem(i:nUnits:end);
    tmpPos = posMem(i:nUnits:end);
    
    [maxPow, maxIdx] = max(tmpPow);
    optPos(i) = tmpPos(maxIdx);
    optPow(i) = tmpPow(maxIdx);
  end
  subplot(2,1,1)
  title('External load (\Omega)')
  subplot(2,1,2)
  title('Power output (W)')
  legend(legendStr)
%   optPos
%   optPow
  
  fig = figure;
  set(gcf, 'Color', 'w')
  set(gcf, 'Position', get(0,'Screensize'));
  legendStr = {};
  hold on
  for i = 1 : nUnits
    lengthOfData = length(posMem) / nData;
    plot(posMem(i:nUnits:end), powMem(i:nUnits:end));
    legendStr{i} = ['PV cell ' num2str(unitsMem(i)) ' -> R_o_p_t = ' num2str(optPos(i), '%.2f') ' \Omega, P_o_p_t = ' num2str(optPow(i)*1000, '%.2f') ' mW'];
  end
  title(['Static power curves of the PV cells with a PWM of ' num2str(initLedIntensity/10, '%.1f') '%'])
  xlabel('Load (\Omega)')
  ylabel('Power output (W)')
  legend(legendStr)
  set(gca, 'FontSize', 16)
else
%   curUnit = 1;
%   unitsLeft = nUnits;
%   while unitsLeft >0
%     nFigs = min(unitsLeft, 4);
%     units = curUnit:1:nFigs+curUnit-1;
%     fig = figure;
%     set(gcf, 'Position', get(0,'Screensize'));
%     lengthOfData = length(posMem) / nData;
%     for i = 1 : nFigs
%       subplot(2,nFigs,i)
%       plot(posMem(i+curUnit-1:nUnits:end));
%       titleStr = ['Load (\Omega) - Unit ' num2str(unitsMem(curUnit+i-1))]; 
%       title(titleStr);
%       axes = axis;
%       axes(3) = 0;
%       axis(axes);
%       subplot(2,nFigs,i+nFigs)
%       plot(powMem(i+curUnit-1:nUnits:end));
%       titleStr = ['Power (W) - Unit ' num2str(unitsMem(curUnit+i-1))]; 
%       title(titleStr);
%       axes = axis;
%       axes(3) = 0;
%       axis(axes);
%     end
%     curUnit = curUnit + nFigs;
%     unitsLeft = unitsLeft - nFigs;
%   end
  
  figure
  set(gcf, 'Color', 'w')
  subplot(2,1,1)
  hold on
  set(gcf, 'Position', get(0,'Screensize'));
  legendStr = cell(1,nUnits);
  for i = 1 : nUnits
    plot(tsMem, posMem(i:nUnits:end));
    legendStr{i} = ['PV cell ' num2str(unitsMem(i))];
  end
  legend(legendStr, 'FontSize', 12)
  title(['Évolution des R_e_x_t ' algoStr])
  ylabel('R_e_x_t (\Omega)')
  xlabel('Temps (s)')
  set(gca, 'FontSize', 16)
  
  subplot(2,1,2)
  hold on
  for i = 1 : nUnits
    plot(tsMem, powMem(i:nUnits:end));
  end
  legend(legendStr, 'FontSize', 12)
  title(['Évolution des P_o_u_t ' algoStr])
  ylabel('P_o_u_t (W)')
  xlabel('Temps (s)')
  set(gca, 'FontSize', 16)
  
end

if ~isempty(adcMem)
  figure
  plot(adcMem)
end


%% Analyze convergence time, precision, and tracking efficiency of algorithm
%--------------------------------------------------------------------------
fprintf('\n\t\t\t\t\t\t\t***********************************************\n')
if algo == CLASSIC_PSO
  fprintf('\t\t\t\t\t\t\t*********** SEQUENTIAL PSO ANALYSIS ***********\n')
elseif algo == PARALLEL_PSO
  fprintf('\t\t\t\t\t\t\t*********** PARALLEL PSO ANALYSIS *************\n')
elseif algo == PPSO_PNO
  fprintf('\t\t\t\t\t\t\t************** PSO-P&O ANALYSIS ***************\n')
elseif algo == PPSOCD
  fprintf('\t\t\t\t\t\t\t************** OEPPCD ANALYSIS  ***************\n')
elseif algo == PNO
  fprintf('\t\t\t\t\t\t\t**************** P&O ANALYSIS *****************\n')
else
  error('Must define a type of algorithm!');
end
fprintf('\t\t\t\t\t\t\t***********************************************\n')

if oDoPerturb
  len = perturbIterations(1);
else
  len = nIterations;
end
dBefore = zeros(len, nUnits);
jBefore = zeros(len, nUnits);
optPosBefore = zeros(1, nUnits);
optPowerBefore = zeros(1, nUnits);
for iUnit = 1 : nUnits
  tmpPos = posMem(iUnit:nUnits:end);
  tmpPow = powMem(iUnit:nUnits:end);
  dBefore (:, iUnit) = tmpPos(1:len);
  jBefore (:, iUnit) = tmpPow(1:len);
  [optPowerBefore(iUnit), idx] = max(jBefore(:,iUnit));
  optPosBefore(iUnit) = dBefore(idx, iUnit);
end
steadyStateBefore = zeros(1, nUnits);
convergenceBefore = zeros(1, nUnits);
meanPowerBefore = zeros(1, nUnits);
ssOscBefore = zeros(1, nUnits);
precisionBefore = zeros(1, nUnits);
efficiencyBefore = zeros(1, nUnits);
joulesBefore = zeros(1, nUnits);
tBefore = (1:len) * tsMem(2);

oscAmp = 0.01;
for iUnit = 1 : nUnits
  for iIteration = 1 : len
    clear meanJ
    meanJ = mean(jBefore(iIteration:end, iUnit));
    if (max(jBefore(iIteration:end, iUnit)) - meanJ) / meanJ < oscAmp && (meanJ - min(jBefore(iIteration:end, iUnit)))/meanJ < oscAmp
      break;
    end
  end
  steadyStateBefore(iUnit) = iIteration;
  precisionBefore(iUnit) = 100 - abs((mean(dBefore(iIteration:end, iUnit) - optPosBefore(iUnit)))) / optPosBefore(iUnit) * 100;
  efficiencyBefore(iUnit) = 100 - abs(mean(jBefore(:,iUnit)) - optPowerBefore(iUnit)) / optPowerBefore(iUnit) * 100;
  joulesBefore(iUnit) = trapz(tBefore, jBefore(:,iUnit));

  for iIteration = 1 : len
    if length(dBefore(iIteration:end, iUnit)) < 3
      break;
    end
    [maxPeaks maxIdx] = findpeaks(dBefore(iIteration:end, iUnit));
    dInv = 1.01*max(dBefore(iIteration:end, iUnit)) - dBefore(iIteration:end, iUnit);
    [minPeaks minIdx] = findpeaks(dInv);
    dTemp = dBefore(iIteration:end, iUnit);
    minPeaks = dTemp(minIdx);
    if ~isempty(maxPeaks) && ~isempty(minIdx)
      if (max(maxPeaks) - mean(maxPeaks)) / mean(maxPeaks) < oscAmp/2 && (mean(minPeaks) -  min(minPeaks)) / mean(minPeaks) < oscAmp
        if dTemp(1) ~= max(dTemp) && dTemp(1) ~= min(dTemp)
          break;
        end
      end
    else
      if isempty(maxPeaks) && isempty(minIdx)
        break;
      elseif isempty(maxPeaks)
        if (mean(minPeaks) -  min(minPeaks)) / mean(minPeaks) < oscAmp/2
          if dTemp(1) ~= max(dTemp) && dTemp(1) ~= min(dTemp)
            break;
          end
        end
      else % isempty(minIdx)
        if (max(maxPeaks) - mean(maxPeaks)) / mean(maxPeaks) < oscAmp/2
          if dTemp(1) ~= max(dTemp) && dTemp(1) ~= min(dTemp)
            break;
          end
        end
      end
    end
  end
  convergenceBefore(iUnit) = iIteration;
  meanPowerBefore(iUnit) = mean(jBefore(iIteration:end, iUnit));
  ssOscBefore(iUnit) = max(abs(jBefore(iIteration:end, iUnit) - meanPowerBefore(iUnit))) * 100 / meanPowerBefore(iUnit);
  totalPowerBefore = sum(jBefore(:,:), 2);
  maxPowerBefore = max(totalPowerBefore);
  totalEfficiencyBefore = mean(totalPowerBefore)/maxPowerBefore * 100;
  
  if exist('oDoingLoops', 'var')
    convTime (iLoop, iUnit, 1) = convergenceBefore(iUnit);
  end
end

if exist('oDoingLoops', 'var')
  joulesMem(iLoop, 1) = sum(joulesBefore(:));
  powersMem(iLoop, 1) = sum(meanPowerBefore(:));
  efficiencyMem(iLoop, 1) = totalEfficiencyBefore;
  precisionMem(iLoop, 1) = mean(precisionBefore(:));
end
  
unitsS0 = zeros(1,nUnits);    % Change this

fprintf('\n==========================================================================================================\n')
fprintf('\t\t\t\t\t\t\t\t********  Before perturbation  ********\n')
fprintf('==========================================================================================================\n')
%   fprintf(['Unit\tS0\t\t\t\tPrecision\t\tConvergence\t\tEfficiency\tJoules\n']);
fprintf(['Unit\tIrradiance\tPrecision\tSS @ ±' num2str(uint16(oscAmp*100)) '%%\tEfficiency\tJoules\t\tConvergence time\tConvergence power [mW]\n']);
for iUnit = 1 : nUnits
  fprintf([num2str(uint16(iUnit)) '\t\t\t'])
  if (initLedIntensity < 100)
    fprintf(' ')
  end
  fprintf([num2str(initLedIntensity/10, '%.1f') '\t'])
  fprintf([num2str(precisionBefore(iUnit), '%.2f') '\t\t'])
  fprintf([num2str(uint16(steadyStateBefore(iUnit))) '\t\t\t'])
  fprintf([num2str(efficiencyBefore(iUnit), '%.2f') '\t\t'])
  fprintf([num2str(joulesBefore(iUnit), '%.3f') '\t\t'])
  fprintf([num2str(uint16(convergenceBefore(iUnit))) '\t\t\t\t\t'])
  fprintf([num2str(meanPowerBefore(iUnit)*1000, '%.4f') ' ±' num2str(ssOscBefore(iUnit), '%.2f') '%%'])
  fprintf('\n')
end
fprintf('----------------------------------------------------------------------------------------------------------\n')
fprintf('Total\t\t\t\t')
fprintf([num2str(mean(precisionBefore(:)), '%.2f') '\t\t\t\t\t'])
%   fprintf([num2str(mean(efficiencyBefore(:)), '%.2f') '\t\t'])
fprintf([num2str(totalEfficiencyBefore, '%.2f') '\t\t'])
fprintf([num2str(sum(joulesBefore(:)), '%.3f') '\t\t\t\t\t\t\t'])
fprintf([num2str(sum(meanPowerBefore(:).*1000), '%.3f') '\t\t'])

% ---------------------------------

for iPerturb = 1 : nPerturbs
  
  if iPerturb < nPerturbs
    finalIteration = perturbIterations(iPerturb + 1);
  else
    finalIteration = nIterations;
  end
  steadyStateAfter = zeros(1, nUnits);
  convergenceAfter = zeros(1, nUnits);
  meanPowerAfter = zeros(1, nUnits);
  ssOscAfter = zeros(1, nUnits);
  precisionAfter = zeros(1, nUnits);
  efficiencyAfter = zeros(1, nUnits);
  joulesAfter = zeros(1, nUnits);
  tAfter  = (1:finalIteration - perturbIterations(iPerturb)) * tsMem(2);


  dAfter = zeros(finalIteration - perturbIterations(iPerturb), nUnits);
  jAfter = zeros(finalIteration - perturbIterations(iPerturb), nUnits);
  optPosAfter = zeros(1, nUnits);
  optPowerAfter = zeros(1, nUnits);
  for iUnit = 1 : nUnits
    tmpPos = posMem(iUnit:nUnits:end);
    tmpPow = powMem(iUnit:nUnits:end);
    dAfter  (:, iUnit) = tmpPos(perturbIterations(iPerturb)+1: finalIteration);
    jAfter  (:, iUnit) = tmpPow(perturbIterations(iPerturb)+1: finalIteration);
%     dAfter  (:, iUnit) = posMem(perturbIterations(iPerturb)+1, finalIteration);
%     jAfter  (:, iUnit) = powMem(perturbIterations(iPerturb)+1, finalIteration);
    [optPowerAfter(iUnit), idx] = max(jAfter(:,iUnit));
    optPosAfter(iUnit) = dAfter(idx, iUnit);
  end
  
  oscAmp = 0.05;
  for iUnit = 1 : nUnits
    for iIteration = 1 : finalIteration - perturbIterations(iPerturb)
      clear meanJ
      meanJ = mean(jAfter(iIteration:end, iUnit));
      if (max(jAfter(iIteration:end, iUnit)) - meanJ) / meanJ < oscAmp && (meanJ - min(jAfter(iIteration:end, iUnit)))/meanJ < oscAmp
        break;
      end
    end
    steadyStateAfter(iUnit) = iIteration;
    precisionAfter(iUnit) = 100 - abs((mean(dAfter(iIteration:end, iUnit) - optPosAfter(iUnit)))) / optPosAfter(iUnit) * 100;
    efficiencyAfter(iUnit) = 100 - abs(mean(jAfter(:,iUnit)) - optPowerAfter(iUnit)) / optPowerAfter(iUnit) * 100;
    joulesAfter(iUnit) = trapz(tAfter, jAfter(:,iUnit));

    for iIteration = 1 : perturbIterations(iPerturb)
      if length(dAfter(iIteration:end, iUnit)) < 3
        break;
      end
      [maxPeaks maxIdx] = findpeaks(dAfter(iIteration:end, iUnit));
      dInv = 1.01*max(dAfter(iIteration:end, iUnit)) - dAfter(iIteration:end, iUnit);
      [minPeaks minIdx] = findpeaks(dInv);
      dTemp = dAfter(iIteration:end, iUnit);
      minPeaks = dTemp(minIdx);
      if ~isempty(maxPeaks) && ~isempty(minIdx)
        if (max(maxPeaks) - mean(maxPeaks)) / mean(maxPeaks) < oscAmp/2 && (mean(minPeaks) -  min(minPeaks)) / mean(minPeaks) < oscAmp
          if dTemp(1) ~= max(dTemp) && dTemp(1) ~= min(dTemp)
            break;
          end
        end
      else
        if isempty(maxPeaks) && isempty(minIdx)
          break;
        elseif isempty(maxPeaks)
          if (mean(minPeaks) -  min(minPeaks)) / mean(minPeaks) < oscAmp/2
            if dTemp(1) ~= max(dTemp) && dTemp(1) ~= min(dTemp)
              break;
            end
          end
        else % isempty(minIdx)
          if (max(maxPeaks) - mean(maxPeaks)) / mean(maxPeaks) < oscAmp/2
            if dTemp(1) ~= max(dTemp) && dTemp(1) ~= min(dTemp)
              break;
            end
          end
        end
      end
    end
    convergenceAfter(iUnit) = iIteration;
    meanPowerAfter(iUnit) = mean(jAfter(iIteration:end, iUnit));
    ssOscAfter(iUnit) = max(abs(jAfter(iIteration:end, iUnit) - meanPowerAfter(iUnit))) * 100 / meanPowerAfter(iUnit);
    totalPowerAfter = sum(jAfter(:,:), 2);
    maxPowerAfter = max(totalPowerAfter);
    totalEfficiencyAfter = mean(totalPowerAfter)/maxPowerAfter * 100;
    
    if exist('oDoingLoops', 'var')
      convTime (iLoop, iUnit, iPerturb + 1) = convergenceAfter(iUnit);
    end
  end
  if exist('oDoingLoops', 'var')
    joulesMem(iLoop, iPerturb + 1) = sum(joulesAfter(:));
    powersMem(iLoop, iPerturb + 1) = sum(meanPowerAfter(:));
    efficiencyMem(iLoop, iPerturb + 1) = totalEfficiencyAfter;
  precisionMem(iLoop, iPerturb + 1) = mean(precisionAfter(:));
  end

  fprintf('\n==========================================================================================================\n')
  fprintf(['\t\t\t\t\t\t\t\t********  After perturbation ' num2str(uint16(iPerturb)) ' ********\n'])
  fprintf('==========================================================================================================\n')
  %   fprintf(['Unit\tS0\t\t\t\tPrecision\t\tConvergence\t\tEfficiency\tJoules\n']);
  fprintf(['Unit\tS0\t\t\tPrecision\tSS @ ±' num2str(uint16(oscAmp*100)) '%%\tEfficiency\tJoules\t\tConvergence time\tConvergence power [mW]\n']);
  for iUnit = 1 : nUnits
    fprintf([num2str(uint16(iUnit)) '\t\t'])
    if (unitsS0(iUnit) + perturbAmps(iPerturb)) < 100
      fprintf(' ')
    end
    fprintf([num2str(unitsS0(iUnit) + perturbAmps(iPerturb), '%.4f') '\t'])
    fprintf([num2str(precisionAfter(iUnit), '%.2f') '\t\t'])
    fprintf([num2str(uint16(steadyStateAfter(iUnit))) '\t\t\t'])
    fprintf([num2str(efficiencyAfter(iUnit), '%.2f') '\t\t'])
    if joulesAfter(iUnit) < 1000
      fprintf(' ')
    end
    fprintf([num2str(joulesAfter(iUnit), '%.3f') '\t'])
    fprintf([num2str(uint16(convergenceAfter(iUnit))) '\t\t\t\t\t'])
    fprintf([num2str(meanPowerAfter(iUnit)*1000, '%.4f') ' ±' num2str(ssOscAfter(iUnit), '%.2f') '%%'])
    fprintf('\n')
  end
  fprintf('----------------------------------------------------------------------------------------------------------\n')
  fprintf('Total\t\t\t\t')
  fprintf([num2str(mean(precisionAfter(:)), '%.2f') '\t\t\t\t\t'])
  %   fprintf([num2str(mean(efficiencyAfter(:)), '%.2f') '\t\t'])
  fprintf([num2str(totalEfficiencyAfter, '%.2f') '\t\t'])
  fprintf([num2str(sum(joulesAfter(:)), '%.3f') '\t\t\t\t\t\t\t'])
  fprintf([num2str(sum(meanPowerAfter(:).*1000), '%.3f') '\t\t'])

end

fprintf('\n');
