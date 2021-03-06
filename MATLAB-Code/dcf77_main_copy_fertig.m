function []= dcf77_main(file,command,option)
close all, clc
%dcf77_main('simu','volksempfaenger','plot')
%dcf77_main('simu','hilbert','plot')
%dcf77_main('realData','volksempfaenger','plot')
%dcf77_main('realData','hilbert','plot')

%dcf77_main('simu','volksempfaenger','')
%dcf77_main('simu','hilbert','')
%dcf77_main('realData','volksempfaenger','')
%dcf77_main('realData','hilbert','')
%file
Fs=5438.596; %Hz

%%Signal laden
switch file
    case 'realData'
    [sig_ges,f_A] = wavread('B48_UPV_Feature_DModC6713_plus_ADDA16_EDMA_SDRAM_5438_Adler.wav');
    sig_ges=sig_ges*8;
   
    case 'simu'
    sig_ges =  load('gen_DCF77_several_minutes.mat','-ascii');
end
 figure,plot(sig_ges),title('Gesamtsignal')
len_sig_ges = length(sig_ges);
block_laenge = 4096;
buffer_pll_len = 2*block_laenge;
A_buffer_pll_re = zeros(buffer_pll_len,1);
B_buffer_pll_re = zeros(buffer_pll_len,1);
A_buffer_pll_im = zeros(buffer_pll_len,1);
B_buffer_pll_im = zeros(buffer_pll_len,1);
%Index f�r die Bef�llung des Buffers der PLL. Matlab Indizierung startet
%bei 1!
ind_pll=1;
%Gibt an das der Input Buffer mit Werten gef�llt wird.
input_buffer_pll_flag=0;
%Gibt an das der Inputbuffer -> Workingbuffer
calc_phase_noise=0;

% In C sind Input.. und Working.. Pointer vom Datentyp Complex.
input_buffer_pll_re = A_buffer_pll_re;
input_buffer_pll_im = A_buffer_pll_im;
working_buffer_pll_re = B_buffer_pll_re;
working_buffer_pll_im = B_buffer_pll_im;

%Buffer f�r den Entscheider
sig_bin = zeros(block_laenge,1);

%Schneide Eingangsvektor ab, sodass dieser ein Vielfaches der Blockl�nge
%ist
len_sig_ges =  floor(len_sig_ges/block_laenge)*block_laenge;
%Bestimmte Bitfolge
erg=[];
%Suche Startzeit, �berschreitet dieser Wert eine Millionen
count_min=0;
%Z�hle L�nge der Amplitudenabsenkung
count_zero = 0;
%Es muss gekl�rt werden, ob das letzte Bit der aktuellen Sequenz Teil einer
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

%Zum Debug Ploten
plotindex4096=1:4096;
plotindex8192=1:8192;

%Corr ergebnisse in Vektor speichern
num = 1;

for iter=1:block_laenge:len_sig_ges
    tic
%sig = sig(1:400000);
sig = sig_ges(iter:iter+block_laenge-1);
%figure, plot(sig), grid
%sig = [1 zeros(1,4095)];
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
        %H�llkurve bestimmen
%          figure,plot(detrend(phase(sig_I+1j*sig_Q)))
        sig = sqrt(sig_I.^2+sig_Q.^2);       
       
end



%%FFT nach Null geschoben
%SIG = fft(sig,NFFT)/lenSig;
%%%%%%%%%figure, plot(f,db(2*abs(SIG(1:NFFT/2+1)))), grid

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Schwellen detektieren
switch file
    case 'realData'
        sig_bin = dcf77_bit_decider(sig_bin, sig, 0.8, block_laenge);
    case 'simu'
        sig_bin = dcf77_bit_decider(sig_bin, sig, 0.2, block_laenge);
end
if(strcmp(option,'plot'))
figure,plot(plotindex4096,sig,plotindex4096,sig_bin),title('Signalausschnitt')
end
%%FFT nach Null geschoben
%%%SIG = fft(sig_bin,NFFT)/lenSig;
%%%%%%%%%%figure, plot(f,db(2*abs(SIG(1:NFFT/2+1)))), grid



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%H�llkurve abtasten

