%dcf77_phase_detector_Unit_Test
clear,close all, clc
format long e

global tmp_I;
global tmp_Q;

Fs=5438.596;
phasediff = 45;
f_sig = Fs/4;%Hz
t =[0:0.01:30]/(f_sig);
% x1 = cos(2*pi*t)+j*sin(phi);
% x2 = conj(cos(phi+delta_phi)+j*sin(phi+delta_phi));

SIG1 = exp(+1j*2*pi*f_sig*t);
SIG2 = exp(-1j*(2*pi*f_sig*t+phasediff*pi/180));
[phaseout] = dcf77_phase_detector(real(SIG1),imag(SIG1),real(SIG2),imag(SIG2));

 %phase123 =phase(tmp_I+j*tmp_Q);
%  
Erg=SIG1.*SIG2;
phase_test = phase(Erg);
 
 figure,plot(t,phaseout/pi*180,t,phase_test/pi*180+1),legend('phaseout','phasetest'),grid 
 figure,plot(t,tmp_I,t,tmp_Q,t,real(Erg)+1,t,imag(Erg)+1),legend('tmp_I','tmp_Q','Erg_I','Erg_Q'),grid
% figure,plot(t,real(z3),t,imag(z3)),legend('z3 real ','z3 imag'),grid
%  figure,plot(t,phase_test,t,phase123),legend('phase z3','phase tmp'),grid

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% %Design HilberFilter
% N=6;
% Fs=5438.596;
% 
% plot_frequenzgang=1;
% b_FIR_hilber = firpm(N,[.4 .6],[1 1],'Hilbert');
% b_delays = [zeros(1,N/2) 1];
% 
% if plot_frequenzgang == 1
% freq=(1:floor(Fs))/Fs;
% hz = freqz(b_FIR_hilber,1, 2*pi*freq);
% figure,plot(freq*Fs,db(hz),'b'),grid
% figure, plot(freq,phase(hz))
% end
% 
% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% %Define Signal
% f_sig = Fs/4;%Hz
% phasediff = 90;
% global t;
% t =[1:1000]/Fs;
% sig1=sin(2*pi*f_sig*t);
% sig2=sin(2*pi*f_sig*t+phasediff*pi/180);
% %figure,plot(t,sig1,t,sig2);
% 
% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% %Generate Input Signal 1
% [sig_Q] = filter(b_FIR_hilber,1,sig1);      
% [sig_I] = filter(b_delays,1,sig1);
% figure,grid,plot(t,sig_I,t,sig_Q),legend('sig_I','sig_Q')
% 
% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% %Generate Input Signal 2
% [VCO_Q] = filter(b_FIR_hilber,1,sig2);      
% [VCO_I] = filter(b_delays,1,sig2);
% figure,grid,plot(t,VCO_I,t,VCO_Q),legend('VCO_I','VCO_Q')
% 
% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% %Aufruf Phasendetektor
% 
% [phase] = dcf77_phase_detector(sig_I,sig_Q,VCO_I,VCO_Q);


        