

%% Constants

PROTOCOL_DELIMITER        = uint8(hex2dec( '7E' ));

% Output
NEW_RNG_SEED              = uint8(hex2dec( '00' ));
START_ACQ                 = uint8(hex2dec( '01' ));
STOP_ACQ                  = uint8(hex2dec( '02' ));
SET_PERTURB               = uint8(hex2dec( '03' ));
SET_DEBUG_DATA            = uint8(hex2dec( '04' ));
INIT_PERTURB              = uint8(hex2dec( '05' ));
% Input
UNITS_DATA                = uint8(hex2dec( '10' ));
PSO_DATA                  = uint8(hex2dec( '11' ));
ADC_DATA                  = uint8(hex2dec( '12' ));
PPSO_PNO_DATA             = uint8(hex2dec( '13' ));

CLASSIC_PSO               = uint8(hex2dec( '00' ));
PARALLEL_PSO              = uint8(hex2dec( '01' ));
PARALLEL_PSO_MULTI_SWARM  = uint8(hex2dec( '02' ));
MULTI_UNIT                = uint8(hex2dec( '03' ));
EXTREMUM_SEEKING          = uint8(hex2dec( '04' ));
CHARACTERIZATION          = uint8(hex2dec( '05' ));
PPSO_PNO                  = uint8(hex2dec( '06' ));
PNO                       = uint8(hex2dec( '07' ));
DEBUG_ADC                 = uint8(hex2dec( '08' ));
POLARIZATION              = uint8(hex2dec( '09' ));

PROTOCOL_START_ALGO       = uint8('!' - 0);
PROTOCOL_STOP_ALGO        = uint8('x' - 0);

SIZE_OF_PROTOCOL_HEADER   = 4;

N_UNITS_TOTAL             = 12;

N_LARGE_SAMPLES           = 300;

POT_MIN_INDEX             = 0;
POT_MAX_INDEX             = 255;
CHARAC_POS_INCREMENT      = 4;
CHARAC_N_ITERATIONS       = ceil((POT_MAX_INDEX - POT_MIN_INDEX + 1) / CHARAC_POS_INCREMENT);



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
port.Timeout = 20;

% Connect to instrument object, port.
fopen(port);


%% Send Stop Algo

delimiter = PROTOCOL_DELIMITER;
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
