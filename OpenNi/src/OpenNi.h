#pragma once

#include "ofMain.h"
#include "ofxOpenNi.h"
#include "Control.h"


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
    Control control;
};
