function [phaseout] = dcf77_phase_detector(Inp_I,Inp_Q,VCO_I,VCO_Q)
% Calculates the Phase between the Complex Signal 'Inp' and 'VCO'

global tmp_I;
global tmp_Q;

% Komplexe Multiplikation : z1 * z2 = (a1*a2+a1*jb2+jb1*a2-b1*b2)    
tmp_I= Inp_I .* VCO_I - Inp_Q .* VCO_Q;
tmp_Q= Inp_I .* VCO_Q + Inp_Q .* VCO_I;
phaseout = atan2(tmp_Q,tmp_I);

end