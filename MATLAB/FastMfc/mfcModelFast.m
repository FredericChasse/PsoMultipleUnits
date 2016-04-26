function [dy, Pout]= mfcModelFast(T, y, dummy, S0, Rext)
F = 96485;%
R = 8.314472;%
T = 298.15;%
yield = 22.753;%
% Ych4 = .3;%
qmax_a = 8.484;%
qmax_m = 8.2;%
umax_a = 1.9753;%
umax_m = .1;%
Ks_a = 20;%
Ks_m = 80;%
m = 2;%
gamma = 663400;%
Mtotal = .05;%
Km = .2*Mtotal;%
% Kd_a = .02*umax_a;%
% Kd_m = .02*umax_m;%
Xmax = 512.5;%
Kx = .01*4;%
Rmin = 25;%
Rmax = 2000 + Rmin;%
Emin = .01;%
Emax = 0.25 + 0.4144+ Emin;%
Kr = .024;%

% i0ref=0.4*4;%

e = 0.0001*Mtotal;%

V = 0.2;%
Fin = 0.1527;%

D = Fin/V;

Xtot = 500;

Xeg = 250;

S   = y(1);
Mox = y(2);
xm = Xtot - Xeg;


%% �quations dynamiques
  
Mred = Mtotal - Mox;

Rint = Rmin + (Rmax - Rmin) * exp(-Kr*Xeg);                         % (22)
Eocv = Emin + (Emax - Emin) * exp(-1/(Kr*Xeg));                     % (23)

% alpha = 1/2*(1 + tanh(Kx*(Xeg + xm - Xmax)));                    % (9)

qa = qmax_a * S / (Ks_a + S) * Mox / (Km + Mox);                    % (15)
qm = qmax_m * S / (Ks_m + S);                                       % (16)

% ueg = umax_a * S / (Ks_a + S) * Mox / (Km + Mox);                    % (13)

% Imfc = (Eocv - n_conc) / (Rext + Rint) * Mred / (e + Mred);         % (21)
temp = Eocv - (R*T/m/F)*abs(log(complex(Mtotal/Mred)));
% Imfc = (Eocv - (R*T/m/F)*abs(log(complex(Mtotal/Mred))))/(Rext+Rint)*Mred/(e+Mred);
Imfc = temp/(Rext+Rint)*Mred/(e+Mred);

% Pout = ((Eocv - (R*T/m/F)*abs(log(complex(Mtotal/Mred))))*Mred/(e+Mred)-Rint*Imfc)*Imfc;
Pout = (temp*Mred/(e+Mred)-Rint*Imfc)*Imfc;

Sp = -qa*Xeg - qm*xm + D*(S0 - S);                                   % (6)
% Xap = ueg*xa - Kd_a*xa - alpha_a*D*xa;                               % (7)
% Xmp = um*xm - Kd_m*xm - alpha_m*D*xm;                               % (8)

Moxp = -yield*qa + gamma*Imfc / (m*F*V*Xeg) * 86400;                 % (12)

% if xa < 1 && Xap < 0
%   Xap = 0;
% end
% 
% if xm < 0.99 && Xmp < 0
%   Xmp = 0;
% end

% Pout = Rext*Imfc^2;

% dy = [Sp; Xap; Xmp; Moxp];
dy = [Sp; Moxp];
