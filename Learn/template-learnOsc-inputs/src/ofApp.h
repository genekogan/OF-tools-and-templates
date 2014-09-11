#include "ofMain.h"
#include "ofxOpenNI.h"


#define HOST "localhost"
#define PORT 58231


class ofApp : public ofBaseApp
{
public:
	void setup();
    void exit();
    
	void update();
	void draw();
    
    void userEvent(ofxOpenNIUserEvent & event);
	void keyPressed(int key);
    
	ofxOpenNI kinect;
    vector<ofPoint *> joints;
    vector<string> jointNames;
    ofPoint jointTorso, jointNeck, jointHead,
        jointLeftShoulder, jointLeftElbow, jointLeftHand,
        jointRightShoulder, jointRightElbow, jointRightHand,
        jointLeftHip, jointLeftKnee, jointLeftFoot,
        jointRightHip, jointRightKnee, jointRightFoot;
    
    //ofxOscSender osc;
};
