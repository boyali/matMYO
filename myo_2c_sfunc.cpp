
#include <math.h>
#include <matrix.h>
#include <mex.h>

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include "myo_class2a.hpp"

#define NUM_DEF 1
#define NUM_MAX 12000UL

DataCollector dataCollector;

 
void matAssign (mxArray *A, mxArray *B, int Counter, int mRow, int nCol) 
{
 
    for (int i=0;i<nCol;i++)
    {
        *(mxGetPr (A)+Counter + mRow*i) =  *(mxGetPr (B) + i);
    }

}

mxArray *create_fill_vector3 (const myo::Vector3<float>&v)
{
    mxArray *p = mxCreateNumericMatrix (1, 3, mxSINGLE_CLASS, mxREAL);
    *(mxGetPr (p) + 0) = (v.x());
    *(mxGetPr (p) + 1) = (v.y());
    *(mxGetPr (p) + 2) = (v.z());
    return p;
}

mxArray *create_fill_EMG (const std::array<int8_t, 8> emgData)
{
    mxArray *p = mxCreateNumericMatrix (1, 8, mxDOUBLE_CLASS, mxREAL);
    
    for (int i=0;i<8;i++)
    {
        *(mxGetPr (p) + i) =  emgData[i]; 
    }
    return p;
 
}
 
 
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  long int num = NUM_DEF; // default value for input num
  
  // handle optional input num
  if ( nrhs>0 ) { // user input value for num
    // check for valid input type and size
    if( !mxIsDouble(prhs[0]) || mxIsComplex(prhs[0]) ||
        !(mxGetM(prhs[0])==1 && mxGetM(prhs[0])==1) )
      mexErrMsgTxt("Input num must be a real numeric scalar.");
    // get the input value
    num = *mxGetPr(prhs[0]); // implicit cast to integer value
    // check for valid input value
    if (num<1 || num>NUM_MAX)
      mexErrMsgTxt("Input num is out of range.");
  }
  
  // check number of output arguments
  if(nlhs > 1){
    mexErrMsgTxt("Too many output arguments.");
  }
  
    myo::Hub hUb("com.example.hello-myo"); 
	hUb.addListener(&dataCollector);
    myo::Myo* mYo = hUb.waitForMyo(10);
 
    
    if (!mYo) {throw std::runtime_error("Unable to find a Myo!");}

    // Enable EMG streaming
//     mYo->setStreamEmg(myo::Myo::streamEmgEnabled);
    mYo->unlock(myo::Myo::unlockHold);
    hUb.setLockingPolicy(hUb.lockingPolicyNone);
    
    const char *frame_field_names[] =
    {
        "Roll",
        "Pitch",
        "Yaw",
        "accData",
        "gyroData",
        "emgData",
        "onArm",
        "isUnlocked",
        "onWhichArm",
        "whichPose",
        "eCounter",
        "aCounter",
        "gCounter"
                
    };
    int frame_fields = sizeof (frame_field_names) / sizeof (*frame_field_names); 
    plhs[0] = mxCreateStructMatrix (1, 1, frame_fields, frame_field_names);
    
    int counter=0;
    int mRow=num;//12000;
    
    const mwSize dims[]={mRow,1};
    const mwSize dimsV[]={mRow,3};
    const mwSize dimsE[]={mRow,8};
    
    mxArray *mxRoll = mxCreateNumericArray (2, dims, mxDOUBLE_CLASS, mxREAL); 
    mxArray *mxPitch = mxCreateNumericArray (2, dims, mxDOUBLE_CLASS, mxREAL); 
    mxArray *mxYaw = mxCreateNumericArray (2, dims, mxDOUBLE_CLASS, mxREAL);  
    mxArray *mxaccDataVec = mxCreateNumericArray (2, dimsV, mxDOUBLE_CLASS, mxREAL);
    mxArray *mxgyroDataVec = mxCreateNumericArray (2, dimsV, mxDOUBLE_CLASS, mxREAL); 
    mxArray *mxEMGDataVec = mxCreateNumericArray (2, dimsE, mxDOUBLE_CLASS, mxREAL); 
    mxArray *mxOnArm = mxCreateLogicalArray (2, dims);  
    mxArray *mxWhichArm=mxCreateLogicalArray (2, dims); 
    mxArray *mxisUnlocked=mxCreateLogicalArray (2, dims); 
    mxArray *mxwPose = mxCreateNumericArray (2, dims, mxDOUBLE_CLASS, mxREAL); 
    mxArray *mxeCounter = mxCreateNumericArray (2, dims, mxDOUBLE_CLASS, mxREAL); 
    mxArray *mxaCounter = mxCreateNumericArray (2, dims, mxDOUBLE_CLASS, mxREAL); 
    mxArray *mxgCounter = mxCreateNumericArray (2, dims, mxDOUBLE_CLASS, mxREAL); 
    
    
