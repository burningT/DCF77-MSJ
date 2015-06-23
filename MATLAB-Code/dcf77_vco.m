% dcf77_vco.m (TR 26-Mai-2015)
%
% Edited by: 
%
%                   Voltage Controlled Oscillator
%
%                   This function generates a complex sinus signal based on
%                   its input y. The generation of the signal is based on
%                   the functionality of a common VCO as part of an PLL.
%                   
%
%       y           : Input signal (filtered result of Phase Detector)
%       ts          : Sampling period
%       wc          : Reference frequency of the VCO
%       kv          : Amplification of the input signal
%       zi          : Initial delays
%       vi          : In phase output of the VCO
%       vq          : Quadrature output of the VCO
%       zf          : Final delays
%

function [vi, vq, zf] = dcf77_vco( y , ts, wc, kv, zi);

y_length = length(y);

%Amplify input
y_temp = y*kv*ts;       %for-loop in C

%Regulate output-frequency

z_temp = zi;    %initialise delay

for k = 1:y_length
    z_temp = z_temp + y_temp(k) + wc*ts;
    y_temp (k) = z_temp;
end

zf = z_temp;    %set final delay

%prevent overflow
if y_temp > pi
    y_temp = y_temp-2*pi;
elseif y_temp < -pi
    y_temp = y_temp+2*pi;
end

%Generate output signal

vi = cos(-y_temp);
vq = sin(-y_temp);