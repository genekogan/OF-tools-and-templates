#pragma once

#include "ofMain.h"
#include "Control.h"
#include "ofxSuperCollider.h"
#include "ofxRegex.h"
#include "Synth.h"




class SynthGroup {
public:
    SynthGroup(string name, int idx) {
        this->name = name;
        this->idx = idx;
        busOut = new ofxSCBus();
    }
    void setup(map<string, Synth*> *synths, vector<string> synthNames, Control *control) {
        this->synths = synths;
        control->addMenu(name, synthNames, this, &SynthGroup::chooseSynth);
    }
    map<string, Synth*> * getSynths() {return synths;}
    string getName() {return name;}
    ofxSCBus * getBus() {return busOut;}
    void chooseSynth(string &s) {
        map<string, Synth*>::iterator it = synths->begin();
        while (it != synths->end()) {
            it->second->setVisible(false);
            ++it;
        }
        if (synths->count(s) > 0) {
            synths->at(s)->setGuiPosition(170*(idx+1), 5);
            synths->at(s)->setVisible(true);
        }
    }

private:
    string name;
    int idx;
    map<string, Synth*> *synths;
    ofxSCBus *busOut;
};




class SuperCollider
{
public:
    void setup();
    void readFromFile(string synthType, string synthFile);
    void free();
    
    void update();
    
protected:
    
    Control control;

    map<string, SynthGroup*> groups;
    map<string, ofxSCBuffer*> buffers;
    map<string, ofxSCBus*> buses;

    int groupCount;
    
};
