 
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <array>
#include <sstream>
#include <stdexcept>
#include <string>
#include <fstream>
#include <time.h>

// #include "stdafx.h"

#include <myo.hpp>

struct Frame
{
	double roll, pitch, yaw;
	bool onArm, isUnlocked, whichArm;
	myo::Pose currentPose;
	//myo::Arm whichArm;
	//     std::string whichArm;
	myo::Vector3<float> accData, gyroData;
	std::array<int8_t, 8> emgData;
	int wPose;
    int ecounter=0;
    int acounter=0;
    int gcounter=0;

};

// default behavior is to do nothing.
class DataCollector : public myo::DeviceListener
{
private:
	Frame currentFrame;
public:
	DataCollector()
		: onArm(false), isUnlocked(true), roll_w(0), pitch_w(0), yaw_w(0), currentPose(),
		emgData(), accData(), gyroData(), wPose()
	{
	}

	~DataCollector()
	{
	}

	void onConnect(myo::Myo *myo, uint64_t timestamp, myo::FirmwareVersion firmwareVersion) {
		//Reneable streaming
		myo->setStreamEmg(myo::Myo::streamEmgEnabled);
// 		openFiles();
	}

	// onUnpair() is called whenever the Myo is disconnected from Myo Connect by the user.
	void onUnpair(myo::Myo* myo, uint64_t timestamp)
	{
		// We've lost a Myo.
		// Let's clean up some leftover state.
		roll_w = 0;
		pitch_w = 0;
		yaw_w = 0;
		onArm = false;
		isUnlocked = false;
		emgData.fill(0);


	}
	// onOrientationData() is called whenever the Myo device provides its current orientation, which is represented
	// as a unit quaternion.
	void onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat)
	{
		using std::atan2;
		using std::asin;
		using std::sqrt;
		using std::max;
		using std::min;
		// Calculate Euler angles (roll, pitch, and yaw) from the unit quaternion.
		float roll = atan2(2.0f * (quat.w() * quat.x() + quat.y() * quat.z()),
			1.0f - 2.0f * (quat.x() * quat.x() + quat.y() * quat.y()));
		float pitch = asin(max(-1.0f, min(1.0f, 2.0f * (quat.w() * quat.y() - quat.z() * quat.x()))));
		float yaw = atan2(2.0f * (quat.w() * quat.z() + quat.x() * quat.y()),
			1.0f - 2.0f * (quat.y() * quat.y() + quat.z() * quat.z()));
		// Convert the floating point angles in radians to a scale from 0 to 18.
		roll_w = ((roll + (float)M_PI) / (M_PI * 2.0f));
		pitch_w = ((pitch + (float)M_PI / 2.0f) / M_PI);
		yaw_w = ((yaw + (float)M_PI) / (M_PI * 2.0f));
	}
	// onPose() is called whenever the Myo detects that the person wearing it has changed their pose, for example,
	// making a fist, or not making a fist anymore.
	void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose)
	{
		currentPose = pose;
		myo->unlock(myo::Myo::unlockHold);

		if (pose == myo::Pose::unknown)
		{
			wPose = 0;
		}

		if (pose == myo::Pose::rest)
		{
			wPose = 1;
		}

		if (pose == myo::Pose::fist)
		{
			wPose = 2;
		}

		if (pose == myo::Pose::waveIn)
		{
			wPose = 3;
		}

		if (pose == myo::Pose::waveOut)
		{
			wPose = 4;
		}

		if (pose == myo::Pose::fingersSpread)
		{
			wPose = 5;
		}

		if (pose == myo::Pose::doubleTap)
		{
			wPose = 6;
		}
 
	}
 
	void onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm, myo::XDirection xDirection)
	{
		onArm = true;
		whichArm = arm;
		swhichArm = (whichArm == myo::armLeft ? false : true);
	}
 
	void onArmUnsync(myo::Myo* myo, uint64_t timestamp)
	{
		onArm = false;
	}
 
	void onUnlock(myo::Myo* myo, uint64_t timestamp)
	{
		isUnlocked = true;
	}
 
	void onLock(myo::Myo* myo, uint64_t timestamp)
	{
		isUnlocked = false;
	}

	// Get EMG Samples
	void onEmgData(myo::Myo* myo, uint64_t timestamp, const int8_t  *emg)
	{

// 		emgFile << timestamp;
		for (int i = 0; i < 8; i++) {
			emgData[i] = emg[i];
// 			emgFile << ',' << static_cast<int>(emg[i]);
		}
        ecounter++;
// 		printVector(emgFile, accData);
// 		printVector(emgFile, gyroData);
// 		emgFile << std::endl;
	}
	void onAccelerometerData(myo::Myo * myo, uint64_t timestamp, const myo::Vector3< float > & accel)
	{
		accData = accel;
        acounter++;
	}

	void onGyroscopeData(myo::Myo * myo, uint64_t timestamp, const myo::Vector3< float > & gyro)
	{
		gyroData = gyro;
        gcounter++;
	}
 

	// These values are set by onArmSync() and onArmUnsync() above.
	bool onArm;
	myo::Arm whichArm;
	bool swhichArm;

	// This is set by onUnlocked() and onLocked() above.
	bool isUnlocked;
	// These values are set by onOrientationData() and onPose() above.
	double roll_w, pitch_w, yaw_w;
	int wPose;
	myo::Pose currentPose;

	// The values of this array is set by onEmgData() above.
	std::array<int8_t, 8> emgData;
	myo::Vector3<float> accData;
	myo::Vector3<float > gyroData;
    
    int ecounter=0;
    int acounter=0;
    int gcounter=0;

	const Frame &getFrame()
	{
		currentFrame.roll = roll_w;
		currentFrame.pitch = pitch_w;
		currentFrame.yaw = yaw_w;
		currentFrame.accData = accData;
		currentFrame.gyroData = gyroData;
		currentFrame.emgData = emgData;
		currentFrame.onArm = onArm;
		currentFrame.whichArm = swhichArm;
		currentFrame.isUnlocked = isUnlocked;
		currentFrame.wPose = wPose;
        currentFrame.ecounter = ecounter;
        currentFrame.gcounter = gcounter;
        currentFrame.acounter = acounter;
		return currentFrame;
	}

	void openFiles() {
		time_t timestamp = std::time(0);

		// Open file for EMG log
		if (emgFile.is_open()) {
			emgFile.close();
		}
		std::ostringstream emgFileString;
		emgFileString << "emg-" << timestamp << ".csv";
		emgFile.open(emgFileString.str(), std::ios::out);
		emgFile << "timestamp,emg1,emg2,emg3,emg4,emg5,emg6,emg7,emg8, ax, ay, az, gx, gy, gz" << std::endl;

	 	}
 

	void printVector(std::ofstream &file,  const myo::Vector3< float > &vector) {
			
		file << ',' << vector.x()
			<< ',' << vector.y()
			<< ',' << vector.z();
			 
	}
	// The files we are logging to
	std::ofstream emgFile;
 
};

 



