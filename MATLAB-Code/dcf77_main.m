function [diff_sample_count, diff_time] = dcf77_main(file,command,option)
close all, clc
%[diff_sample_count, diff_time] = dcf77_main('simu','volksempfaenger','plot')
%[diff_sample_count, diff_time] = dcf77_main('simu','hilbert','plot')
%[diff_sample_count, diff_time] = dcf77_main('realData','volksempfaenger','plot')
%[diff_sample_count, diff_time] = dcf77_main('realData','hilbert','plot')

%[diff_sample_count, diff_time] = dcf77_main('simu','volksempfaenger','')
%[diff_sample_count, diff_time] = dcf77_main('simu','hilbert','')
%[diff_sample_count, diff_time] = dcf77_main('realData','volksempfaenger','')
%[diff_sample_count, diff_time] = dcf77_main('realData','hilbert','')

%Zum generieren von H-Files: 'write H-file'
%[diff_sample_count, diff_time] = dcf77_main([?], [?], 'write H-file')

%Fs=5438.596; %Hz

%%Signal laden
switch file
    case 'realData'
    %[sig_ges,f_A] = wavread('B48_UPV_Feature_DModC6713_plus_ADDA16_EDMA_SDRAM_5438_Adler.wav');
    %pwd = aktueller Ordner
    outpath = pwd;
    outfolder = 'test1';
    amplifie = 8;
    %wavfilename = 'B48_UPV_Feature_DModC6713_plus_ADDA16_EDMA_SDRAM_5438_Adler.wav';
    wavfilename = 'B48_UPV_Feature_DModC6713_plus_ADDA16_EDMA_SDRAM_Fs_5438_59649_Now_no_dropout.wav';
    
    wavdir = pwd;
    [ outfolder ] = dcf77_readwav(outpath, outfolder, wavfilename, wavdir )
    
    block_laenge = dcf77_rvff('N.txt');
    shoToFlo = dcf77_rvff('shoToFlo.txt');
    x_n = dcf77_rvff('x_n_s.txt');
    % Skaliere float-Werte
    x_n = x_n/shoToFlo;
    Fs= dcf77_rvff('f_A.txt');
    sig_ges=x_n*amplifie;
   
    case 'simu'
    sig_ges =  load('gen_DCF77_several_minutes.mat','-ascii');
end
 figure,plot(sig_ges),title('Gesamtsignal')
len_sig_ges = length(sig_ges);
%block_laenge = 4096;
buffer_pll_len = 2*block_laenge;
buffer_pll_re = zeros(buffer_pll_len,1);
buffer_pll_im = zeros(buffer_pll_len,1);
%Index für die Befüllung des Buffers der PLL. Matlab Indizierung startet
%bei 1!
ind_pll=1;
%Gibt an das der Input Buffer mit Werten gefüllt wird.
input_buffer_pll_flag=0;
%Startet die Berechnung des Phasenrauschens.
calc_phase_noise=0;

%Buffer für den Entscheider
sig_bin = zeros(block_laenge,1);

bit_count = 0;

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

%Schreibe H-File
if(strcmp(option,'write H-file'))
    writeHilbertHFile('dcf77_main', 'dcf77_hilbert',  'hilbert', Fs, .4*Fs, .6*Fs, N, b_FIR_hilber)
    pause
end

%Zum Debug Ploten
plotindex4096=1:4096;
plotindex8192=1:8192;

%Corr ergebnisse in Vektor speichern
num = 1;

for iter=1:block_laenge:len_sig_ges
   % tic
sig = sig_ges(iter:iter+block_laenge-1);
lenSig=length(sig);
 %Handelt es sich gerade um das letzte Bit der aktuellen Sequenz und befindet sich der Signalausschnitt in der Absenkung     
flag_absenkung = 0;

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
       
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Schwellen detektieren
len_median = 7;
switch file
    case 'realData'
        sig_bin = dcf77_bit_decider(sig_bin, sig, 0.8, block_laenge);
          for k = 1 : length(sig_bin)-len_median 
          sig_bin(k) = dcf77_MedFilt(sig_bin(k:k+len_median),len_median);
          end
    case 'simu'
        sig_bin = dcf77_bit_decider(sig_bin, sig, 0.2, block_laenge);
end
if(strcmp(option,'plot'))
figure,plot(plotindex4096,sig,plotindex4096,sig_bin),title('Signalausschnitt')
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Hüllkurve abtasten

