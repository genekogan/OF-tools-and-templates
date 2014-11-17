#pragma once

#include "ofMain.h"
#include "OpenNi.h"
#include "ofxSecondWindow.h"



class ofApp : public ofBaseApp
{
public:
	void setup();
	void update();
	void draw();
    void exit();
    void keyPressed  (int key);
    
	OpenNi kinect;
    ofxSecondWindow gfx;
};
