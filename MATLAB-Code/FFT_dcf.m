%fft von DCF Testsignal
clear, close all
Fs=5438.596; %Hz

%%Signal laden
sig =  load('gen_DCF77_several_minutes.mat','-ascii');
%sig = sig(1:400000);
%sig = sig(11111:11111+4095);
figure, plot(sig), grid

%%FFT Ursprungssignal
lenSig=length(sig);
NFFT = 2^nextpow2(lenSig)*64; % Next power of 2 from length of y
SIG = fft(sig,NFFT)/lenSig;
f = Fs/2*linspace(0,1,NFFT/2+1);
figure, plot(f,db(2*abs(SIG(1:NFFT/2+1)))), grid


%%'Nach Null schieben'
%for k=1:lenSig
%sig(k)=sig(k)*exp(-1i*2*pi/4*k);
%end
%sig=2*real(sig);
%sig=detrend(sig,0);

%Betrag bilden
sig=abs(sig);

%Tiefpass Filtern 'integrieren'
sig=tp2(sig);

figure,plot(sig)
%%FFT nach Null geschoben
SIG = fft(sig,NFFT)/lenSig;
figure, plot(f,db(2*abs(SIG(1:NFFT/2+1)))), grid

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Schwellen detektieren
sig_bin = zeros(lenSig,1);

for i=1:lenSig
    if sig(i) < 0.2
        sig_bin(i) = 0;
    else
         sig_bin(i) = 1;
    end
end

figure,plot(sig_bin)
%%FFT nach Null geschoben
SIG = fft(sig_bin,NFFT)/lenSig;
figure, plot(f,db(2*abs(SIG(1:NFFT/2+1)))), grid



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Bits suchen
%100ms = 0
%200ms = 1
count_zero = 0;
for i=1:lenSig
    if sig_bin(i) == 0
        count_zero = count_zero + +1;
    end
end

%564 entspricht 100ms
if count_zero < 600
    disp('Null entdeckt');
    
else
     disp('Eins entdeckt');
end


%%I und Q Vektoren generieren
% I_vek = zeros(lenSig,1);
% Q_vek = zeros(lenSig,1);
% anzZust = 4;
% for ind=1:lenSig
%     if mod(ind,anzZust) == 1
%         I_vek(ind)=1;
%     elseif mod(ind,anzZust) == 2
%         Q_vek(ind)=-1;
%     elseif mod(ind,anzZust) == 3
%         I_vek(ind)=-1;
%     else mod(ind,anzZust) == 4;
%         Q_vek(ind)=1;
%     end
% end