//     int counter=1;
//     // Finally we enter our main loop.
 
    
    
    while (1) 
    {
        // In each iteration of our main loop, we run the Myo event loop for a set number of milliseconds.
        // In this case, we wish to update our display 20 times a second, so we run for 1000/20 milliseconds.
        hUb.run(1);
//        mYo->unlock(myo::Myo::unlockHold);
         
        // get the frame
        const Frame &f = dataCollector.getFrame();        
//         getFrame (nlhs, plhs);

        *(mxGetPr (mxRoll) + counter) =  f.roll;
        *(mxGetPr (mxPitch) + counter) =  f.pitch;
        *(mxGetPr (mxYaw) + counter) =  f.yaw;
        *(mxGetLogicals (mxOnArm) + counter) =  f.onArm;   
        *(mxGetPr (mxwPose) + counter) =  f.wPose;
        *(mxGetPr (mxeCounter) + counter) =  f.ecounter;
        *(mxGetPr (mxaCounter) + counter) =  f.acounter;
        *(mxGetPr (mxgCounter) + counter) =  f.gcounter;
        
                
        if(f.onArm)
        {
            *(mxGetLogicals (mxWhichArm) + counter) = (f.whichArm);
        }
            
            
        *(mxGetLogicals (mxisUnlocked) + counter) = (f.isUnlocked);
        
             
        mxArray *accData = create_fill_vector3 (f.accData);
        mxArray *gyroData = create_fill_vector3 (f.gyroData);
//         mxArray *emgData = create_fill_EMG(f.emgData);
         
        matAssign(mxaccDataVec, accData, counter, mRow, 3);
        matAssign(mxgyroDataVec, gyroData, counter, mRow, 3);
        
        
        mxArray *emgData = create_fill_EMG(f.emgData);
        matAssign(mxEMGDataVec, emgData, counter, mRow, 8);
 
  
        counter++;
       
        
        
        if( counter>= mRow)//||f.isUnlocked==false)
       {
          // terminate the loop
          break;
       }
//         mexPrintf("Counter: %f\n", counter);
    }
     
     mxSetFieldByNumber (plhs[0], 0, 0, mxRoll);     
     mxSetFieldByNumber (plhs[0], 0, 1, mxPitch); 
     mxSetFieldByNumber (plhs[0], 0, 2, mxYaw); 
     mxSetFieldByNumber (plhs[0], 0, 3, mxaccDataVec); 
     mxSetFieldByNumber (plhs[0], 0, 4, mxgyroDataVec);     
     mxSetFieldByNumber (plhs[0], 0, 5, mxEMGDataVec); 
     mxSetFieldByNumber (plhs[0], 0, 6, mxOnArm);
     mxSetFieldByNumber (plhs[0], 0, 7, mxisUnlocked);
     mxSetFieldByNumber (plhs[0], 0, 8, mxWhichArm);
     mxSetFieldByNumber (plhs[0], 0, 9, mxwPose);
     mxSetFieldByNumber (plhs[0], 0, 10, mxeCounter);
     mxSetFieldByNumber (plhs[0], 0, 11, mxaCounter);
     mxSetFieldByNumber (plhs[0], 0, 12, mxgCounter);
     
     hUb.removeListener(&dataCollector);
     
  
} 







 
