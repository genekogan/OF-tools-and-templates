#pragma once

#include "Canvas.h"
#include "ofxPostGlitch.h"
#include "Control.h"
#include "TimeFunction.h"


class PostGlitchLayer : public CanvasLayer {
public:
    void setup();
    string getType() {return "PostGlitchLayer";}
    void setTexLayer(CanvasLayer *texLayer=NULL);
    void render();
    
private:
    void setGuiPosition(int x, int y);
    void toggleVisible();
    void setVisible(bool visible);

    void checkGuiCalls(){}
    void updateEffects();
    void noiseChange();
    
    ofxPostGlitch post;
    
    bool convergence;
    bool glow;
    bool shaker;
    bool cutslider;
    bool twist;
    bool outline;
    bool noise;
    bool slitscan;
    bool swell;
    bool invert;
    bool crHighContrast;
    bool crBlueRaise;
    bool crRedRaise;
    bool crGreenRaise;
    bool crBlueInvert;
    bool crRedInvert;
    bool crGreenInvert;
    
    bool customParameters;
    float stepMin, stepMax;
    float delTime, pDelTime;
    
    TimeFunction<float> sv0, sv1, sv2, sv3;
    TimeFunction<float> rand;
    TimeFunction<float> m0, m1;
    TimeFunction<float> b0, b1;
    
};


