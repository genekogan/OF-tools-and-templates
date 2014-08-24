#pragma once

#include "ofMain.h"
#include "Scene.h"


class DebugScreen : public Scene
{
public:
    void setup();
    void update();
    void draw();

private:
    enum DebugType { FULL, GRADIENT, CHECKERBOARD, FRAMES };
    
    void drawFull();
    void drawCheckerboard();
    void drawFrames();
    void drawGradient();

    void setType(string & s);
    void setupGradient(string &gradientType);
    void changedColor(ofColor & c);
    
    ofVboMesh gradientMesh;
    DebugType type = GRADIENT;
    string gradientMode;
    
    ofxParameter<ofColor> color1;
    ofxParameter<ofColor> color2;
    ofxParameter<int> numRects;
    ofxParameter<int> speed;
};
