clear
close all

% Next 2 lines are to close any open waitbar
f = findall(0,'tag','TMWWaitbar');
delete(f);

%% Constants

% PROTOCOL_DELIMITER = uint8(hex2num('7E'));
PROTOCOL_DELIMITER = uint8(126);

NEW_RNG_SEED = uint8(0);
START_ACQ = uint8(1);
STOP_ACQ = uint8(2);
UNITS_DATA = uint8(3);
PSO_DATA = uint8(4);

CLASSIC_PSO = uint8(0);
PARALLEL_PSO = uint8(1);
PARALLEL_PSO_MULTI_SWARM = uint8(2);
MULTI_UNIT = uint8(3);
EXTREMUM_SEEKING = uint8(4);
CHARACTERIZATION = uint8(5);
PPSO_PNO = uint8(6);
PNO = uint8(7);
DEBUG_ADC = uint8(8);

PROTOCOL_START_ALGO = uint8('!' - 0);

PROTOCOL_STOP_ALGO = uint8('x' - 0);

SIZE_OF_PROTOCOL_HEADER = 4;

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

% port.BaudRate = 115200;
port.BaudRate = 9600;
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
% lengthOfPayload = fliplr(typecast(uint16(16), 'uint8'));
lengthOfPayload = typecast(uint16(16), 'uint8');
[seed1, seed2] = GenerateNewSeeds;
% seed1 = uint64(6634093205851280027);
% seed2 = uint64(9627091455319487185);
seeds = typecast([seed1, seed2], 'uint8');

buf = [delimiter, typeOfMsg, lengthOfPayload, seeds];
fwrite(port, buf);

% Start algo
typeOfMsg = START_ACQ;
startAlgoChar = PROTOCOL_START_ALGO;
% algo = CHARACTERIZATION;
% algo = CLASSIC_PSO;
% algo = PARALLEL_PSO;
% algo = PARALLEL_PSO_MULTI_SWARM;
% algo = MULTI_UNIT;
% algo = EXTREMUM_SEEKING;
% algo = PPSO_PNO;
algo = PNO;
% algo = DEBUG_ADC;
% units = uint8(3:1:10);
% units = uint8([3:6 11:14]);
units = uint8(0:2);
nUnits = uint8(length(units));
% lengthOfPayload = fliplr(typecast(uint16(3 + nUnits), 'uint8'));
lengthOfPayload = typecast(uint16(3 + nUnits), 'uint8');

buf = [delimiter, typeOfMsg, lengthOfPayload, startAlgoChar, algo, nUnits, units];
fwrite(port, buf);

if algo == CHARACTERIZATION
  nIterations = 256;
%   nIterations = 10;
elseif algo == CLASSIC_PSO
  nIterations = 140;
elseif algo == PARALLEL_PSO
  nIterations = 60;
elseif algo == PARALLEL_PSO_MULTI_SWARM
  nIterations = 130;
elseif algo == PPSO_PNO
  nIterations = 130;
elseif algo == PNO
  nIterations = 30;
elseif algo == DEBUG_ADC
  nIterations = 100;
else
  nIterations = 20;
end

%% Figures init

nSolarCells = double(nUnits);

% Wait for the user to stop the program
% stopBtn = 0;
% f = figure(i+1);
% b = uicontrol('style','push','string','Stop','callback','stopBtn=stopBtn+1');

tsMem  = [];
posMem = [];
powMem = [];

sIteration = [];
pSpeedMem = [];
pPosMem = [];
pPowMem = [];

tic
wbh = waitbar(0, ['Iteration : ' num2str(0) '/' num2str(nIterations)]);  % Waitbar handle
for iIteration = 1 : nIterations
  % header
%   buf = fread(port, SIZE_OF_PROTOCOL_HEADER, 'uint8');
  buf = fread(port, SIZE_OF_PROTOCOL_HEADER);
  delimiter = buf(1);
  typeOfMsg = buf(2);
  lengthOfPayload = double(typecast(uint8(buf(3:4)'), 'uint16'));
  
  %payload
  buf = fread(port, lengthOfPayload, 'uint8');
  if typeOfMsg == UNITS_DATA
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
  end
    waitbar(iIteration/nIterations, wbh, ['Iteration: ' num2str(iIteration) '/' num2str(nIterations)])
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

% Disconnect from instrument object, obj1.
fclose(port);

% Clean up all objects.
delete(port);

%% Figures

% close all

% tmpFig = figure;
% set(gcf, 'Position', get(0,'Screensize'));
% maxPos = tmpFig.Position;
% close(tmpFig);
% maxHeigth = maxPos(4);
% maxWidth  = maxPos(3);
% figHeigth = maxHeigth;
% figWidth  = floor(maxWidth /nUnits);

% fig(1) = figure(1);
% fig(1).Position = [1 1 figWidth figHeigth];
% for i = 2 : nUnits
%   fig(i) = figure(i);
%   figPos = fig(i-1).Position;
%   fig(i).Position = [figPos(1)+figWidth, 1, figWidth, figHeigth];
% end

fig = figure;
set(gcf, 'Position', get(0,'Screensize'));
lengthOfData = length(posMem) / nData;
for i = 1 : nUnits
%   fig(i) = figure(i);
  subplot(2,nUnits,i)
  plot(tsMem, posMem(i:nUnits:end));
  subplot(2,nUnits,i+nUnits)
  plot(tsMem, powMem(i:nUnits:end));
end