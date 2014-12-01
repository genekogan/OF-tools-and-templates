#pragma once

#include "ofMain.h"
#include "Control.h"
#include "ModuleImageAnalyzer.h"


class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    
    ModuleImageAnalyzer learn;
};
