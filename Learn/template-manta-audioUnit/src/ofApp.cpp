#include "ofApp.h"


//-------------
void ofApp::setup(){
    learn.setOutputOptionVisible(false);

    // setup selection gui
    guiSelectUnit = new ofxUICanvas("Select AudioUnit");
    guiSelectUnit->addLabelButton("Aalto", false);
    guiSelectUnit->addLabelButton("Kaivo", false);
    guiSelectUnit->autoSizeToFitWidgets();
    ofAddListener(guiSelectUnit->newGUIEvent, this, &ofApp::selectAudioUnit);
    
    // positioning
    guiSelectUnit->setPosition(1030, 5);
    output.setGuiPosition(1030, 85);

    // setup learn->audio bridge
    output.setupLearn(&learn);
    
    
    aumc.setup(learn.getMantaControllerRef(), &audio);
}

//-------------
void ofApp::setupAutioUnit(OSType type, OSType subType, OSType manufacturer){
    dac.stop();
    audio.reset();
    
    audio = ofxAudioUnitSampler(type, subType, manufacturer);
    audio.connectTo(mixer, 0);
    mixer.connectTo(dac);
    dac.start();
    
    output.setupAudioUnit(&audio);
}

//-------------
void ofApp::update(){
    learn.update();
    output.update();
}

//-------------
void ofApp::draw(){
    learn.draw();
    output.draw();
}

//-------------
void ofApp::keyPressed(int key){
    if (key=='g')
        learn.toggleVisible();
    else if (key=='!')
        learn.toggleMantaVisible();
}

//-------------
void ofApp::selectAudioUnit(ofxUIEventArgs &e) {
    if (e.getButton()->getValue())  return;
    if (e.getName() == "Aalto") {
        setupAalto();
    }
    else if (e.getName() == "Kaivo") {
        setupKaivo();
    }
}
