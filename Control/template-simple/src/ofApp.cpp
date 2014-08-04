#include "ofApp.h"


//---------
void ofApp::setup(){
    control.setName("myCircle");
    control.registerParameter("radius", &radius, 10.0f, 500.0f);
    control.registerParameter("color", &color, ofColor(0, 0, 0, 0), ofColor(255, 255, 255,255));
    control.registerParameter("position", &position, ofVec2f(0, 0), ofVec2f(ofGetWidth(), ofGetHeight()));
    control.registerParameter("resolution", &resolution, 3, 30);
    control.registerParameter("filled", &filled);
}

//---------
void ofApp::update(){
    control.setEasingCoefficient( (float)ofGetMouseX()/ofGetWidth() );
}

//---------
void ofApp::draw(){
    if (filled) ofFill();
    else        ofNoFill();
    ofSetCircleResolution(resolution);
    ofSetColor(color->r, color->g, color->b);
    ofCircle(position->x, position->y, radius);
}

//---------
void ofApp::keyPressed(int key){
    // easing
    if      (key=='1')   {
        control.easeTo(&radius, ofRandom(100, 500), 20);
    }
    else if (key=='2') {
        control.easeTo(&position, ofVec2f(ofRandom(ofGetWidth()), ofRandom(ofGetHeight())), 20);
    }
    else if (key=='3') {
        control.easeTo(&resolution, (int) ofRandom(3, 30), 20);
    }
    else if (key=='4') {
        control.easeTo(&color, ofColor(ofRandom(255), ofRandom(255), ofRandom(255)), 10);
    }
    // lerping
    else if  (key=='q') {
        control.lerpTo(&radius, ofRandom(100, 500), 20);
    }
    else if (key=='w') {
        control.lerpTo(&position, ofVec2f(ofRandom(ofGetWidth()), ofRandom(ofGetHeight())), 20);
    }
    else if (key=='e') {
        control.lerpTo(&resolution, (int) ofRandom(3, 30), 20);
    }
    else if (key=='r') {
        control.lerpTo(&color, ofColor(ofRandom(255), ofRandom(255), ofRandom(255)), 10);
    }
}