#include "ofApp.h"


//---------
void ofApp::setup(){
    control.setName("myCircle");
    control.registerParameter("radius", &radius, 10.0f, 500.0f);
    control.registerParameter("color", &color, ofColor(0, 0, 0, 0), ofColor(255, 255, 255,255));
    control.registerParameter("position", &position, ofVec2f(0, 0), ofVec2f(ofGetWidth(), ofGetHeight()));
    control.registerParameter("resolution", &resolution, 3, 30);
    control.registerParameter("filled", &filled);
    
    radius = 100;
    color = ofColor(0, 255, 0);
    position = ofVec2f(200, 200);
    resolution = 16;
    filled = true;
}

//---------
void ofApp::update(){

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
        radius.easeTo(ofRandom(100, 300), 20);
    }
    else if (key=='2') {
        position.easeTo(ofVec2f(ofRandom(100, ofGetWidth()-100), ofRandom(100, ofGetHeight()-100)), 20);
    }
    else if (key=='3') {
        resolution.easeTo(ofRandom(5, 30), 20);
    }
    else if (key=='4') {
        color.easeTo(ofColor(ofRandom(255), ofRandom(255), ofRandom(255)), 10);
    }
    
    // lerping
    else if  (key=='q') {
        radius.lerpTo(ofRandom(100, 300), 20);
    }
    else if (key=='w') {
        position.lerpTo(ofVec2f(ofRandom(100, ofGetWidth()-100), ofRandom(100, ofGetHeight()-100)), 20);
    }
    else if (key=='e') {
        resolution.lerpTo(ofRandom(5, 30), 20);
    }
    else if (key=='r') {
        color.lerpTo(ofColor(ofRandom(255), ofRandom(255), ofRandom(255)), 10);
    }
}