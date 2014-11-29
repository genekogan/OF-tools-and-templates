#pragma once

#include "ofMain.h"
#include "Control.h"
#include "ModuleOpenNi.h"
#include "OutputAudioUnit.h"
#include "AudioUnits.h"


class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
		
    ModuleOpenNi learn;
    AudioUnits audio;    
    OutputAudioUnit output;
};
