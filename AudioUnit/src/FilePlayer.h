#pragma once

#include "ofMain.h"
#include "ofxAudioUnit.h"
#include "Waveform.h"
#include "Control.h"


class FilePlayer
{
public:
    void setup();
    void connectTo(ofxAudioUnitMixer &mixer, int channel);

    void update();
    void draw();
    
    void setGuiPosition(int x, int y);
    void setVisible(bool visible);
    void toggleVisible() {setVisible(!visible);}

private:
    
    void selectClip(string &s);
    
    void waveformSelectionEvent(ofPoint &e);
    void waveformSelectEvent(ofPoint &e);
    void setSelection(float _t1, float _t2);
    void addSelection(float _t1, float _t2);
    
    ofxAudioUnitFilePlayer player;
    ofxAudioUnitTap tap;
    ofPolyline wave;
    vector<float> data;
    
    
    Waveform waveform;
    float t1, t2;
    int currentSample, sample1, sample2, numSamples;
    
    Control control;
    bool visible;
};

