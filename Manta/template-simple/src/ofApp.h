#pragma once

#include "ofMain.h"
#include "MantaController.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    
    void exit();
    
    void PadEvent(ofxMantaEvent & evt);
    void SliderEvent(ofxMantaEvent & evt);
    void ButtonEvent(ofxMantaEvent & evt);
    void PadVelocityEvent(ofxMantaEvent & evt);
    void ButtonVelocityEvent(ofxMantaEvent & evt);

    void click(MantaElement &e);

    MantaController manta;
};
