% dcf77_bit_sequence_fitted.m (TR 01-June-2015)
%
% Edited by: 
%
%                   Fits the DCF77 pseudo random bit sequence to a Sampling
%                   frequency higher than the frequency the bits natively
%                   change with.
%
%
%       fs_final            : goal sampling frequency
%       option              : 'write H-file' to generate H-file
%

function p_out=dcf77_bit_sequence_fitted( fs_final, option)


%Parameters
N           = 2^9;  %gem. DCF77 Standard
lcontrol    = 0;    %Control (Low active)
fs_native   = 1/( 793e-3/N );

f_factor        = fs_final/fs_native;
N_sequence_out  = floor(N*f_factor);
N_factor        = N_sequence_out / N;

N_out = 2^ceil( log2(N_sequence_out) ); %round up to next multiple of 2
p_out = zeros(N_out,1);

%Check parameters
if nargin~=2 || ~(ischar(option))
    option = 'default';
end

%generate pseudo random sequence
p_sequence = DCF77_bit_sequence(N, lcontrol);

%sample sequence to wanted frequency
step_multiplier_sum = 0;
m = 0;

for k=1:N
       
    if(step_multiplier_sum/k < N_factor)    %Decide way to round according to current factor of array length
        step_multiplier = ceil(N_factor);
    else
        step_multiplier = floor(N_factor);
    end
    
    step_multiplier_sum = step_multiplier_sum + step_multiplier; %Sum up multipliers of all steps
    
    for l=1:step_multiplier     
        m=m+1;                      %pointer for out-array
        
        p_out(m) = p_sequence(k);   %fill out array according to current multipier
    end
       
end

%---------------------------------------------------------------------------
% write to file !
if(strcmp(option,'write H-file'))
    % create header file and info
    filename = 'dcf77_bit_sequence_fitted.h';
    file_ID = fopen(filename, 'w');		% generate include-file
    fprintf(file_ID, '//------------------------------------------- \n');
    fprintf(file_ID, '// designed with -- dcf77_bit_sequence_fitted.m -- \n');
    fprintf(file_ID, '// Fs = %6.2f\n', fs_final );
    fprintf(file_ID, '//------------------------------------------- \n');

    fprintf(file_ID, '#define N_bit_sequence %d\n', length(p_out));
    fprintf(file_ID, '\n// DCF77 bit sequence\n');

    fprintf(file_ID,'short dcf77_bit_sequence []={\n');
    j = 0;
    for i= 1:length(p_out);
       fprintf(file_ID,' %1.0f,', p_out(i) );
       j = j + 1;
       if j >7
         fprintf(file_ID, '\n');
         j = 0;
      end
    end
    fprintf(file_ID,'};\n');
    fclose(file_ID);
end