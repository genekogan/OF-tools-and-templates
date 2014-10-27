#pragma once

#include "ofMain.h"
#include "ofxAudioUnit.h"
#include "Waveform.h"


class Player
{
public:
    void setup();
    void connectTo(ofxAudioUnitMixer &mixer, int channel);

    void update();
    void draw();

private:

    ofxAudioUnitFilePlayer player;
    ofxAudioUnitTap tap;
    ofPolyline wave;
    vector<float> data;
    
    void waveformSelectionEvent(ofPoint &e);
    void setSelection(float _t1, float _t2);

    Waveform waveform;
    float t1, t2;
    int currentSample, sample1, sample2, numSamples;
};