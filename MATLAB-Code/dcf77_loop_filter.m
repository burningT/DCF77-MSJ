% dcf77_loop_filter.m (TB 27-Mai-2015)
%
% Edited by: 
%
%                   Loop filter for the usage in the PLL
%                   
%
%       inp         : Input signal (result of Phase Detector)
%       delay       : delays states initial/final
%       out         : Filtered output signal
%       alpha       : 
%       beta        :
%

function [out, delay] = dcf77_loop_filter( inp, delay, alpha, beta)

siglen = length(inp);
out=zeros(1,siglen);

for k=1:siglen    
    [up, delay] = filter([beta], [1 -1],inp(k),delay);
    down= inp(k) * alpha;
    out(k)= up + down;
end

end