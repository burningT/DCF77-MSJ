function y = exor(a, b)
if a==0 & b==0
    y=0;
elseif a==0 & b==1
    y=1;
elseif a==1 & b==0
    y=1;
elseif a==1 & b==1
    y=0;
else
    disp('EXOR : error');
end; % if
    
    