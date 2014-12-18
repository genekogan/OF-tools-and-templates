#pragma once

#include "ofMain.h"
#include "AudioUnitPlayer.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
		
    AudioUnitPlayer audio;
    ofxAudioUnitSampler audioUnit;
    ofxAudioUnitMixer mixer;
    ofxAudioUnitOutput output;
    
};
