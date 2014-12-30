#include "ofApp.h"

//----------
void ofApp::setup(){
    manta.setup();
    
    // manta event listeners
    manta.addPadListener(this, &ofApp::PadEvent);
    manta.addSliderListener(this, &ofApp::SliderEvent);
    manta.addButtonListener(this, &ofApp::ButtonEvent);
    manta.addPadVelocityListener(this, &ofApp::PadVelocityEvent);
    manta.addButtonVelocityListener(this, &ofApp::ButtonVelocityEvent);
    
    // manta gui click listeners
    manta.addClickListener(this, &ofApp::click);
    
    
    synth = new ofxSCSynth("sine_harmonic");
	synth->create();

}

//----------
void ofApp::update(){
    manta.update();
}

//----------
void ofApp::draw(){
    manta.draw(5, 5, 480);
    manta.drawStats(5, 385, 480);
}

//----------
void ofApp::keyPressed(int key){

}

//----------
void ofApp::exit() {
    manta.removePadListener(this, &ofApp::PadEvent);
    manta.removeSliderListener(this, &ofApp::SliderEvent);
    manta.removeButtonListener(this, &ofApp::ButtonEvent);
    manta.removePadVelocityListener(this, &ofApp::PadVelocityEvent);
    manta.removeButtonVelocityListener(this, &ofApp::ButtonVelocityEvent);
    manta.removeClickListener(this, &ofApp::click);
    manta.close();
    
    synth->free();
}

//----------
ofApp::~ofApp(){
    exit();
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

//----------
void ofApp::click(MantaElement &e) {
    cout << "clicked "<< (e.type == PAD ? "pad" : (e.type == SLIDER ? "slider" : "button")) << " : ";
    cout << "selection "<<e.selection<<", element "<<e.element << endl;
}
