#pragma once

#include "ofMain.h"
#include "ofxOpenNi.h"


class OpenNi
{
public:
    void setup();
    void update();
    void draw();
    void close();
    
private:
    void userEvent(ofxOpenNIUserEvent & event);
    
	ofxOpenNI openNIDevice;
    ofTrueTypeFont verdana;
    Control control;
};
