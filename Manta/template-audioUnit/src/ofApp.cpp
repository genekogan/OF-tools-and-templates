#include "ofApp.h"

//-------------
void ofApp::setup(){
    
    manta.setup();
    aumc.setup(&manta, &audio);

    // setup selection gui
    guiSelectUnit = new ofxUICanvas("Select AudioUnit");
    guiSelectUnit->addLabelButton("Aalto", false);
    guiSelectUnit->addLabelButton("Kaivo", false);
    guiSelectUnit->addSpacer();
    guiSelectUnit->addLabelButton("show ui", false);
    guiSelectUnit->autoSizeToFitWidgets();
    ofAddListener(guiSelectUnit->newGUIEvent, this, &ofApp::selectAudioUnit);

    // positioning
    guiSelectUnit->setPosition(760, 5);
    aumc.setGuiPosition(760, 120);
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
    manta.draw(5, 5, 410);
    manta.drawStats(5, 340, 410);
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
    else if (e.getName() == "show ui") {
        showAudioUnitUI();
    }
}
