#include "ofApp.h"


//-------------
void ofApp::setup(){
    
    // setup selection gui
    guiSelectUnit = new ofxUICanvas("Select AudioUnit");
    guiSelectUnit->addLabelButton("Aalto", false);
    guiSelectUnit->addLabelButton("Kaivo", false);
    guiSelectUnit->autoSizeToFitWidgets();
    ofAddListener(guiSelectUnit->newGUIEvent, this, &ofApp::selectAudioUnit);

    manta.setup();
    aumc.setup(&manta, &audio);
    
    // positioning
    guiSelectUnit->setPosition(750, 5);
    aumc.setGuiPosition(750, 300);
}

//-------------
void ofApp::setupAutioUnit(OSType type, OSType subType, OSType manufacturer){
    dac.stop();
    audio.reset();
    
    audio = ofxAudioUnitSampler(type, subType, manufacturer);
    audio.connectTo(mixer, 0);
    mixer.connectTo(dac);
    dac.start();
    
    aumc.setupAudioUnit(&audio);
}

//-------------
void ofApp::update(){
    manta.update();
    aumc.update();
}

//-------------
void ofApp::draw(){
    manta.draw(5, 85, 410);
    manta.drawStats(5, 420, 410);
    aumc.draw();
}

//-------------
void ofApp::keyPressed(int key){
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
