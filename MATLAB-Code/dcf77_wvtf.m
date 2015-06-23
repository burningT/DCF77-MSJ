function [ ] = dcf77_wvtf( varname, var )
% cvmwvtf.m 'write variable to file - float'
%
% Schreibt eine Variable als float-Werte in eine .txt-Datei.
%
% Parameter:
% varname : Variablenname als String. Bildet den Dateinamen.
% var : Der Vektor oder Skalar der in die Datei geschrieben wird.

% Filename erzeugen
filename= sprintf('%s%s',varname,'.txt');

fid=fopen(filename,'wt');
if(fid<0)
    error('Fehler beim Datei erzeugen');
end
fprintf(fid,'%f\n',var);
fclose(fid);

end

