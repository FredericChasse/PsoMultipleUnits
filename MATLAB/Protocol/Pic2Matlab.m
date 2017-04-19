clear
% close all

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
% [seed1, seed2] = GenerateNewSeeds;
seed1 = uint64(7827858136972333423);
seed2 = uint64(14206939411198680430);
seeds = typecast([seed1, seed2], 'uint8');

buf = [delimiter, typeOfMsg, lengthOfPayload, seeds];
fwrite(port, buf);

% Start algo
typeOfMsg = START_ACQ;
startAlgoChar = PROTOCOL_START_ALGO;
% algo = CHARACTERIZATION;
algo = CLASSIC_PSO;
% algo = PARALLEL_PSO;
% algo = PARALLEL_PSO_MULTI_SWARM;
% algo = MULTI_UNIT;
% algo = EXTREMUM_SEEKING;
% units = uint8(0:1:7);
units = uint8(0:1:6);
nUnits = uint8(length(units));
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

close all

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

fig = figure(1);
set(gcf, 'Position', get(0,'Screensize'));
lengthOfData = length(posMem) / nData;
for i = 1 : nUnits
%   fig(i) = figure(i);
  subplot(2,nUnits,i)
  plot(tsMem, posMem(i:nUnits:end));
  subplot(2,nUnits,i+nUnits)
  plot(tsMem, powMem(i:nUnits:end));
end

% if matlabMode == 'c'
%   % Extract data from figure
%   for i = 1 : nSolarCells
%     axesObjs = get(fig(i), 'Children');
%     dataObjs = get(axesObjs, 'Children');
%     xdata = get(dataObjs, 'XData');
%     ydata = get(dataObjs, 'YData');
%     ndata = length(xdata);
% 
%     x1 = [xdata{ndata}];
%     y1 = [ydata{ndata}];
%     for j = ndata - 1 : -1 : 1
%       x1 = [x1 xdata{j}];
%       y1 = [y1 ydata{j}];
%     end
% 
%     R(i,:) = x1./255.*1000 + 50;
%   %   P(i,:) = y1.^2 ./ R(i,:);
%     P(i,:) = y1;
%   end
% 
%   fig(i+1) = figure(i +1);
%   hold on
%   set(gcf,'units','normalized','outerposition',[0 0 1 1])
%   
%   plotLines  = {'-' '-' '-' '-' '--' '--' '--' '--'};
%   plotWidths = [.5 .5 .5 .5 1 1 1 1];
%   
%   for i = 1 : nSolarCells
%     Ropt(i) = R(i,find(P(i,:) >= max(P(i,:))));
%   end
%   
%   for i = 1 : nSolarCells
%     if matlabMode == 'c'
%       plot(R(i,:), P(i,:), plotLines{i}, 'LineWidth', plotWidths(i))
%     else
%       plot(R(i,:), P(i,:))
%     end
%     legendStr{i} = ['Cellule ' num2str(i)];
%   end
%   legend(legendStr);
% %   legend({'Cellule 1' 'Cellule 2' 'Cellule 3' 'Cellule 4'})
%   title('Duty cycle = test%')
%   
%   yAxis = ylim;
%   for i = 1 : nSolarCells
%     text(800, yAxis(2)*(1.01 - i*.03), ['Ropt' num2str(i) ' = ' num2str(Ropt(i)) ' \Omega'])
%   end
%   
%   Ropt
%   
%   % saveas(fig(nSolarCells + 1),'4cells/test','fig')
%   % saveas(fig(nSolarCells + 1),'4cells/test','jpg')
%   
% elseif matlabMode == 'p'
%   format short g
% %   psoString = {'i' 'Pbest1' 'Pbest2' 'Pbest3' 'Gbest' 'v1' 'v2' 'v3'};
% %   psoData;
% %   close all
%   % Extract data from figure
%   for i = 1 : nSolarCells*2
%     axesObjs = get(fig(i), 'Children');
%     dataObjs = get(axesObjs, 'Children');
%     xdata = get(dataObjs, 'XData');
%     ydata = get(dataObjs, 'YData');
%     ndata = length(xdata);
% 
%     x1 = [xdata{ndata}];
%     y1 = [ydata{ndata}];
%     for j = ndata - 1 : -1 : 1
%       x1 = [x1 xdata{j}];
%       y1 = [y1 ydata{j}];
%     end
% 
%     tt(i,:) = x1;
%     yy(i,:) = y1;
%   end
%   
%   plotLines  = {'-' '-' '-' '-' '--' '--' '--' '--'};
%   plotWidths = [.5 .5 .5 .5 1 1 1 1];
% 
%   fig(i+1) = figure(i +1);
%   subplot(2,1,1)
%   hold on
%   for i = 1 : nSolarCells
% %     plot(tt(i,:), yy(i,:), plotLines{i}, 'LineWidth', plotWidths(i))
%     plot(tt(i,:), yy(i,:))
%     legendStr{i} = ['Cellule ' num2str(i)];
%   end
%   legend(legendStr);
%   xlabel('Itération')
%   ylabel('Résistance externe [\Omega]')
%   title('Évolution des résistances externes')
%   
%   subplot(2,1,2)
%   hold on
%   for i = nSolarCells + 1 : nSolarCells*2
% %     plot(tt(i,:), yy(i,:), plotLines{i}, 'LineWidth', plotWidths(i))
%     plot(tt(i,:), yy(i,:))
%   end
%   legend(legendStr);
%   xlabel('Itération')
%   ylabel('Puissance [W]')
%   title('Évolution des puissances des cellules')
%   
% %   set(gcf,'units','normalized','outerposition',[0 0 1 1])
%  
% elseif matlabMode == 'm'
% %   close all
% % Extract data from figure
%   for i = 1 : nSolarCells*2
%     axesObjs = get(fig(i), 'Children');
%     dataObjs = get(axesObjs, 'Children');
%     xdata = get(dataObjs, 'XData');
%     ydata = get(dataObjs, 'YData');
%     ndata = length(xdata);
% 
%     x1 = [xdata{ndata}];
%     y1 = [ydata{ndata}];
%     for j = ndata - 1 : -1 : 1
%       x1 = [x1 xdata{j}];
%       y1 = [y1 ydata{j}];
%     end
% 
%     tt(i,:) = x1;
%     yy(i,:) = y1;
%   end
%   
%   plotLines  = {'-' '-' '-' '-' '--' '--' '--' '--'};
%   plotWidths = [.5 .5 .5 .5 1 1 1 1];
% 
%   fig(i+1) = figure(i +1);
%   subplot(2,1,1)
%   hold on
%   for i = 1 : nSolarCells
% %     plot(tt(i,:), yy(i,:), plotLines{i}, 'LineWidth', plotWidths(i))
%     plot(tt(i,:), yy(i,:))
%     legendStr{i} = ['Cellule ' num2str(i)];
%   end
%   legend(legendStr);
%   xlabel('Itération')
%   ylabel('Résistance externe [\Omega]')
%   title('Évolution des résistances externes')
%   
%   subplot(2,1,2)
%   hold on
%   for i = nSolarCells + 1 : nSolarCells*2
% %     plot(tt(i,:), yy(i,:), plotLines{i}, 'LineWidth', plotWidths(i))
%     plot(tt(i,:), yy(i,:))
%   end
%   legend(legendStr);
%   xlabel('Itération')
%   ylabel('Puissance [W]')
%   title('Évolution des puissances des cellules')
%   
% %   set(gcf,'units','normalized','outerposition',[0 0 1 1])
% end