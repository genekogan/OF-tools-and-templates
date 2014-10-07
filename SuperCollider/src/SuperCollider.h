#pragma once

#include "ofMain.h"
#include "Control.h"
#include "ofxSuperCollider.h"
#include "ofxRegex.h"
#include "Instrument.h"



class SuperColliderLayer
{
public:
    void setup(string synthType, string synthFile);
    void update();
    void guiEvent(string &s);
    
    void setBusIn(ofxSCBus *bus) {
        for (int i=0; i<instruments.size(); i++) {
            instruments[i]->setBusIn(bus);
        }
    }
    void setBusOut(ofxSCBus *bus) {
        for (int i=0; i<instruments.size(); i++) {
            instruments[i]->setBusOut(bus);
        }
    }
    
    
    void setBusOutSelf() {
        for (int i=0; i<instruments.size(); i++) {
            instruments[i]->setBusOut(bus);
        }
    }
    
    
    void setBusOutToDac() {
        for (int i=0; i<instruments.size(); i++) {
            instruments[i]->setBusOutToDac();
        }
    }
    
    void setGuiPosition(int x, int y) {
        control.setGuiPosition(x, y);
        for (int i=0; i<instruments.size(); i++) {
            instruments[i]->setGuiPosition(x+200, y);
        }
    }
    
    ofxSCBus * getBus() {return bus;}
    
protected:
    
    Control control;
    vector<Instrument *> instruments;
    ofxSCBus *bus;
};



class SuperCollider
{
public:
    void setup();
    void update();
    SuperColliderLayer * addLayer(string synthType, string synthFile);
    
    void setGuiPosition(int x, int y) {
        for (int i=0; i<layers.size(); i++) {
            layers[i]->setGuiPosition(i*400, 5);
        }
    }

protected:

    vector<SuperColliderLayer *> layers;
};
