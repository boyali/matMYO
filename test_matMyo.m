
 
 clear
 clc
 f=myo_2c_sfunc ;
 
 
 
 %%
[~, ind]=unique(f.eCounter);  
f.emgData =f.emgData(ind, :); 

[~, ind]=unique(f.aCounter);  
f.accData =f.accData(ind, :); 

[~, ind]=unique(f.aCounter);  
f.gyroData =f.gyroData(ind, :); %  save fist_1
 
% myo poses; 0-unknown, 1-rest, 2- fist, 3- win, 4-wout, 5=fspread, 6- db
