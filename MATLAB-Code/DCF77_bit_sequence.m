function p = DCF77_bit_sequence(N, lcontrol)
%Erzeugt 1/0 Zahlenfolge
p = zeros(N,1);
if lcontrol == 0
    reg = zeros(9,1);
	reg(1)=1;
	for kx=1:N
        p(kx) = exor(reg(9), reg(5));
        reg(2:9)=reg(1:8);
        reg(1)=p(kx);
	end;
else
    p(128:160)=1;
    p(192:224)=1;
end; % if