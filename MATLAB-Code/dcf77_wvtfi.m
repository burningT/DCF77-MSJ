function [ ] = dcf77_wvtfi( varname, var )
% cvmwvtfi.m 'write variable to file - integer'
%
% Schreibt eine Variable als Integer-Werte in eine .txt-Datei.
%
% Parameter:
% varname - Variablenname als String. Bildet den Dateinamen.
% var - Der Vektor oder Skalar der in die Datei geschrieben wird.

% Filename erzeugen
filename= sprintf('%s%s',varname,'.txt');

fid=fopen(filename,'wt');
if(fid<0)
    error('Fehler beim Datei erzeugen');
end
fprintf(fid,'%d\n',var);
fclose(fid);

end

