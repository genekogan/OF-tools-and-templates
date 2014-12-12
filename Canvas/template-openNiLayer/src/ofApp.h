#pragma once

#include "ofMain.h"
#include "Canvas.h"
#include "OpenNi.h"
#include "ofxSecondWindow.h"


class ofApp : public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();    
    void exit();
    void keyPressed(int key);
    
private:
    
    Canvas canvas;
    OpenNi kinect;
    ofxSecondWindow window;
};
