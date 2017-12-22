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

PROTOCOL_START_ALGO       = uint8('!' - 0);
PROTOCOL_STOP_ALGO        = uint8('x' - 0);

SIZE_OF_PROTOCOL_HEADER   = 4;

N_UNITS_TOTAL             = 15;

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
% port.BaudRate = 9600;
port.DataBits = 8;
port.Parity = 'none';
port.StopBits = 1;
port.Terminator = '';

% Connect to instrument object, port.
fopen(port);


%% Kill PWM

initLedIntensity = 0;
delimiter = PROTOCOL_DELIMITER;
typeOfMsg = INIT_PERTURB;
lengthOfPayload = typecast(uint16(2), 'uint8');
payload = typecast(uint16(initLedIntensity), 'uint8');

buf = [delimiter, typeOfMsg, lengthOfPayload, payload];
fwrite(port, buf);


%% Close port

% Remove contents of input buffer
flushinput(port);

% Disconnect from instrument object.
fclose(port);

% Clean up all objects.
delete(port);