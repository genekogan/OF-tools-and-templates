#pragma once

#include "ofMain.h"
#include "Control.h"
#include "ofxSuperCollider.h"

class Instrument
{
public:
    Instrument() { }
    
    void setup(string name) {
        this->name = name;
        synth = new ofxSCSynth(name);
        control.addEvent("create", this, &Instrument::create);
        control.addEvent("addToTail", this, &Instrument::addToTail);
        control.addEvent("grain", this, &Instrument::grain);
        control.addEvent("free", this, &Instrument::free);
        active = false;
    }
    
    void setBusIn(ofxSCBus *bus) {
        synth->set("busIn", bus->index);
        cout << "set bus in for " << name << " :: " << bus->index << endl;

    }
    
    void setBusOut(ofxSCBus *bus) {
        synth->set("busOut", bus->index);
        cout << "set bus out for " << name << " :: " << bus->index << endl;
    }
    
    void setBusOutToDac() {
        synth->set("busOut", 0);
        cout << "set bus out for " << name << " :: " << 0 << endl;

    }
    
    void addParameter(string name, float min, float max) {
        control.addParameter(name, new float(), min, max);
    }
    
    void create(string &s) {
        synth->create();
        active = true;
    }

    void addToTail(string &s) {
        cout << "add to tail"<<endl;
        synth->addToTail();
        active = true;
    }

    void grain(string &s) {
        synth->grain();
        active = true;
    }
    
    void free(string &s) {
        synth->free();
        active = false;
    }
    
    void update() {
        if (!active) return;
        vector<ParameterBase *> parameters = control.getParameters();
        for (int i=0; i<parameters.size(); i++) {
            cout << "update " << parameters[i]->getName() <<" "<< parameters[i]->get<float>()<< endl;
            synth->set(parameters[i]->getName(), parameters[i]->get<float>());
        }
    }
    
    void setGuiPosition(int x, int y) {
        control.setGuiPosition(x, y);
    }
    
    void setVisible(bool visible) {
        this->visible = visible;
        control.setVisible(visible);
    }
    
    void toggleVisible() {
        control.setVisible(!visible);
    }
    
    string getName() {
        return name;
    }
    
protected:
    
    ofxSCSynth *synth;
    string name;
    Control control;
    bool active;
    bool visible;
};

