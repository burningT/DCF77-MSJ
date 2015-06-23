clear all
close all

Fs = 5438.6;
f_sig = Fs/4;
num_periods = 10;
resolution = 1e-4;
 
phi= 2*pi*f_sig*(0:resolution:num_periods*1/Fs);

delta_phi = pi/4;

x1 = cos(phi)+j*sin(phi);
x2 = conj(cos(phi+delta_phi)+j*sin(phi+delta_phi));

x_erg = x1.*x2;

phase2 = dcf77_phase_detector(real(x1),imag(x1),real(x2),imag(x2));

plot(real(x_erg));
title('Simulierter Ausgang');
hold on;
plot(imag(x_erg));
hold off;
figure;
plot(phase(x_erg)/pi*180);
title('Simulierte Phase');
figure;
plot(phase2/pi*180);
title('Phase des Diskriminators');