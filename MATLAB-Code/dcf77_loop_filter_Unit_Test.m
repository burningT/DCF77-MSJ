% dcf77_loop_filter_Unit_Test.m (TR 27-Mai-2015)
%
% Edited by: 
%
%                   Test of the PLL Loop Filter
%

close all
clear all

Fs = 5438.6;
f_sig = Fs/4;
num_periods = 20;

alpha = 1;
beta = 1;
 
sin5 = sin(2*pi*(Fs/5)*(0:1/Fs:num_periods*1/Fs));
sin4 = sin(2*pi*(Fs/4)*(0:1/Fs:num_periods*1/Fs));
sin3 = sin(2*pi*(Fs/3)*(0:1/Fs:num_periods*1/Fs));
sin2 = sin(2*pi*(Fs/2)*(0:1/Fs:num_periods*1/Fs));
sin1 = sin(2*pi*(Fs/1)*(0:1/Fs:num_periods*1/Fs));

sig = 2 + .8*sin1 + .6*sin2 + .4*sin3 + .2*sin4+ .1*sin5;

[out, delay] = dcf77_loop_filter( sig, 0, alpha, beta);

plot(sig);
title('Eingang');

figure;
plot(out);
title('Ausgang');   %Steigung äquivalent dem Offset erwartet

figure;
plot(sig-out);
title('Eingang-Ausgang');

