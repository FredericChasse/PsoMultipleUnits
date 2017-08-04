pot = 50:1000/255:1050;
potInv = 1./pot;
% potInv1000 = potInv ./ 1000;
potInv1000 = potInv;

for i = 1 : 32
  str = [];
  for j = 1 : 8
    str = [str [num2str(potInv1000((i-1)*8+j), '%.6f') ', ']];
  end
  str = [str '\n'];
  fprintf(str)
end
