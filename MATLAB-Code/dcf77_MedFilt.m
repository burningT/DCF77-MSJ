function [ output ] = dcf77_MedFilt( input ,LEN)
% cvmMedfilt.m
%
% Gibt den Median der Elemente aus input zurück.
%
% Parameter:
% input : Geschwindigkeitseingangsvektor, muss LEN Werte enthalten.
% LEN : Filterlänge, ungerade Zahl.
%
% Rückgabewert:
% outut : Geschwindigkeitswert.

% Sortiere 'input'.
input = sort(input);
% Gebe den Median zurück.
output = input((LEN+1)/2);

end
