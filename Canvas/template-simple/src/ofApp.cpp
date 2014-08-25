#include "ofApp.h"

//-----------
void ofApp::setup(){
    ofSetBackgroundAuto(false);
    
    canvas.setup(1024, 768);
    canvas.addCreator(1);
    canvas.addModifier(2);
}

//-----------
void ofApp::update(){
    if (dispFrameRate) {
        if (ofGetFrameNum() % 90 == 0) {
            cout << "FrameRate :: " << ofGetFrameRate() << endl;
        }
    }
    
    canvas.update();
}

//-----------
void ofApp::draw() {
    canvas.draw(0, 0);
}

//-----------
void ofApp::keyPressed(int key){
    if (key=='g') {
        canvas.toggleGuiVisible();
    }
    else if (key=='z') {
        dispFrameRate = !dispFrameRate;
    }
}