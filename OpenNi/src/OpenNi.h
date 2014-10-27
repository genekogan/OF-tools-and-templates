#pragma once

#include "ofMain.h"
#include "ofxOpenNi.h"
#include "ofxCv.h"
#include "ofxOpenCv.h"
#include "ofxKinectProjectorToolkit.h"
#include "Control.h"


using namespace ofxCv;
using namespace cv;

enum ContourStrategy { BLOBS, SEGMENTATION };

class OpenNi
{
public:
    void setup();
    void close();
    
    void setGuiPosition(int x, int y);
    void toggleGuiVisible();

    bool update();
    void updateContours();
    void updateJoints();

    
    void draw(int x=0, int y=0, int w=640, int h=480);
    void drawCalibrated(int x=0, int y=0, int width=640, int height=480);
    
    void setCalibration(string path);
    
    vector<ofVec3f*> & getJoints(bool normalized=false);
    vector<ofVec2f>  & getProjectedJoints();
    string getJointName(int idx) {return jointNames[idx];}
    
    
    
    
    //void                    setTrackingBlobs(bool t) {trackingBlobs = t;}
    //void                    setTrackingKeypoints(bool t) {trackingKeypoints = t;}
    

    ContourFinder&          getContourFinder();
    RectTracker&            getContourTracker();
    
    
    void drawCalibratedSkeleton(int width, int height);
    
private:
    
    void userEvent(ofxOpenNIUserEvent & event);

    // kinect
    ofxOpenNI kinect;
    ofxKinectProjectorToolkit kpt;

    // kinect data
    vector<string> jointNames;
    vector<ofVec3f*> joints;
    vector<ofVec2f> projectedJoints;
    ofVec3f userBoundingBoxMin, userBoundingBoxMax;
    ofVec3f jointTorso, jointNeck, jointHead,
        jointLeftShoulder, jointLeftElbow, jointLeftHand,
        jointRightShoulder, jointRightElbow, jointRightHand,
        jointLeftHip, jointLeftKnee, jointLeftFoot,
        jointRightHip, jointRightKnee, jointRightFoot;

    // parameters
    Control control;
    
    
    
    
    /* blob tracking */
    ContourFinder           contourFinder;

    bool                    trackingBlobs;
    bool                    trackingKeypoints;
    bool trackingJoints;

    
    float                   fade;
    float                   minArea;
    float                   maxArea;
    float                   threshold;
    float                   persistence;
    float                   maxDistance;
    int                     numDilate;
    int                     numErode;
    float                   nearThreshold;
    float                   farThreshold;
    int                     smoothness;
    bool                    curved;
    float                   smoothingRate;

};


