#pragma once

#include "ofMain.h"
#include "Syphon.h"


class ofApp : public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    
private:

    Scene *syphon;
};
