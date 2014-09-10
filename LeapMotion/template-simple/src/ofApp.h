#pragma once

#include "ofMain.h"
#include "LeapMotion.h"

class ofApp : public ofBaseApp
{    
public:
    ~ofApp();
    void setup();
    void update();
    void draw();
    
private:
    LeapMotion leap;
};
