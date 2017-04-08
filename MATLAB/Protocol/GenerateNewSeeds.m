function [ seed1, seed2 ] = GenerateNewSeeds(  )

f = @() randi([0 intmax('uint32')], 'uint32');

seed1 = typecast([f() f()], 'uint64');
seed2 = typecast([f() f()], 'uint64');

end

