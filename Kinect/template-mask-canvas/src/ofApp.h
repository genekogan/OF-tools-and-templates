#pragma once

#include "ofMain.h"
#include "Canvas.h"
#include "Kinect.h"


class ofApp : public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    
private:
    
    Canvas canvas;
    Kinect kinect;
    ofFbo fbo;
};
