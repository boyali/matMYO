% MYO_2C_SFUNC(NUM)  Poll Myo for NUM samples
%   This MEX function collects Myo data by polling with best-effort
%   approach to collect NUM samples. Data is returned in a single struct
%   output variable.
%
% Usage:
%   F = myo_2c_sfunc()
%     Returns one data sample from Myo in output struct F.
%
%   F = myo_2c_sfunc(num)
%     Same as above, but returns num samples in scalar output struct F.
%     Note that if input num is not provided, the default is 1.
%
% Inputs
%   num (optional)
%     Number of samples to collect from Myo. The default value is 1.
%
% Outputs
%   F
%     Structure containing vectors of Myo data samples with the following
%     fields
%   F.Roll        TODO
%   F.Pitch       TODO
%   F.Yaw         TODO
%   F.accData     TODO
%   F.gyroData    TODO
%   F.emgData     TODO
%   F.onArm       TODO
%   F.isUnlocked  TODO
%   F.onWhichArm  TODO
%   F.whichPose   TODO
%   F.eCounter    TODO
%   F.aCounter    TODO
%   F.gCounter    TODO
