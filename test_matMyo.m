
 
 clear
 clc
 % f=myo_2c_sfunc;         % default one sample
 % f=myo_2c_sfunc(1);      % explicit one sample
 f=myo_2c_sfunc(10000);
 
  
% Input error cases
% [~,~] = myo_2c_sfunc; % too many outputs
% myo_2c_sfunc({}); % bad data type
% myo_2c_sfunc(''); % bad data type
% myo_2c_sfunc(struct); % bad data type
% myo_2c_sfunc(0); % bad value
% myo_2c_sfunc(-1); % bad value
% myo_2c_sfunc(1e8); % bad value

 
 %%
[~, ind]=unique(f.eCounter);  
f.emgData =f.emgData(ind, :); 

[~, ind]=unique(f.aCounter);  
f.accData =f.accData(ind, :); 

[~, ind]=unique(f.aCounter);  
f.gyroData =f.gyroData(ind, :); %  save fist_1
 
% myo poses; 0-unknown, 1-rest, 2- fist, 3- win, 4-wout, 5=fspread, 6- db
