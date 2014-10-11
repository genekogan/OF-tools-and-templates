#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    manta.setup();
    
    manta.addPadListener(this, &ofApp::PadEvent);
    manta.addSliderListener(this, &ofApp::SliderEvent);
    manta.addButtonListener(this, &ofApp::ButtonEvent);
    manta.addPadVelocityListener(this, &ofApp::PadVelocityEvent);
    manta.addButtonVelocityListener(this, &ofApp::ButtonVelocityEvent);
}

//--------------------------------------------------------------
void ofApp::update(){
    manta.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    manta.draw(5, 5, 500);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//----------
void ofApp::exit() {
    manta.close();
}

//----------
void ofApp::PadEvent(ofxMantaEvent & evt) {
    cout << "Pad event: " << ofGetElapsedTimeMicros() << ", id " << evt.id << ", row "<< evt.row <<", column "<< evt.col << ", value "<< evt.value << endl;
}

//----------
void ofApp::SliderEvent(ofxMantaEvent & evt) {
    cout << "Slider event: id " << evt.id << ", value "<< evt.value << endl;
}

//----------
void ofApp::ButtonEvent(ofxMantaEvent & evt) {
    cout << "Button event: id " << evt.id << ", value "<< evt.value << endl;
}

//----------
void ofApp::PadVelocityEvent(ofxMantaEvent & evt) {
    cout << "Pad velocity event: id " << evt.id << ", row "<< evt.row <<", column "<< evt.col << ", value "<< evt.value << endl;
}

//----------
void ofApp::ButtonVelocityEvent(ofxMantaEvent & evt) {
    cout << "Button velocity event: id " << evt.id << ", value "<< evt.value << endl;
}
