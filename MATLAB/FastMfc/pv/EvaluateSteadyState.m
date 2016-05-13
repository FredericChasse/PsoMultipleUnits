nIterations = 30;

% for kk = 1 : 10
%   
%   dstr = [num2str(kk) 'd'];
%   jstr = [num2str(kk) 'J'];
%   
%   [d, TXT, RAW] = xlsread('C:\Users\ironi\Documents\ETS\Maîtrise\Été 2016\Article\Data/Sim_data.xlsx', dstr)
%   [J, TXT, RAW] = xlsread('C:\Users\ironi\Documents\ETS\Maîtrise\Été 2016\Article\Data/Sim_data.xlsx', jstr)
% 
%   iSteadyState = nIterations + 1;
% 
%   if oDoPerturbStatic || oDoPerturbDynamic
%     iStart = iPerturb;
%   else
%     iStart = 1;
%   end
% 
%   for iData = iStart : nIterations
% 
%     meanD = mean(d(iData:end,:));
% 
%     for jData = iData : nIterations
%       if ( mean( (d(jData, :) <= (1 + ssPrecision).*meanD) & (d(jData, :) >= (1 - ssPrecision).*meanD) ) ~= 1 )
%         iSteadyState = nIterations + 1;
%         break;
%       else
%         iSteadyState = iData;
%       end
%     end
% 
%     if (iSteadyState ~= nIterations + 1)
%       break;
%     end
%   end
% 
%   if iStart ~= 1
%     iSteadyState = iSteadyState - iStart;
%   else
%     iSteadyState;
%   end
%   
%   sequentialSteadyState(kk) = iSteadyState;
%   
%   sequentialFinalPower(kk) = mean(J(end,:));
% 
% end

for kk = 1 : 10
  
  dstr = [num2str(kk) 'd'];
  jstr = [num2str(kk) 'J'];
  
  [d, TXT, RAW] = xlsread('C:\Users\ironi\Documents\ETS\Maîtrise\Été 2016\Article\Data\multi_cell/Sim_data.xlsx', dstr);
  [J, TXT, RAW] = xlsread('C:\Users\ironi\Documents\ETS\Maîtrise\Été 2016\Article\Data\multi_cell/Sim_data.xlsx', jstr);

  iSteadyState = nIterations + 1;

  if oDoPerturbStatic || oDoPerturbDynamic
    iStart = iPerturb;
  else
    iStart = 1;
  end

  for iData = iStart : nIterations

    meanD = mean(d(iData:end,:));

    for jData = iData : nIterations
      if ( mean( (d(jData, :) <= (1 + ssPrecision).*meanD) & (d(jData, :) >= (1 - ssPrecision).*meanD) ) ~= 1 )
        iSteadyState = nIterations + 1;
        break;
      else
        iSteadyState = iData;
      end
    end

    if (iSteadyState ~= nIterations + 1)
      break;
    end
  end

  if iStart ~= 1
    iSteadyState = iSteadyState - iStart;
  else
    iSteadyState;
  end
  
  sequentialSteadyState(kk) = iSteadyState;
  
  sequentialFinalPower(kk) = sum(J(end,:));
%   sequentialFinalPower(kk) = sum(J(end,1:7), J(end,9:10));

end