#pragma once

#include "ofMain.h"
#include "Control.h"


class ofApp : public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    
    Control control;
    
    ofxParameter<float>   radius;
    ofxParameter<ofColor> color;
    ofxParameter<ofVec2f> position;
    ofxParameter<int>     resolution;
    ofxParameter<bool>    filled;
};
