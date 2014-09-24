#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetBackgroundAuto(false);
    
    scene = new MyCustomScene();
    scene->setup(1024, 768);
    scene->setActive(true);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    scene->draw(0, 0);
}