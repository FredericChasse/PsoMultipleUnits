files = dir('./test results');

idxTmp = [];
for i = 1 : length(files)
  if strfind(files(i).name, '~')
    idxTmp = [idxTmp i];
  end
end

files(idxTmp) = [];

idxParallelPso = [];
idxClassicPso = [];
idxParallelPsoMultiSwarm = [];

for i = 1 : length(files)
  if (strfind(files(i).name, 'Parallel PSO Multi Swarm'))
    idxParallelPsoMultiSwarm = [idxParallelPsoMultiSwarm i];
  end
end

for i = 1 : length(files)
  if (strfind(files(i).name, 'Parallel PSO 2'))
    idxParallelPso = [idxParallelPso i];
  end
end

for i = 1 : length(files)
  if (strfind(files(i).name, 'Sequential PSO'))
    idxClassicPso = [idxClassicPso i];
  end
end

powersParaPsoMultiSwarm = zeros(1, length(idxParallelPsoMultiSwarm));
for i = 1 : length(idxParallelPsoMultiSwarm)
  file = ['./test results/' files(idxParallelPsoMultiSwarm(i)).name];
  powers = xlsread(file, 'Powers');
  powersParaPsoMultiSwarm(i) = sum(powers(end, 2:end));
end

if ~isempty(idxParallelPso)
  powersParaPso = zeros(1, length(idxParallelPso));
  for i = 1 : length(idxParallelPso)
    file = ['./test results/' files(idxParallelPso(i)).name];
    powers = xlsread(file, 'Powers');
    powersParaPso(i) = sum(powers(end, 2:end));
  end
end

powersSeqPso = zeros(1, length(idxClassicPso));
for i = 1 : length(idxClassicPso)
  file = ['./test results/' files(idxClassicPso(i)).name];
  powers = xlsread(file, 'Powers');
  powersSeqPso(i) = sum(powers(end, 2:end));
end

meanPowParaMM = mean(powersParaPsoMultiSwarm);
fprintf(['Para PSO Multi Swarm power = ' num2str(meanPowParaMM) '\n']);
if ~isempty(idxParallelPso)
  meanPowPara   = mean(powersParaPso);
  fprintf(['Para PSO power = ' num2str(meanPowPara) '\n']);
end
meanPowSeq    = mean(powersSeqPso);
fprintf(['Sequential PSO power = ' num2str(meanPowSeq) '\n']);
