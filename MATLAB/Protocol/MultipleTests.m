clear
close all

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

%% Test suite

nTests = 10;

% algo = CHARACTERIZATION;
algo = CLASSIC_PSO;
% algo = PARALLEL_PSO;
% algo = PARALLEL_PSO_MULTI_SWARM;
% algo = MULTI_UNIT;
% algo = EXTREMUM_SEEKING;
% units = uint8(0:1:7);
units = uint8(0:1:6);
nUnits = uint8(length(units));

% Wait for the user to start the program
reply = 'N';
while reply == 'N'
  reply = upper(input('Start? Y/N [N]:','s'));
  if isempty(reply)
    reply = 'Y'
  end
end

switch algo
  case CHARACTERIZATION
    algoName = 'Characterization';
  case CLASSIC_PSO
    algoName = 'Sequential PSO';
  case PARALLEL_PSO
    algoName = 'Parallel PSO';
  case PARALLEL_PSO_MULTI_SWARM
    algoName = 'Parallel PSO Multi Swarm';
  case MULTI_UNIT
    algoName = 'Multi-Unit';
  case EXTREMUM_SEEKING
    algoName = 'Extremum Seeking';
  otherwise
    error('Not a valid algo');
end

for iTest = 1 : nTests
  
  fprintf(['iTest = ' num2str(iTest) '\n'])
  Pic2MatlabMulti
  
  time = clock;
  year = num2str(time(1));
  month = num2str(time(2));
  day = num2str(time(3));
  hour = num2str(time(4));
  minute = sprintf('%02.0f', time(5));
  second = sprintf('%02.0f', time(6));
  timeStr = [year '-' month '-' day ' ' hour 'h' minute 'm' second 's'];
%   timeStr = [num2str(time(1)) '-' num2str(time(2)) '-' num2str(time(3)) '-' num2str(time(4)) 'h' num2str(time(5)) 'm' num2str(time(6)) 's'];
  xlsname = ['.\test results\' algoName ' ' timeStr '.xlsx'];
  
  titleStr{1} = 'Ts';
  joules = zeros(length(tsMem), nUnits);
  for iUnit = 1 : nUnits
    titleStr{iUnit+1} = ['Unit ' num2str(iUnit)];
    joules(:,iUnit) = cumtrapz(tsMem, powMem(:,iUnit));
  end
  
  xlswrite(xlsname, titleStr  , 'Positions', 'A1');
  xlswrite(xlsname, tsMem     , 'Positions', 'A2');
  xlswrite(xlsname, posMem    , 'Positions', 'B2');
  xlswrite(xlsname, titleStr  , 'Powers'   , 'A1');
  xlswrite(xlsname, tsMem     , 'Powers'   , 'A2');
  xlswrite(xlsname, powMem    , 'Powers'   , 'B2');
  xlswrite(xlsname, titleStr  , 'Joules'   , 'A1');
  xlswrite(xlsname, tsMem     , 'Joules'   , 'A2');
  xlswrite(xlsname, joules    , 'Joules'   , 'B2');
  
  titleStr = titleStr(2:end);
  titleStr{end+1} = 'Total';
  totalJoules = joules(end,:);
  totalJoules = [totalJoules, sum(totalJoules)];
  xlswrite(xlsname, titleStr   , 'Joules total', 'A1');
  xlswrite(xlsname, totalJoules, 'Joules total', 'A2');
  
  flushinput(port);
end
  

% Remove contents of input buffer
flushinput(port);

% Disconnect from instrument object, obj1.
fclose(port);

% Clean up all objects.
delete(port);

% Save figures
% lastFig = gcf;
% nFigs = lastFig.Number;
% for iFig = 1 : nFigs
%   fig = figure(iFig);
%   saveas(fig, [algoName ' - ' num2str(iFig)], 'fig');
%   saveas(fig, [algoName ' - ' num2str(iFig)], 'jpg');
% end

%% Notify that test has ended

beep
  