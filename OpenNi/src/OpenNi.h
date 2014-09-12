#pragma once

#include "ofMain.h"
#include "ofxOpenNi.h"
#include "Control.h"


class OpenNi
{
public:
    void setup();
    void update();
    void draw();
    void close();
    
private:
    void userEvent(ofxOpenNIUserEvent & event);
    
	ofxOpenNI kinect;
    Control control;
    
    vector<string> jointNames;
    vector<ofPoint *> joints;
    ofPoint jointTorso, jointNeck, jointHead,
        jointLeftShoulder, jointLeftElbow, jointLeftHand,
        jointRightShoulder, jointRightElbow, jointRightHand,
        jointLeftHip, jointLeftKnee, jointLeftFoot,
        jointRightHip, jointRightKnee, jointRightFoot;

};
