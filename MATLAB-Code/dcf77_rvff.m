function [ data ] = dcf77_rvff( filename )
% cvmrvff.m 'read variable from file - float'
%
% Liest den Inhalt einer einer .txt-Datei und liefert diesen als Vektor zurück.
%
% Parameter:
% filename : Dateiname als String inklusive .txt Endung.
%
% Rueckgabewerte:
% data : Enthält den eingelesenen Vektor.

fid=fopen(filename,'r');
if(fid<0)
    error('Fehler beim Einlesen von Datei');
end
data = fscanf(fid,'%f');
fclose(fid);

end
