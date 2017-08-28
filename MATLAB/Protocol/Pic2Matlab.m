clear
close all

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
UNITS_DATA                = uint8(5);
PSO_DATA                  = uint8(6);
ADC_DATA                  = uint8(7);
PPSO_PNO_DATA             = uint8(8);

CLASSIC_PSO               = uint8(0);
PARALLEL_PSO              = uint8(1);
PARALLEL_PSO_MULTI_SWARM  = uint8(2);
MULTI_UNIT                = uint8(3);
EXTREMUM_SEEKING          = uint8(4);
CHARACTERIZATION          = uint8(5);
PPSO_PNO                  = uint8(6);
PNO                       = uint8(7);
DEBUG_ADC                 = uint8(8);

PROTOCOL_START_ALGO       = uint8('!' - 0);
PROTOCOL_STOP_ALGO        = uint8('x' - 0);

SIZE_OF_PROTOCOL_HEADER   = 4;

N_UNITS_TOTAL             = 15;

%% Port setup

% Wait for the user to start the program
reply = 'N';
while reply == 'N'
  reply = upper(input('Start? Y/N [N]:','s'));
  if isempty(reply)
    reply = 'Y'
  end
end

% Create a serial port object.
port = instrfind('Type', 'serial', 'Port', 'COM3', 'Tag', '');

if isempty(port)
%     port = serial('COM5');
    port = serial('COM3');
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
% port.BaudRate = 9600;
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
% [seed1, seed2] = GenerateNewSeeds;
seed1 = uint64(14373098484861655346);
seed2 = uint64(2396346581266420478);
seeds = typecast([seed1, seed2], 'uint8');

buf = [delimiter, typeOfMsg, lengthOfPayload, seeds];
fwrite(port, buf);

% Perturb
nPerturbs = 1;

if nPerturbs > 0
  perturbAmps = zeros(1, nPerturbs);
  perturbUnits = cell(1, nPerturbs);
  perturbIterations = zeros(1, nPerturbs);
  perturbAmps(1) = -100;
%   perturbUnits{1} = 0:1:3;
  perturbUnits{1} = [0:1:4 7:1:14];
%   perturbUnits{1} = [0:1:14];
  perturbIterations(1) = 50;
end

if nPerturbs >= 2
  perturbUnits{2} = [4 7 8 10];
  perturbAmps(2) = 100;
  perturbIterations(2) = 40;
end

perturbPayloadBaseLength = 4 + 2 + 1;
delimiter = PROTOCOL_DELIMITER;
typeOfMsg = SET_PERTURB;
for i = 1 : nPerturbs
  iteration = typecast(uint32(perturbIterations(i)), 'uint8');
  amplitude = typecast(int16(perturbAmps(i)), 'uint8');
  nUnits = uint8(length(perturbUnits{i}));
  units = uint8(perturbUnits{i});
  
  lengthOfPayload = typecast(uint16(perturbPayloadBaseLength + length(perturbUnits{i})), 'uint8');
  buf = [delimiter, typeOfMsg, lengthOfPayload, iteration, amplitude, nUnits, units];
  fwrite(port, buf)
end

% Start algo
typeOfMsg = START_ACQ;
startAlgoChar = PROTOCOL_START_ALGO;
% algo = CHARACTERIZATION;
% algo = CLASSIC_PSO;
% algo = PARALLEL_PSO;
% algo = PARALLEL_PSO_MULTI_SWARM;
% algo = MULTI_UNIT;
% algo = EXTREMUM_SEEKING;
algo = PPSO_PNO;
% algo = PNO;
% algo = DEBUG_ADC;
% units = uint8(3:1:10);
% units = uint8([3:6 11:14]);
% units = uint8(0:1:14);
units = uint8([0:1:4 7:1:14]);
unitsMem = units;
nUnits = uint8(length(units));
% lengthOfPayload = fliplr(typecast(uint16(3 + nUnits), 'uint8'));
lengthOfPayload = typecast(uint16(3 + nUnits), 'uint8');

buf = [delimiter, typeOfMsg, lengthOfPayload, startAlgoChar, algo, nUnits, units];
fwrite(port, buf);

if algo == CHARACTERIZATION
  oSendDebugData = uint8(0);
  nIterations = 256;
%   nIterations = 60;
elseif algo == CLASSIC_PSO
  oSendDebugData = uint8(0);
  nIterations = 140;
elseif algo == PARALLEL_PSO
  oSendDebugData = uint8(0);
  nIterations = 60;
elseif algo == PARALLEL_PSO_MULTI_SWARM
  oSendDebugData = uint8(0);
  nIterations = 130;
elseif algo == PPSO_PNO
  oSendDebugData = uint8(1);
  nIterations = 130;
%   nIterations = 60;
elseif algo == PNO
  oSendDebugData = uint8(0);
  nIterations = 40;
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

nSolarCells = double(nUnits);

tsMem  = [];
posMem = [];
powMem = [];

sIteration = [];
pSpeedMem = [];
pPosMem = [];
pPowMem = [];

adcMem = [];

tic
wbh = waitbar(0, ['Iteration : ' num2str(0) '/' num2str(nIterations)]);  % Waitbar handle

iIteration = 0;
while iIteration <= nIterations
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
  
    tsMem  = [tsMem timestamp];
    posMem = [posMem positions];
    powMem = [powMem powers];
    
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
  fig = figure;
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
  end
  subplot(2,1,1)
  title('External load [\Omega]')
  subplot(2,1,2)
  title('Power output [W]')
  legend(legendStr)
    
else
  curUnit = 1;
  unitsLeft = nUnits;
  while unitsLeft >0
    nFigs = min(unitsLeft, 4);
    units = curUnit:1:nFigs+curUnit-1;
    fig = figure;
    set(gcf, 'Position', get(0,'Screensize'));
    lengthOfData = length(posMem) / nData;
    for i = 1 : nFigs
      subplot(2,nFigs,i)
      plot(posMem(i+curUnit-1:nUnits:end));
      titleStr = ['Load [\Omega] - Unit ' num2str(unitsMem(curUnit+i-1))]; 
      title(titleStr);
      axes = axis;
      axes(3) = 0;
      axis(axes);
      subplot(2,nFigs,i+nFigs)
      plot(powMem(i+curUnit-1:nUnits:end));
      titleStr = ['Power [W] - Unit ' num2str(unitsMem(curUnit+i-1))]; 
      title(titleStr);
      axes = axis;
      axes(3) = 0;
      axis(axes);
    end
    curUnit = curUnit + nFigs;
    unitsLeft = unitsLeft - nFigs;
  end
end

if ~isempty(adcMem)
  figure
  plot(adcMem)
end