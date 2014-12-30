#include "Synth.h"


Synth::~Synth() {
    free();
}

Synth::Synth(string name, map<string, ofxSCBus*> & buses, map<string, ofxSCBuffer*> & buffers) : ofxSCSynth(name) {
    this->name = name;
    this->buses = buses;
    this->buffers = buffers;
    control.setName(name);

    vector<string> busInNames, busOutNames, bufferInNames, bufferOutNames;
    busOutNames.push_back("out");
    map<string, ofxSCBus*>::iterator it = buses.begin();
    while(it != buses.end()) {
        busInNames.push_back(it->first);
        busOutNames.push_back((it++)->first);
    }
    map<string, ofxSCBuffer*>::iterator itb = buffers.begin();
    while(itb != buffers.end()) {
        bufferInNames.push_back(itb->first);
        bufferOutNames.push_back((itb++)->first);
    }
    control.addMenu("choose bus in", busInNames, this, &Synth::chooseBusIn);
    control.addMenu("choose bus out", busOutNames, this, &Synth::chooseBusOut);
    control.addMenu("choose buffer in", bufferInNames, this, &Synth::chooseBufferIn);
    control.addMenu("choose buffer out", bufferOutNames, this, &Synth::chooseBufferOut);

    control.addEvent("play", this, &Synth::play);
    control.addEvent("head", this, &Synth::head);
    control.addEvent("tail", this, &Synth::tail);
    control.addEvent("stop", this, &Synth::stop);
}

void Synth::addParameter(string name, float min, float max) {
    control.addParameter(name, new float(), min, max);
}

void Synth::play(string &s) {
    active = true;
    create();
}

void Synth::head(string &s) {
    active = true;
    addToHead();
}

void Synth::tail(string &s) {
    active = true;
    addToTail();
}

void Synth::stop(string &s) {
    active = false;
    free();
}

void Synth::update() {
    if (!active) return;
    vector<ParameterBase *> parameters = control.getParameters();
    for (int i=0; i<parameters.size(); i++) {
        set(parameters[i]->getName(), parameters[i]->get<float>());
    }
}

void Synth::setGuiPosition(int x, int y) {
    control.setGuiPosition(x, y);
}

void Synth::setVisible(bool visible) {
    this->visible = visible;
    control.setVisible(visible);
}