for i=1:lenSig
    if sig_bin(i) == 0
        count_zero = count_zero +1;
        count_min = 0;
        %Der Buffer für die PLL wird bereits mit Beginn der Absenkung befüllt, so
        %hat die PLL etwas Zeit sich einzuschwingen.
        buffer_pll_re(ind_pll)= sig_I(i);
        buffer_pll_im(ind_pll)= sig_Q(i);
        
        input_buffer_pll_flag=1;
        
        %Handelt es sich gerade um das letzte Bit der aktuellen Sequenz
        if i==lenSig
            flag_absenkung=1;
        end
    else
        count_min = count_min +1;
        %Hat die Befüllung des PLL Buffers begonnen, so muss diese
        %fortgesetzt werden, sobald die ABsenkung vorbei ist und das
        %Phasenrauschen beginnt.
        if input_buffer_pll_flag == 1
            buffer_pll_re(ind_pll)= sig_I(i);
            buffer_pll_im(ind_pll)= sig_Q(i);
            
        end
    end
    
    %Sobald der input_buffer_pll voll ist, muss die Befüllung gestoppt
    %werden.
    if ind_pll == buffer_pll_len/2
        input_buffer_pll_flag=0;      
        %Befüllen des neuen InputBuffers vorbereiten.
        ind_pll=1;
        %Starten der Berechnung des Phasenrauschens.
        calc_phase_noise = 1;
    end
    if input_buffer_pll_flag == 1
        ind_pll = ind_pll+1;
    end
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Neue Minute Markierung erkennen
% Fs = 5438,6Hz => 5438 Samples pro Sekunde. Wenn für >5438 Samples keine
% Nullen auftauchen, somuss es sich um den Minutenübergang handeln. + 200 ist
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
%Länge der Absenkung detektieren -> Bits detektieren

%564 entspricht, 600 schafft etwas Toleranz
%                100ms, Logisch 0
%                200ms, Logisch 1

disp(['bit_count= ' num2str(bit_count) ' bit_count= ' num2str(count_zero)])

if count_zero < 600 && count_zero > 50 && flag_absenkung == 0
    %disp('Null entdeckt');
    erg=[erg(:)',0];
    %Bit erkannt, wieder auf Null setzen
    count_zero = 0;
    
bit_count = bit_count+1;
elseif (count_zero > 600 && flag_absenkung == 0)%200ms
     %disp('-- Eins entdeckt');
     erg=[erg(:)',1];
     %Bit erkannt, wieder auf Null setzen
    count_zero = 0;
    
bit_count = bit_count+1;
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Phasenrauschen bestimmen, geht nur im Modus mit Hilberfilter

  if calc_phase_noise == 1 && strcmp(command,'hilbert')
     [buffer_pll_re] = dcf77_pll(buffer_pll_re, buffer_pll_im,Fs);
%        figure,plot(buffer_pll_re)
       %Es müssen nur die ersten block_laenge Werte entschieden werden. Die
       %Ausgabe kann inplace erfolgen.
      buffer_pll_rePLOT = dcf77_bit_decider(buffer_pll_re,buffer_pll_re,0.0,block_laenge); 
      
      %%%Suche Begin Phasenrauschen.
      % Dies beginnt bei Sample 1088, dies entspricht 200ms bei der
      % verwendeten ABtastfrequenz 200ms*1Sample/(1/Fs)=1088
      phase_noise_window=1088;
      
      if(strcmp(option,'plot'))
        figure,plot(plotindex8192,buffer_pll_re,plotindex8192,buffer_pll_rePLOT),title('buffer_pll_re')
      end
        buffer_pll_re=buffer_pll_rePLOT;
      
        p_out=dcf77_bit_sequence_fitted( Fs);
        
         
        corrErg=xcorr(buffer_pll_re(phase_noise_window+1:block_laenge)*2-1,p_out(1:block_laenge-phase_noise_window)*2-1);
        if(strcmp(option,'plot'))
        figure,plot(abs(corrErg)),title('corrErg')
        figure,plot(plotindex4096(1:block_laenge-phase_noise_window),buffer_pll_re(phase_noise_window+1:block_laenge)*2-1+0.5,...
        plotindex4096(1:block_laenge-phase_noise_window),(p_out(1:block_laenge-phase_noise_window)*2-1)),title('corr Bitfolgen')
        end
        
    % Maximum der KKF bestimmen. 
    [T_v_value,T_v_index] = max(abs(corrErg));
    corr_length=length(corrErg);
    %Mittelpunkt
    corr_mid = (corr_length-1)/2;
    %Abweichung bestimmen und Delay vom Median Filter abziehen.
    diff_sample_count(num) = abs(T_v_index-corr_mid)-floor(len_median/2)-1;
    diff_time(num) = diff_sample_count(num)*(1/Fs);
    
    num=num+1;
        
      %Null Setzten, um neu zu befüllen.
      calc_phase_noise=0; 
     % pause
  end
 % toc
end
  disp(length(erg));
 
end