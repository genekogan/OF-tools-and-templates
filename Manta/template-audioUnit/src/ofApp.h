#pragma once

#include "ofMain.h"
#include "Control.h"
#include "ofxAudioUnit.h"
#include "AudioUnitMantaController.h"


class ofApp : public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
    
    void setupAutioUnit(OSType type, OSType subType, OSType manufacturer);
    void setupAalto() {setupAutioUnit('aumu', 'Aalt', 'MLbs');}
    void setupKaivo() {setupAutioUnit('aumu', 'Kaiv', 'MLbs');}
    
    void selectAudioUnit(ofxUIEventArgs &e);
    void keyPressed(int key);
    
	ofxAudioUnitSampler audio;
    ofxAudioUnitMixer mixer;
	ofxAudioUnitOutput dac;
    
    ofxUICanvas *guiSelectUnit;
    
    MantaController manta;
    AudioUnitMantaController aumc;
};
