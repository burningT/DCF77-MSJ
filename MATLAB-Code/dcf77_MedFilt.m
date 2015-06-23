function [ output ] = dcf77_MedFilt( input ,LEN)
% cvmMedfilt.m
%
% Gibt den Median der Elemente aus input zur�ck.
%
% Parameter:
% input : Geschwindigkeitseingangsvektor, muss LEN Werte enthalten.
% LEN : Filterl�nge, ungerade Zahl.
%
% R�ckgabewert:
% outut : Geschwindigkeitswert.

% Sortiere 'input'.
input = sort(input);
% Gebe den Median zur�ck.
output = input((LEN+1)/2);

end
