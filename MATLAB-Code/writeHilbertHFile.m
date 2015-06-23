function writeHilbertHFile(sourceFileName, fileName,  filterName, Fs, fstop, fpass, N_FIR, b_FIR)
%writeHilbertHFile Writes coefficients to H-File

filename = [fileName '.h'];
file_ID = fopen(filename, 'w');		% generate include-file
fprintf(file_ID, '//------------------------------------------- \n');
fprintf(file_ID, '// designed with -- %s.m -- \n', sourceFileName);
fprintf(file_ID, '// Fs = %6.2f\n', Fs );
fprintf(file_ID, '// fstop = %6.2f\n', fstop);
fprintf(file_ID, '// fpass = %6.2f\n', fpass);
fprintf(file_ID, '// N_FIR = %d\n',  N_FIR);
fprintf(file_ID, '//------------------------------------------- \n');

fprintf(file_ID, '#define N_delays_FIR_%s %d\n', filterName, N_FIR);
fprintf(file_ID, '\n// DEC FILTER\n');
fprintf(file_ID, 'float H_filt_FIR_%s[N_delays_FIR_%s]; \n',filterName,filterName);

write_coeff_float(file_ID, filterName, b_FIR, length(b_FIR));

fclose(file_ID);
end

