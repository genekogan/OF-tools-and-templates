#pragma once

#include "Canvas.h"
#include "ofxPostGlitch.h"
#include "Control.h"


class PostGlitchLayer : public CanvasLayer {
    ~PostGlitchLayer();
    void setup();
    void setGuiPosition(int x, int y);
    void toggleVisible();
    
private:
    
    void programChange(bool &b);
    void buildGui() {}
    void render();
    
    ofxPostGlitch post;
    
    ofxParameter<bool> convergence;
    ofxParameter<bool> glow;
    ofxParameter<bool> shaker;
    ofxParameter<bool> cutslider;
    ofxParameter<bool> twist;
    ofxParameter<bool> outline;
    ofxParameter<bool> noise;
    ofxParameter<bool> slitscan;
    ofxParameter<bool> swell;
    ofxParameter<bool> invert;
    ofxParameter<bool> crHighContrast;
    ofxParameter<bool> crBlueRaise;
    ofxParameter<bool> crRedRaise;
    ofxParameter<bool> crGreenRaise;
    ofxParameter<bool> crBlueInvert;
    ofxParameter<bool> crRedInvert;
    ofxParameter<bool> crGreenInvert;
};


