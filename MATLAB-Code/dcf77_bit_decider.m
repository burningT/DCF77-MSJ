% dcf77_bit_decider.m (TB 03-June-2015)
%
% Edited by: 
%
%                   Function for binary decision.
%                   
%
%       sig_bin         : Buffer for the Signal after the decision.
%                           Use thereference to avoid additional
%                           memory allocation
%       sig             : The input signal, shall have the same length as
%                           sig_bin
%       threshold       : Threshhold for the decision
%       lenSig          : Length of sig and sig_bin

function [sig_bin] = dcf77_bit_decider(sig_bin, sig, threshold,lenSig)

for i=1:lenSig
    if sig(i) < threshold
        sig_bin(i) = 0;
    else
        sig_bin(i) = 1;
    end
end
