%HilbertDesign
clear,clc,close all

N=6;
Fs=5438.596;

plot_frequenzgang=1;

%[N_FIR_unused,fo,ao,w] = firpmord([fp fs], [A1 A2], [0.01 0.01], Fs);
b_FIR_hilber = firpm(N,[.4 .6],[1 1],'Hilbert');

if plot_frequenzgang == 1
freq=(1:999*2)/2000;
hz = freqz(b_FIR_hilber,1, 2*pi*freq);
figure,plot(freq*Fs,db(hz),'b'),grid
figure, plot(freq,phase(hz))
end