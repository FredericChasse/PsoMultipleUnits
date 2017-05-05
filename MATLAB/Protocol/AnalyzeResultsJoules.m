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

joulesParaPsoMultiSwarm = zeros(1, length(idxParallelPsoMultiSwarm));
for i = 1 : length(idxParallelPsoMultiSwarm)
  file = ['./test results/' files(idxParallelPsoMultiSwarm(i)).name];
  joules = xlsread(file, 'Joules total');
  joulesParaPsoMultiSwarm(i) = joules(end);
end

if ~isempty(idxParallelPso)
  joulesParaPso = zeros(1, length(idxParallelPso));
  for i = 1 : length(idxParallelPso)
    file = ['./test results/' files(idxParallelPso(i)).name];
    joules = xlsread(file, 'Joules total');
    joulesParaPso(i) = joules(end);
  end
end

joulesSeqPso = zeros(1, length(idxClassicPso));
for i = 1 : length(idxClassicPso)
  file = ['./test results/' files(idxClassicPso(i)).name];
  joules = xlsread(file, 'Joules total');
  joulesSeqPso(i) = joules(end);
end

meanJouleParaMM = mean(joulesParaPsoMultiSwarm);
fprintf(['Para PSO Multi Swarm joules = ' num2str(meanJouleParaMM) '\n']);
if ~isempty(idxParallelPso)
  meanJoulePara   = mean(joulesParaPso);
  fprintf(['Para PSO joules = ' num2str(meanJoulePara) '\n']);
end
meanJouleSeq    = mean(joulesSeqPso);
fprintf(['Sequential PSO joules = ' num2str(meanJouleSeq) '\n']);
