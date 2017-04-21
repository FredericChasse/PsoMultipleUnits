% clear
% close all

%% Port setup

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
% seed1 = uint64(7827858136972333423);
% seed2 = uint64(14206939411198680430);
seeds = typecast([seed1, seed2], 'uint8');

buf = [delimiter, typeOfMsg, lengthOfPayload, seeds];
fwrite(port, buf);

% Start algo
typeOfMsg = START_ACQ;
startAlgoChar = PROTOCOL_START_ALGO;
% lengthOfPayload = fliplr(typecast(uint16(3 + nUnits), 'uint8'));
lengthOfPayload = typecast(uint16(3 + nUnits), 'uint8');

buf = [delimiter, typeOfMsg, lengthOfPayload, startAlgoChar, algo, nUnits, units];
fwrite(port, buf);

if algo == CHARACTERIZATION
  nIterations = 256;
elseif algo == CLASSIC_PSO
  nIterations = 140;
elseif algo == PARALLEL_PSO
  nIterations = 60;
elseif algo == PARALLEL_PSO_MULTI_SWARM
  nIterations = 130;
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
  
    tsMem  = [tsMem; timestamp];
    posMem = [posMem; positions];
    powMem = [powMem; powers];
    
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
end
toc

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


% fig = figure;
% set(gcf, 'Position', get(0,'Screensize'));
% lengthOfData = length(posMem) / nData;
% for i = 1 : nUnits
% %   fig(i) = figure(i);
%   subplot(2,nUnits,i)
%   plot(tsMem, posMem(i:nUnits:end));
%   subplot(2,nUnits,i+nUnits)
%   plot(tsMem, powMem(i:nUnits:end));
% end
