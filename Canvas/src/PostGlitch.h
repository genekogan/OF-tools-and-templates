#pragma once

#include "Canvas.h"
#include "ofxPostGlitch.h"
#include "Control.h"
#include "TimeFunction.h"


class PostGlitchLayer : public CanvasLayer {
public:
    ~PostGlitchLayer();
    void setup();
    
    void setGuiPosition(int x, int y);
    void toggleVisible();
    void setVisible(bool visible);
    
private:
    
    void effectsChange(bool &b);
    void noiseChange(float &v);
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
    
    ofxParameter<bool> customParameters;
    ofxParameter<float> stepMin, stepMax;
    ofxParameter<float> delTime;
    
    TimeFunction<float> sv0, sv1, sv2, sv3;
    TimeFunction<float> rand;
    TimeFunction<float> m0, m1;
    TimeFunction<float> b0, b1;
    
};


