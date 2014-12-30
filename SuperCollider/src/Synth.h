#pragma once

#include "ofMain.h"
#include "Control.h"
#include "ofxSuperCollider.h"



class Buffer : public ofxSCBuffer
{
public:
    
};




class Synth : public ofxSCSynth
{
public:
    ~Synth();
    Synth(string name, map<string, ofxSCBus*> & buses, map<string, ofxSCBuffer*> & buffers);
    void addParameter(string name, float min, float max);

    void play(string &s);
    void head(string &s);
    void tail(string &s);
    void stop(string &s);
    void update();
    
    void chooseBusIn(string &s) {
        if (s!="out") set("busIn", buses[s]->index);
    }
    void chooseBusOut(string &s) {
        set("busOut", s=="out" ? 0 : buses[s]->index);
    }

    void chooseBufferIn(string &s) {
        set("bufIn", buffers[s]->index);
    }
    void chooseBufferOut(string &s) {
        set("bufOut", buffers[s]->index);
    }

    
    void setGuiPosition(int x, int y);
    void setVisible(bool visible);
    void toggleVisible() {control.setVisible(!visible);}
    
protected:
    
    string name;
    Control control;
    bool active;
    bool visible;
    
    map<string, ofxSCBuffer*> buffers;
    map<string, ofxSCBus*> buses;
};
