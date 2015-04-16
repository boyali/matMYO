
% Check for sdk

clc
clear

if exist('MYO_SDK')~=7
    error('The "MYO_SDK" folder cannot be found.  A link with this name should point to the directory that contains the SDK.');
end

% determine where the MYO library is located
if ismac
    libdir_switch='-L./MYO_SDK/lib';
elseif isunix || ispc
    % check register size
         libdir_switch='-L./MYO_SDK/lib/'; 
else
    error('Unknown operating system');
end


% create the mex command line
% fn='myo_2a.cpp';
fn='myo_2c_sfunc.cpp';
fprintf('Compiling %s\n',fn);
cmd=['mex',...
    ' -I./MYO_SDK/include ',...
    libdir_switch,...
    ' -lmyo64 ',...
    fn];
fprintf('Evaluating "%s"\n',cmd)

% run mex
eval(cmd)

fprintf('Done\n')