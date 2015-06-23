% dcf77_vco:Unit_Test.m (TR 27-Mai-2015)
%
% Edited by: 
%
%                   Unit test for Voltage Controlled Oscillator
%
%

Fs = 5438.6;
Ts = 1/Fs;
wc = 2*pi*Fs/4/2;
Kv = 100;
Zi = 0;

testArray = [ones(1,50)*0 ones(1,50)*pi]; %Create Test array with jump

disp(['Test startet mit: '])
disp(['   Fs:   ',num2str(Fs)])
disp(['   Ts:   ',num2str(Ts)])
disp(['   wc:   ',num2str(wc)])
disp(['   Kv:   ',num2str(Kv)])
disp(['   Zi:   ',num2str(Zi)])

[vi, vq, zf] = dcf77_vco( testArray , Ts, wc, Kv, Zi);

if not((length(vi) == length(testArray)) && (length(vq) == length(testArray)))
    disp(['Die Längen der Vektoren stimmen nicht überein:   ',num2str([r1 r2 r3])])
    disp(['   length(testArray):   ',num2str(length(testArray))])
    disp(['   length(vi):   ',num2str(length(vi))])
    disp(['   length(vq):   ',num2str(length(vq))])
end

plot(vi); 
hold on;
plot(vq);
plot(testArray);
grid;
legend('V_I','V_Q','VCO Input (Phase)');
hold off;

disp(['Test abgeschlossen!'])