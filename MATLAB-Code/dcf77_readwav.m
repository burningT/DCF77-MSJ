function [ outfolder ] = dcf77_readwav(outpath, outfolder, wavfilename, wavdir )
% cvmreadwav.m
% Liest einen Datensatz aus einer .wav-Datei und schreibt diesen in entsprechende .txt-Dateien.
%
% Parameter:
% outpath : Pfad in den der Ordner mit den Daten gespeichert werden soll.
% Outfolder : Ordnername in den die .txt-Dateien gespeichert werden sollen.
% 'outfolder' wird ergänzt mit der Bufferlänge N 
% wavfilename : Dateiname der zu öffnenden .wav-Datei.
% wavdir : Pfad zur zu öffnenden .wav-Datei
%
% Rückgabewert:
% outfolder : Ordner indem die .txt-Dateien gespeichert sind

cd(wavdir);
[data,f_A] = wavread(wavfilename);

f_A=5438.596

% Datenbufferlänge N
N = 4096;

% Skalierungsfaktor short nach float
shoToFlo = 32768;

% Ordner anlegen
cd(outpath);
outfolder = sprintf('%s_N%d',outfolder,N);
mkdir(outfolder);
cd(outfolder);

x_n = transpose(data(:,1));

% Schreibe Daten in .txt-Dateien
% Datenbufferlänge N
dcf77_wvtf('N',N);
% Abtastfrequenz f_A
dcf77_wvtf('f_A',f_A);

% Daten auf signed short skalieren
x_n=round(x_n*shoToFlo);

dcf77_wvtfi('shoToFlo',shoToFlo);
dcf77_wvtfi('x_n_s',x_n);

% Speichere ersten Datenblock als .h-Datei zum Testen des C-Algorithmus auf dem DSP.
dcf77_wvtfih('inputDataCH1',x_n(1:N));
end