for i=1:lenSig
    if sig_bin(i) == 0
        count_zero = count_zero +1;
        count_min = 0;
        %Der Buffer f�r die PLL wird bereits mit Beginn der Absenkung bef�llt, so
        %hat die PLL etwas Zeit sich einzuschwingen.
        input_buffer_pll_re(ind_pll)= sig_I(i);
        input_buffer_pll_im(ind_pll)= sig_Q(i);
        
        ind_pll = ind_pll+1;
        input_buffer_pll_flag=1;
        
        %Handelt es sich gerade um das letzte Bit der aktuellen Sequenz
        if i==lenSig
            flag_absenkung=1;
        end
    else
        count_min = count_min +1;
        %Hat die Bef�llung des PLL Buffers begonnen, so muss diese
        %fortgesetzt werden, sobald die ABsenkung vorbei ist und das
        %Phasenrauschen beginnt.
        if input_buffer_pll_flag == 1
            input_buffer_pll_re(ind_pll)= sig_I(i);
            input_buffer_pll_im(ind_pll)= sig_Q(i);
            ind_pll = ind_pll+1;
        end
    end
    
    %Sobald der input_buffer_pll voll ist, muss die Bef�llung gestoppt
    %werden.
    if ind_pll == buffer_pll_len/2
        input_buffer_pll_flag=0;
        %Working und Inputbuffer wechseln.
        temp_re = working_buffer_pll_re;
        temp_im = working_buffer_pll_im;
        working_buffer_pll_re = input_buffer_pll_re;  
        working_buffer_pll_im = input_buffer_pll_im; 
        input_buffer_pll_re = temp_re;
        input_buffer_pll_im = temp_im;        
        %Bef�llen des neuen InputBuffers vorbereiten.
        ind_pll=1;
        %Starten der Berechnung des Phasenrauschens.
        calc_phase_noise = 1;
    end
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Neue Minute Markierung erkennen
% Fs = 5438,6Hz => 5438 Samples pro Sekunde. Wenn f�r >5438 Samples keine
% Nullen auftauchen, somuss es sich um den Minuten�bergang handeln. + 200 ist
% Sicherheitsabstand.
if count_min > round(Fs)+200
    disp(length(erg));
    if(strcmp(file,'simu'))
    disp(dcf77_bitdecoder(erg));  %TR
    end
    erg=[];    
    disp('NEUE MINUTE-----------------------');
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%L�nge der Absenkung detektieren -> Bits detektieren

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

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Phasenrauschen bestimmen, geht nur im Modus mit Hilberfilter

  if calc_phase_noise == 1 && strcmp(command,'hilbert')
     [working_buffer_pll_re] = dcf77_pll(working_buffer_pll_re, working_buffer_pll_im,Fs);
%        figure,plot(working_buffer_pll_re)
       %Es m�ssen nur die ersten block_laenge Werte entschieden werden. Die
       %Ausgabe kann inplace erfolgen.
      working_buffer_pll_rePLOT = dcf77_bit_decider(working_buffer_pll_re,working_buffer_pll_re,0.0,block_laenge); 
      
      %%%Suche Begin Phasenrauschen.
      % Dies beginnt bei Sample 1088, dies entspricht 200ms bei der
      % verwendeten ABtastfrequenz 200ms*1Sample/(1/Fs)=1088
      phase_noise_window=1088;
      
      if(strcmp(option,'plot'))
        figure,plot(plotindex8192,working_buffer_pll_re,plotindex8192,working_buffer_pll_rePLOT),title('working_buffer_pll_re')
      end
        working_buffer_pll_re=working_buffer_pll_rePLOT;
      
        p_out=dcf77_bit_sequence_fitted( Fs);
        
         
        corrErg=xcorr(working_buffer_pll_re(phase_noise_window+1:block_laenge)*2-1,p_out(1:block_laenge-phase_noise_window)*2-1);
        if(strcmp(option,'plot'))
        figure,plot(abs(corrErg)),title('corrErg')
        figure,plot(plotindex4096(1:block_laenge-phase_noise_window),working_buffer_pll_re(phase_noise_window+1:block_laenge)*2-1+0.5,...
        plotindex4096(1:block_laenge-phase_noise_window),(p_out(1:block_laenge-phase_noise_window)*2-1)),title('corr Bitfolgen')
        end
        
    % Maximum der KKF bestimmen. 
    [T_v_value,T_v_index] = max(abs(corrErg));
    corr_length=length(corrErg);
    %Mittelpunkt
    corr_mid = (corr_length-1)/2;
    diff_sample_count(num) = abs(T_v_index-corr_mid)
    diff_time(num) = diff_sample_count(num)*(1/Fs)
    
    num=num+1;
%         corrErg=xcorr(working_buffer_pll_re(1030:block_laenge)*2-1,p_out(1:block_laenge-1030)*2-1);
%         figure,plot(corrErg),title('corrErg')
%         figure,plot(plotindex4096(1:block_laenge-1030-101),working_buffer_pll_re(1031+101:block_laenge)*2-1+0.5,...
%         plotindex4096(1:block_laenge-1030-101),(p_out(1:block_laenge-1030-101)*2-1)),title('corr Bitfolgen')
        
      %Null Setzten, um neu zu bef�llen.
      calc_phase_noise=0; 
     % pause
  end
  toc
end
  disp(length(erg));
  


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