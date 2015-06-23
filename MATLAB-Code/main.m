function []= main(file,command)
close all, clc
%main('simu','volksempfaenger')
%main('simu','hilbert')
%main('realData','volksempfaenger')
%main('realData','hilbert')

%file
Fs=5438.596; %Hz

%%Signal laden
switch file
    case 'realData'
    [sig_ges,f_A] = wavread('B48_UPV_Feature_DModC6713_plus_ADDA16_EDMA_SDRAM_5438_Adler.wav');
    sig_ges=sig_ges*4;
   
    case 'simu'
    sig_ges =  load('gen_DCF77_several_minutes.mat','-ascii');
end
 figure,plot(sig_ges)
len_sig_ges = length(sig_ges);
block_laenge = 4096;
buffer_pll_len = 2*block_laenge;
A_buffer_pll = zeros(buffer_pll_len,1);
B_buffer_pll = zeros(buffer_pll_len,1);
ind_pll=0;
input_buffer_pll = A_buffer_pll;
working_buffer_pll = B_buffer_pll;

%Schneide Eingangsvektor ab, sodass dieser ein Vielfaches der Blocklänge
%ist
len_sig_ges =  floor(len_sig_ges/block_laenge)*block_laenge;
%Bestimmte Bitfolge
erg=[];
%Suche Startzeit, überschreitet dieser Wert eine Millionen
count_min=0;
%Zähle Länge der Amplitudenabsenkung
count_zero = 0;
%Es muss geklärt werden, ob das letzte Bit der aktuellen Sequenz Teil einer
%Absenkung ist. Wenn ja, darf in der aktuellen Iteration kein Bit
%ausgegeben werden.
flag_absenkung =0;
%Hilbertkoeff

N=6;
%Koeffizienten sind aus dem Example in der Matlab-Doku
b_FIR_hilber = firpm(N,[.4 .6],[1 1],'Hilbert');
b_delays = [zeros(1,N/2) 1];
zf_Q = zeros(N,1);
zf_I = zeros(N/2,1);


for iter=1:block_laenge:len_sig_ges
%sig = sig(1:400000);
sig = sig_ges(iter:iter+block_laenge-1);
figure, plot(sig), grid

%%FFT Ursprungssignal
lenSig=length(sig);
flag_absenkung = 0;
%NFFT = 2^nextpow2(lenSig)*64; % Next power of 2 from length of y
%SIG = fft(sig,NFFT)/lenSig;
%f = Fs/2*linspace(0,1,NFFT/2+1);
%%%%%%%%%figure, plot(f,db(2*abs(SIG(1:NFFT/2+1)))), grid


%%'Nach Null schieben'
%for k=1:lenSig
%sig(k)=sig(k)*exp(-1i*2*pi/4*k);
%end
%sig=2*real(sig);
%sig=detrend(sig,0);

switch command
    case 'volksempfaenger'
        %Betrag bilden
        sig=abs(sig);
        %Tiefpass Filtern 'integrieren'
        sig=tp2(sig);
    case 'hilbert'
        [sig_Q, zf_Q] = filter(b_FIR_hilber,1,sig,zf_Q);
        %Z^(-N/2) = z^-3
        [sig_I, zf_I] = filter(b_delays,1,sig,zf_I);
        %Hüllkurve bestimmen
%          figure,plot(detrend(phase(sig_I+1j*sig_Q)))
        sig = sqrt(sig_I.^2+sig_Q.^2);       
        [pllout] = dcf77_pll(sig_I, sig_Q,Fs);
        figure,plot(pllout)
end

pause
%figure,plot(sig)
%%FFT nach Null geschoben
%SIG = fft(sig,NFFT)/lenSig;
%%%%%%%%%figure, plot(f,db(2*abs(SIG(1:NFFT/2+1)))), grid

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
%figure, plot(sig), hold on, plot(sig_bin), hold off;

%%%%%%%%%%%figure,plot(sig_bin)
%%FFT nach Null geschoben
%%%SIG = fft(sig_bin,NFFT)/lenSig;
%%%%%%%%%%figure, plot(f,db(2*abs(SIG(1:NFFT/2+1)))), grid



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Bits suchen
%100ms = 0
%200ms = 1

for i=1:lenSig
    if sig_bin(i) == 0
        count_zero = count_zero +1;
        count_min = 0;
        %Der Buffer für die PLL wird bereits mit Beginn der Absenkung befüllt, so
        %hat die PLL etwas Zeit sich einzuschwingen.
%        input_buffer_pll(ind_pll)= sig(i);
      %  ind_pll = ind_pll+1;
        
        %Handelt es sich gerade um das letzte Bit der aktuellen Sequenz
        if i==lenSig
            flag_absenkung=1;
        end
    else
        count_min = count_min +1;
    end
end

%Neue Minute erkennen
% Fs = 5438,6Hz => 5438 Samples pro Sekunde. Wenn für >5438 Samples keine
% Nullen auftauchen, somuss es sich um den Minutenübergang handeln. + 200 ist
% Sicherheitsabstand.
if count_min > round(Fs)+200
    disp(length(erg));
    disp(dcf77_bitdecoder(erg));  %TR
    erg=[];    
    disp('NEUE MINUTE-----------------------');
end

%564 entspricht, 600 schafft etwas Toleranz
%                100ms, Logisch 0
%                200ms, Logisch 1
if count_zero < 600 && count_zero > 50 && flag_absenkung == 0
    disp('Null entdeckt');
    erg=[erg(:)',0];
    %Bit erkannt, wieder auf Null setzen
    count_zero = 0;
elseif (count_zero > 600 && flag_absenkung == 0)%200ms
     disp('-- Eins entdeckt');
     erg=[erg(:)',1];
     %Bit erkannt, wieder auf Null setzen
    count_zero = 0;
end


end
  disp(length(erg));
  
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Phasenrauschen bestimmen
 % if ind_pll == buffer_pll_len/2
    
      %Null Setzten, um neu zu befüllen.
 %     ind_pll=0;  
 % end

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
end