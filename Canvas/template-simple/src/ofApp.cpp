#include "ofApp.h"

//-----------
void ofApp::setup(){
    ofSetBackgroundAuto(false);
    
    canvas.setup(1024, 768);
    canvas.addCreator(1);
    //canvas.addModifier(2);
    canvas.addPostProcessingLayer(1);
    
    
    
    
    /*
    gui.setup("hello world");
    gui.add(c);
    
    gui.setPosition(200, 600);
    */
}

//-----------
void ofApp::update(){
    if (dispFrameRate) {
        if (ofGetFrameNum() % 90 == 0) {
            cout << "FrameRate :: " << ofGetFrameRate() << endl;
        }
    }    
    canvas.update();
    
    //cout << c << " and "<< b << endl;
}

//-----------
void ofApp::draw() {
    canvas.draw(0, 0);
    
    //gui.draw();
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