#pragma once

#include "ofMain.h"
#include "Scene.h"


class Letters : public Scene
{
public:
    void setup();
    void update();
    void draw();

private:
    ofxParameter<int> nrows;
    ofxParameter<int> ncols;
    ofxParameter<ofVec2f> letterRange;
    ofxParameter<int> rate;
    ofxParameter<float> density;
    ofxParameter<ofColor> color;
    ofxParameter<ofVec2f> rateNoise;
    ofxParameter<float> rateNoiseTime;
    ofxParameter<ofVec2f> letterNoise;
    ofxParameter<float> letterNoiseTime;
    ofxParameter<bool> noisyLetters;
    ofxParameter<bool> chinese;
    vector<char> chars;
    vector<int> charsC;
    ofTrueTypeFont font, fontC;
    static const char * alphanum;
};


/*
 - chinese character sets
   - embed quotes
 - other languages
 - different character orderings
*/