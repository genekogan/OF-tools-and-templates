#pragma once

#include "ofMain.h"
#include "LeapMotion.h"
#include "ofxSuperCollider.h"
#include "ofxOsc.h"

class ofApp : public ofBaseApp
{
public:
    ~ofApp();
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void switchMode(int mode);
    
private:
    
    LeapMotion leap;
    ofxSCSynth *synth;
    
    float f1z = 0;
    
    
    ofVec3f *hand;
    
    
    int mode = -1;
    
    
    ofxOscSender osc;

};
