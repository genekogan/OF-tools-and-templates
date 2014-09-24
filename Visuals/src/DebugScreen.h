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
    void refresh(bool & b);
    
    ofVboMesh gradientMesh;
    DebugType type = GRADIENT;
    string gradientMode;
    
    ofColor color1;
    ofColor color2;
    int numRects;
    int speed;
};
