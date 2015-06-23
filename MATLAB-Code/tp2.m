function y = tp2(x)
%TP2 Filters input x and returns output y.

% MATLAB Code
% Generated by MATLAB(R) 8.4 and the DSP System Toolbox 8.7.
% Generated on: 30-Apr-2015 17:21:09

%#codegen

% To generate C/C++ code from this function use the codegen command. Type
% 'help codegen' for more information.

persistent Hd;

if isempty(Hd)
    
    % The following code was used to design the filter coefficients:
    % % Equiripple Lowpass filter designed using the FIRPM function.
    %
    % % All frequency values are in Hz.
    % Fs = 5438.596;  % Sampling Frequency
    %
    % Fpass = 300;             % Passband Frequency
    % Fstop = 800;             % Stopband Frequency
    % Dpass = 0.057501127785;  % Passband Ripple
    % Dstop = 0.0001;          % Stopband Attenuation
    % dens  = 20;              % Density Factor
    %
    % % Calculate the order from the parameters using FIRPMORD.
    % [N, Fo, Ao, W] = firpmord([Fpass, Fstop]/(Fs/2), [1 0], [Dpass, Dstop]);
    %
    % % Calculate the coefficients using the FIRPM function.
    % b  = firpm(N, Fo, Ao, W, {dens});
    
    Hd = dsp.FIRFilter( ...
        'Numerator', [-0.000628086285008614 -0.00214580352442941 ...
        -0.00496996944304779 -0.00889584686448567 -0.0128150490747414 ...
        -0.0145616997384995 -0.0112175161442663 8.0234968760173e-05 ...
        0.0210120302965356 0.0510229651338714 0.0868103792004036 ...
        0.122689203290725 0.151860537503812 0.16824044481147 0.16824044481147 ...
        0.151860537503812 0.122689203290725 0.0868103792004036 ...
        0.0510229651338714 0.0210120302965356 8.0234968760173e-05 ...
        -0.0112175161442663 -0.0145616997384995 -0.0128150490747414 ...
        -0.00889584686448567 -0.00496996944304779 -0.00214580352442941 ...
        -0.000628086285008614]);
end

y = step(Hd,x);


% [EOF]