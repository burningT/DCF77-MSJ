% dcf77_bitdecoder.m (TR 06-Mai-2015)
%
% Edited by: 
%
%                   Decoder für die Bitfolge des DCF77-Signals
%                     Dekodiert die Bitfolge einer Minute des DCF77-Signals
%
%
%       freq            : Bitfolge einer Minute (59 Werte erwartet)
%

function dateTimeString=dcf77_bitdecoder( rawArray );

rawArray_len 	= 59;
year_offset		= 2000;

%Check for expected array length
if length(rawArray) ~= rawArray_len
	error('Expected array of length %d but received array if length %d',...
        rawArray_len,length(rawArray))
end

%Organize bits
bit_R 	= rawArray(16);	%Bit 15
bit_A1 	= rawArray(17);	%Bit 16
bit_Z1 	= rawArray(18);	%Bit 17
bit_Z2 	= rawArray(19);	%Bit 18
bit_A2 	= rawArray(20);	%Bit 19
bit_S 	= rawArray(21);	%Bit 20

val_min	= rawArray(22)+2*rawArray(23)+4*rawArray(24)+8*rawArray(25)+...
    10*rawArray(26)+20*rawArray(27)+40*rawArray(28);
sum_min = rawArray(22)+rawArray(23)+rawArray(24)+rawArray(25)+rawArray(26)+...
    rawArray(27)+rawArray(28);
bit_P1	= rawArray(29);	

val_h	= rawArray(30)+2*rawArray(31)+4*rawArray(32)+8*rawArray(33)+...
    10*rawArray(34)+20*rawArray(35);
sum_h	= rawArray(30)+rawArray(31)+rawArray(32)+rawArray(33)+rawArray(34)+...
    rawArray(35);
bit_P2	= rawArray(36);

val_d	= rawArray(37)+2*rawArray(38)+4*rawArray(39)+8*rawArray(40)+...
    10*rawArray(41)+20*rawArray(42);
sum_d	= rawArray(37)+rawArray(38)+rawArray(39)+rawArray(40)+rawArray(41)+...
    rawArray(42);

val_wd	= rawArray(43)+2*rawArray(44)+4*rawArray(45);
sum_wd	= rawArray(43)+rawArray(44)+rawArray(45);

val_m	= rawArray(46)+2*rawArray(47)+4*rawArray(48)+8*rawArray(49)+...
    10*rawArray(50);
sum_m	= rawArray(46)+rawArray(47)+rawArray(48)+rawArray(49)+rawArray(50);

val_y	= year_offset+rawArray(51)+2*rawArray(52)+4*rawArray(53)+...
    8*rawArray(54)+10*rawArray(55)+20*rawArray(56)+40*rawArray(57)+80*rawArray(58);
sum_y	= rawArray(51)+rawArray(52)+rawArray(53)+rawArray(54)+rawArray(55)+...
    rawArray(56)+rawArray(57)+rawArray(58);
bit_P3	= rawArray(59);

%Check for parity and skip if error found
checksum = sum_min+bit_P1 +sum_h+bit_P2 +sum_d+sum_wd+sum_m+sum_y+bit_P3;

if mod(checksum,2) > 0
	error('Invalid Data')
end

%Generate String for Output
dateTimeString = [int2str(val_h) ':'];                       %Hours

dateTimeString = [dateTimeString int2str(val_min) ' Uhr'];   %Minutes

switch val_wd	%Monday is first day of the week             %Weekday
   case 1
      dateTimeString = [dateTimeString ', Montag '];
   case 2
      dateTimeString = [dateTimeString ', Dienstag '];
   case 3
      dateTimeString = [dateTimeString ', Mittwoch '];
   case 4
      dateTimeString = [dateTimeString ', Donnerstag '];
   case 5
      dateTimeString = [dateTimeString ', Freitag '];
   case 6
      dateTimeString = [dateTimeString ', Samstag '];
   case 7
      dateTimeString = [dateTimeString ', Sonntag '];
end

dateTimeString = [dateTimeString int2str(val_d) '.'];       %Day

dateTimeString = [dateTimeString int2str(val_m),'.'];       %Month

dateTimeString = [dateTimeString int2str(val_y)];           %Year