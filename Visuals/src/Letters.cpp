#include "Letters.h"



const char * Letters::alphanum = {
    "0123456789ABCDEFGHIJKLMNOPQR"
    "STUVWXYZabcdefghijklmnopqrst"
    "uvwxyz!@#$%^&*()_+;<>?~`"};

const char * Letters::chineseCharacters = {
    "指事字指事字指事字指事字指事字指事字指事字指事字指事字"
    "指事字指事字指事字指事字指事字指事字指事字指事字指事字"
    "指事字指事字指事字指事字指事字指事字指事字指事字指事字"};


void Letters::setup() {
    setName("Letters");

    control.registerParameter("rows", &nrows, 2, 200);
    control.registerParameter("cols", &ncols, 2, 200);
    control.registerParameter("letterRange", &letterRange, ofVec2f(0, 0), ofVec2f(1,1));
    control.registerParameter("color", &color, ofColor(0), ofColor(255));
    control.registerParameter("rate", &rate, 1, 10);
    control.registerParameter("density", &density, 0.0f, 1.0f);
    control.registerParameter("rateNoise", &rateNoise, ofVec2f(0, 0), ofVec2f(0.1, 0.1));
    control.registerParameter("rateNoiseTime", &rateNoiseTime, 0.0f, 1.0f);
    control.registerParameter("letterNoise", &letterNoise, ofVec2f(0, 0), ofVec2f(0.1, 0.1));
    control.registerParameter("letterNoiseTime", &letterNoiseTime, 0.0f, 1.0f);
    control.registerParameter("noisyLetters", &noisyLetters);
    
    nrows = 30;
    ncols = 40;
    letterRange = ofVec2f(0, 1);
    rate = 1;
    density = 0.5;
    rateNoise = ofVec2f(0.1, 0.1);
    rateNoiseTime = 0.1;
    letterNoise = ofVec2f(0.1, 0.1);
    letterNoiseTime = 0.1;
    color = ofColor(0, 255, 20);
    noisyLetters = false;
    
}

void Letters::update() {
    if (chars.size() != nrows * ncols) {
        chars.resize(nrows * ncols);
        font.loadFont("AndaleMono.ttf", (int) height / nrows);
    }
    
    if (ofGetFrameNum() % rate == 0) {
        int idx = 0;
        for (int i=0; i<ncols; i++) {
            for (int j=0; j<nrows; j++) {
                if (ofNoise(rateNoise->x*i, rateNoise->y*j, rateNoiseTime * ofGetFrameNum()) < density) {
                    if (noisyLetters) {
                        chars[idx] = alphanum [ (int) ofMap(ofNoise(letterNoise->x*i, letterNoiseTime * ofGetFrameNum(), letterNoise->y*j), 0, 1, 81*letterRange->x, 81*letterRange->y) ];
                    }
                    else {
                        chars[idx] = alphanum[ (int) ofRandom(81 * letterRange->x, 81 * letterRange->y) ];
                    }
                    
                }
                idx++;
            }
        }
    }
    
}

void Letters::draw() {
    int idx = 0;
    float x, y;
    for (int i=0; i<ncols; i++) {
        for (int j=0; j<nrows; j++) {
            x = ofMap(i, 0, ncols, 0, width);
            y = ofMap(j, 0, nrows, 0, height);
            ofSetColor(color);
            font.drawString(ofToString(chars[idx++]), x, y);
            //ofDrawBitmapString(ofToString(chars[idx++]), x, y);
        }
    }
}