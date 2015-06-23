% dcf77_pll.m (TR 27-Mai-2015)
%
% Edited by: 
%
%                   PLL for calculation of phaseshifts in a signal
%                   
%
%       inp_i         : Real input signal (result of Hilbert Filter)
%       inp_q         : imaginary input signal (result of Hilbert Filter)
%       fs            : Sampling frequency
%       out           : Phase-output of the PLLs
%

function [out] = dcf77_pll(inp_i, inp_q, fs)

siglen = length(inp_i);
out=zeros(1,siglen);

%Initial values
vco_i = 0;
vco_q = 0;
z_lf  = 0;
z_vco = 0;

%Parameters
lf_alpha = 1;%.5;       %s. seite 165
lf_beta  = 1/100;%9*lf_alpha;
vco_kv   = 50;%500;
vco_wc   = 2*pi*fs/4;

for k=1:siglen    
    pdOut = dcf77_phase_detector(inp_i(k),inp_q(k),vco_i,vco_q);
    [y_out, z_lf] = dcf77_loop_filter( pdOut, z_lf, lf_alpha, lf_beta);
    [vco_i, vco_q, z_vco] = dcf77_vco( y_out , 1/fs, vco_wc, vco_kv, z_vco);
    
    out(k) = y_out;
end

end