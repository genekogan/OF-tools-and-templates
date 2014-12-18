#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    // setup audio unit chain
    audioUnit = ofxAudioUnitSampler('aumu', 'Aalt', 'MLbs');
    audioUnit.connectTo(mixer);
    mixer.connectTo(output);
    output.start();
    
    // setup AudioUnitPlayer
    audio.setupAudioUnit(&audioUnit);
    audio.setNoteAutoFrameLength(15);
    audio.setNoteAutoOffVelocity(100);
}

//--------------------------------------------------------------
void ofApp::update(){
    audio.update();

    // update parameter explicitly
    audio.setParameter(22, (float) ofGetMouseX());
    
    // automatically interpolate parameter between min and max
    audio.setParameterScaled(24, (float) ofGetMouseY()/ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(0);
    
    int idxGroup = 0;
    
    // get parameter groups and iterate through them
    map<string, vector<AudioUnitPlayer::AudioUnitParameter*> > parameters = audio.getParameterGroups();
    map<string, vector<AudioUnitPlayer::AudioUnitParameter*> >::iterator it = parameters.begin();
    while (it != parameters.end()) {
        string groupName = it->first;
        vector<AudioUnitPlayer::AudioUnitParameter*> group = it->second;
        string msg = groupName+" (" + ofToString(group.size())+"):\n";
        for (int i=0; i<group.size(); i++) {
            msg += " * "+ofToString(group[i]->parameterId)+": ";
            msg += group[i]->name+" (";
            msg += ofToString(group[i]->min)+"->"+ofToString(group[i]->max)+")";
            msg += "\n";
        }
        
        ofPushMatrix();
        ofTranslate(10 + 240*(idxGroup%4), 60 + 240*floor(idxGroup/4));
        ofDrawBitmapString(msg, 0, 0);
        ofPopStyle();
        ofPopMatrix();

        idxGroup++;

        ++it;
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key=='1') {
        audio.noteEvent(AudioUnitPlayer::NOTE_ON, 64, 127);
    }
    else if (key=='2') {
        audio.noteEvent(AudioUnitPlayer::NOTE_AUTO, 69, 127);
    }
    
    else if (key=='g') {
        audio.showUI();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (key=='1') {
        audio.noteEvent(AudioUnitPlayer::NOTE_OFF, 64, 127);
    }
}