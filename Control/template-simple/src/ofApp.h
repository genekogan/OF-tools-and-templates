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
    
    ofParameter<float>   radius;
    ofParameter<ofColor> color;
    ofParameter<ofVec2f> position;
    ofParameter<int>     resolution;
    ofParameter<bool>    filled;
};
