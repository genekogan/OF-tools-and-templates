#pragma once

#include "ofMain.h"
#include "ofxAudioUnit.h"
//#include "Theory.h"
//#include "Sequencer.h"
//#include "Control.h"
//#include "MantaController.h"

class Player {
public:
    void setup();
    void update();
    void draw();
    void connectTo(ofxAudioUnitMixer &mixer, int channel);

    ofxAudioUnitFilePlayer player;
    ofxAudioUnitTap tap;
    ofPolyline wave;
    vector<float> data;
    int t;
    
    ofFbo fbo;
    
    
    void blah() {
        int tt = ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 55) * 44100; //(int) ofRandom(44100 *30);
        cout << tt << endl;
        player.playAtSampleTime(tt);
    }
};