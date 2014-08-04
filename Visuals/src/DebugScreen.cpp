#include "DebugScreen.h"


//--------
void DebugScreen::setup(int width, int height) {
    Scene::setup(width, height);
    control.registerParameter("color", &color, ofColor(0, 0), ofColor(255, 255));
    color = ofColor(ofRandom(255), ofRandom(255), ofRandom(255), 255);
}

//--------
void DebugScreen::update() {
    
}

//--------
void DebugScreen::draw() {
    if (type == FULL) {
        ofSetColor(color);
        ofFill();
        ofRect(0, 0, width, height);
    }
    
    else if (type == FRAME) {
        ofSetColor(0);
        ofFill();
        ofSetColor(color);
        ofNoFill();
        ofSetLineWidth(8);
        ofRect(4, 4, width-8, height-8);
    }

    else if (type == FRAMES) {
        ofSetColor(color);
        ofNoFill();
        ofSetLineWidth(8);
        for (int i=0; i<8; i++) {
            ofRect(4 + i*24, 4 + i*24, width-8-i*48, height-8-i*48);
        }
    }
}
