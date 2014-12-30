#pragma once

#include "ofMain.h"
#include "MantaController.h"
#include "SuperCollider.h"

#include "ofxSuperCollider.h"


class ofApp : public ofBaseApp
{
public:
    ~ofApp();
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
    
    
    
    ofxSCSynth	*synth;

};
