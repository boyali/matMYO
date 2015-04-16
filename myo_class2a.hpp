
#ifndef myo_class2a_hpp
#define myo_class2a_hpp
#endif
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <array>

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

};

// default behavior is to do nothing.
class DataCollector : public myo::DeviceListener 
{
private:
    Frame currentFrame;
public:
    DataCollector()
    : onArm(false), isUnlocked(true), roll_w(0), pitch_w(0), yaw_w(0), currentPose(),
            emgData(), accData(), gyroData()
    {
    }
    
    ~DataCollector()
    {
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
        roll_w =  ((roll + (float)M_PI)/(M_PI * 2.0f));
        pitch_w =  ((pitch + (float)M_PI/2.0f)/M_PI);
        yaw_w =  ((yaw + (float)M_PI)/(M_PI * 2.0f));
    }
    // onPose() is called whenever the Myo detects that the person wearing it has changed their pose, for example,
    // making a fist, or not making a fist anymore.
    void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose)
    {
        currentPose = pose;
        myo->unlock(myo::Myo::unlockHold);
//         if (pose != myo::Pose::unknown && pose != myo::Pose::rest) {
//             // Tell the Myo to stay unlocked until told otherwise. We do that here so you can hold the poses without the
//             // Myo becoming locked.
//             myo->unlock(myo::Myo::unlockHold);
//             // Notify the Myo that the pose has resulted in an action, in this case changing
//             // the text on the screen. The Myo will vibrate.
//             //myo->notifyUserAction();
//         } else {
//             // Tell the Myo to stay unlocked only for a short period. This allows the Myo to stay unlocked while poses
//             // are being performed, but lock after inactivity.
//             //myo->unlock(myo::Myo::unlockTimed);
//         }
    }
    // onArmSync() is called whenever Myo has recognized a Sync Gesture after someone has put it on their
    // arm. This lets Myo know which arm it's on and which way it's facing.
    void onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm, myo::XDirection xDirection)
    {
        onArm = true;
        whichArm = arm;
        swhichArm=(whichArm == myo::armLeft ? false: true);
    }
    // onArmUnsync() is called whenever Myo has detected that it was moved from a stable position on a person's arm after
    // it recognized the arm. Typically this happens when someone takes Myo off of their arm, but it can also happen
    // when Myo is moved around on the arm.
    void onArmUnsync(myo::Myo* myo, uint64_t timestamp)
    {
        onArm = false;
    }
    // onUnlock() is called whenever Myo has become unlocked, and will start delivering pose events.
    void onUnlock(myo::Myo* myo, uint64_t timestamp)
    {
        isUnlocked = true;
    }
    // onLock() is called whenever Myo has become locked. No pose events will be sent until the Myo is unlocked again.
    void onLock(myo::Myo* myo, uint64_t timestamp)
    {
        isUnlocked = false;
    }
    
    // Get EMG Samples
    void onEmgData(myo::Myo* myo, uint64_t timestamp, const int8_t  *emg)
    {
        for (int i = 0; i < 8; i++) {
            emgData[i] = emg[i];
        }
    }    
    void onAccelerometerData (myo::Myo * myo, uint64_t timestamp, const myo::Vector3< float > & accel)
    {
        accData=accel;
    }
    
    void onGyroscopeData (myo::Myo * myo, uint64_t timestamp, const myo::Vector3< float > & gyro )
    {
        gyroData=gyro;
    }
    // There are other virtual functions in DeviceListener that we could override here, like onAccelerometerData().
    // For this example, the functions overridden above are sufficient.
    // We define this function to print the current values that were updated by the on...() functions above.
   
    // These values are set by onArmSync() and onArmUnsync() above.
    bool onArm;
    myo::Arm whichArm;
    bool swhichArm; 
    
    // This is set by onUnlocked() and onLocked() above.
    bool isUnlocked;
    // These values are set by onOrientationData() and onPose() above.
    double roll_w, pitch_w, yaw_w;
    myo::Pose currentPose;
    
    // The values of this array is set by onEmgData() above.
    std::array<int8_t, 8> emgData;
    myo::Vector3<float> accData;
    myo::Vector3<float > gyroData;
    
    const Frame &getFrame ()
    {
        currentFrame.roll=roll_w;
        currentFrame.pitch=pitch_w;
        currentFrame.yaw=yaw_w;
        currentFrame.accData=accData;
        currentFrame.gyroData=gyroData;
        currentFrame.emgData=emgData;
        currentFrame.onArm=onArm;
        currentFrame.whichArm=swhichArm; 
        currentFrame.isUnlocked=isUnlocked;
        return currentFrame;
    }    
    
};
 


     
     
    
 


 