#pragma once

#include "ofMain.h"
#include "ofxOpenNi.h"
#include "Control.h"


class OpenNi
{
public:
    void setup();
    void update();
    void draw(int x=0, int y=0, int w=640, int h=480);
    void close();
    
    vector<ofPoint *> & getJoints() {return joints;}
    string getJointName(int idx) {return jointNames[idx];}
    
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
