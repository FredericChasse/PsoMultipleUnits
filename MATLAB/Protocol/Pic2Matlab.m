clear
close all

%% Constants

% PROTOCOL_DELIMITER = uint8(hex2num('7E'));
PROTOCOL_DELIMITER = uint8(126);

NEW_RNG_SEED = uint8(0);
START_ACQ = uint8(1);
STOP_ACQ = uint8(2);
UNITS_DATA = uint8(3);

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
[seed1, seed2] = GenerateNewSeeds;
seeds = typecast([seed1, seed2], 'uint8');
% seeds = fliplr(seeds);

buf = [delimiter, typeOfMsg, lengthOfPayload, seeds];
fwrite(port, buf);

% Start algo
typeOfMsg = START_ACQ;
startAlgoChar = PROTOCOL_START_ALGO;
algo = CHARACTERIZATION;
% algo = CLASSIC_PSO
% algo = PARALLEL_PSO;
% algo = PARALLEL_PSO_MULTI_SWARM;
% algo = MULTI_UNIT;
% algo = EXTREMUM_SEEKING;
% units = uint8(0:1:7);
units = uint8(0:1:3);
nUnits = uint8(length(units));
% lengthOfPayload = fliplr(typecast(uint16(3 + nUnits), 'uint8'));
lengthOfPayload = typecast(uint16(3 + nUnits), 'uint8');

buf = [delimiter, typeOfMsg, lengthOfPayload, startAlgoChar, algo, nUnits, units];
fwrite(port, buf);

%% Figures init

nSolarCells = double(nUnits);
% Create figure for data
for i = 1 : nSolarCells*2
  fig(i) = figure(i);
  hold on
end

% if (i == 4)
%   pos2 = fig(2).Position;
%   fig(2).Position = [1860 pos2(2) pos2(3) pos2(4)];
%   pos3 = fig(3).Position;
%   fig(3).Position = [pos3(1) 100 pos3(3) pos3(4)];
%   pos4 = fig(4).Position;
%   fig(4).Position = [1860 100 pos4(3) pos4(4)];
% elseif (i == 6)
%   fig(1).Position = [0        838 840 630];
%   fig(2).Position = [840      838 840 630];
%   fig(3).Position = [840+840  838 840 630];
%   fig(4).Position = [0        88  840 630];
%   fig(5).Position = [840      88  840 630];
%   fig(6).Position = [840+840  88  840 630];
% elseif (i == 8)
%   fig(1).Position = [0 1070 640 430];
%   fig(2).Position = [640 1070 640 430];
%   fig(3).Position = [1280 1070 640 430];
%   fig(4).Position = [1920 1070 640 430];
%   fig(5).Position = [0 521 640 430];
%   fig(6).Position = [640 521 640 430];
%   fig(7).Position = [1280 521 640 430];
%   fig(8).Position = [1920 521 640 430];
% end

% Wait for the user to stop the program
% stopBtn = 0;
% f = figure(i+1);
% b = uicontrol('style','push','string','Stop','callback','stopBtn=stopBtn+1');

tsMem = [];
posMem = [];
powMem = [];

nIterations = 256;
tic
for iIteration = 1 : nIterations
  % header
%   buf = fread(port, SIZE_OF_PROTOCOL_HEADER, 'uint8');
  buf = fread(port, SIZE_OF_PROTOCOL_HEADER);
  delimiter = buf(1);
  typeOfMsg = buf(2);
%   lengthOfPayload = typecast(buf(3:4), 'uint16');
  lengthOfPayload = double(typecast(uint8(buf(3:4)'), 'uint16'));
  
  %payload
  buf = fread(port, lengthOfPayload, 'uint8');
%   timestamp = typecast(uint8(buf(1:4)'), 'uint32');
  timestamp = typecast(typecast(uint8(buf(1:4)'), 'uint32'), 'single');
%   ts_ms = double(timestamp) * 0.001;
  nUnits = buf(5);
  nData = buf(6);
  data = typecast(typecast(uint8(buf(7:end)'), 'uint32'), 'single');
  positions = data(1:end/2);
  powers = data(end/2+1:end);
  
  tsMem  = [tsMem timestamp];
  posMem = [posMem positions];
  powMem = [powMem powers];
  
%   for i = 1 : nUnits
%     fig(i);
%     plot(timestamp, positions(i), 'o');
%   end
%   
%   for i = nUnits+1 : nUnits*2
%     fig(i);
%     plot(timestamp, powers(i - nUnits), 'o');
%   end
  
  %update button
%   drawnow
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

lengthOfData = length(posMem) / nData;
for i = 1 : nUnits
  fig(i) = figure(i);
  plot(tsMem, posMem(i:nUnits:end));
end
for i = nUnits + 1 : 2*nUnits
  fig(i) = figure(i);
  plot(tsMem, powMem(i-nUnits:nUnits:end));
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