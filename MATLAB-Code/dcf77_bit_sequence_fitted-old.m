% dcf77_bit_sequence_fitted.m (TR 01-June-2015)
%
% Edited by: 
%
%                   Decoder für die Bitfolge des DCF77-Signals
%                     Dekodiert die Bitfolge einer Minute des DCF77-Signals
%
%
%       freq            : Bitfolge einer Minute (59 Werte erwartet)
%

function p_sequence=dcf77_bit_sequence_fitted( fs_final )


%Parameters
N          = 2^9;  %gem. DCF77 Standard
lcontrol    = 0;    %Control (Low active)
fs_native   = 1/( 739e-3/N );

%generate SRC
src = dsp.SampleRateConverter('InputSampleRate',fs_native,...
                              'OutputSampleRate',fs_final,...
                              'Bandwidth',fs_native-10);

%generate pseudo random sequence
p_sequence = DCF77_bit_sequence(N, lcontrol);

%sample sequence to wanted frequency
[L,M] = getRateChangeFactors(src);

N_BitsToAdd = M-mod(length(p_sequence),M);

if(N_BitsToAdd ~= M )
    p_sequence  = [p_sequence; zeros(N_BitsToAdd,1)];
end

p_sequence = step(src, p_sequence);