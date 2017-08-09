clear all

v1 = [];
v2 = v1;
v3 = v2;

m = csvread('./DbgData/ffast1.csv', 0, 1);
volts = m(:,1);
v1 = [v1;volts(1:15:end)];
v2 = [v2;volts(2:15:end)];
v3 = [v3;volts(3:15:end)];
m = csvread('./DbgData/ffast2.csv', 0, 1);
volts = m(:,1);
v1 = [v1;volts(1:15:end)];
v2 = [v2;volts(2:15:end)];
v3 = [v3;volts(3:15:end)];
m = csvread('./DbgData/ffast3.csv', 0, 1);
volts = m(:,1);
v1 = [v1;volts(1:15:end)];
v2 = [v2;volts(2:15:end)];
v3 = [v3;volts(3:15:end)];
m = csvread('./DbgData/ffast4.csv', 0, 1);
volts = m(:,1);
v1 = [v1;volts(1:15:end)];
v2 = [v2;volts(2:15:end)];
v3 = [v3;volts(3:15:end)];
% m = csvread('./DbgData/norc5.csv', 0, 1);
% volts = m(:,1);
% v1 = [v1;volts(1:15:end)];
% v2 = [v2;volts(2:15:end)];
% v3 = [v3;volts(3:15:end)];

time = .00015:.00015:length(v1)*.00015;
v1=v1';
v2=v2';
v3=v3';
x=[time;v1];
ts = timeseries(x(2:end,:),x(1,:));
save('./DbgData/testv1_ffast','ts','-v7.3')
x=[time;v2];
ts = timeseries(x(2:end,:),x(1,:));
save('./DbgData/testv2_ffast','ts','-v7.3')
x=[time;v3];
ts = timeseries(x(2:end,:),x(1,:));
save('./DbgData/testv3_ffast','ts','-v7.3')