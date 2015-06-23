function [ ] = dcf77_wvtfih( varname, var )
% cvmwvtfih.m 'write variable to file - integer header'
%
% Schreibt eine Variable in eine .txt-Datei als Integer mit Kommata am Ende.
% Wird zum erstellen der .h-Dateien verwendet.
% 
% Parameter:
% varname - Variablenname als String. Bildet den Dateinamen.
% var - Der Vektor oder Skalar der in die Datei geschrieben wird.

% Filename erzeugen
filename= sprintf('%s%s',varname,'.h');

fid=fopen(filename,'wt');
if(fid<0)
    error('Fehler beim Datei erzeugen');
end
fprintf(fid,'%d,',var);
fseek(fid,-1,'eof');
fprintf(fid,'\n');
fclose(fid);


end

