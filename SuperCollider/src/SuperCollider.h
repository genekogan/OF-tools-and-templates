#pragma once

#include "ofMain.h"
#include "Control.h"
#include "ofxSuperCollider.h"
#include "ofxRegex.h"


class Instrument {
public:
    Instrument() { }
    void setup(string name) {
        this->name = name;
        control.setGuiPosition(300,5);
        
    }
    void addParameter(string name, float min, float max) {
        control.addParameter(name, new float(), min, max);
    }
    
    string name;
    
    Control control;
};



class SuperCollider
{
public:
    SuperCollider();
    void setup();

    void getsc3();
    
    Control control;
    
    float a;
    
    vector<Instrument *> mods;
};
