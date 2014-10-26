#pragma once

#include "ofMain.h"
#include "Instrument.h"
#include "Player.h"

#include "ofxAudioUnit.h"


class AudioUnits {
public:
    void setup();
    void update();
    void draw();
    
    void savePreset();
    
    Instrument aalto;

    Player player;
    
    ofxAudioUnitMixer mixer;
	ofxAudioUnitOutput output;

};