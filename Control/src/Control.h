#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Parameter.h"


class Control
{
public:
    Control() {
        name = "gui";
        gui.setup(name);
        ofAddListener(ofEvents().update, this, &Control::update);
        ofAddListener(ofEvents().draw, this, &Control::draw);
        visible = true;
    }
    
    string getName() {return name;}

    void setName(string name) {
        this->name = name;
        gui.setName(name);
    }
    
    template <typename T>
    void registerParameter(string name, ofParameter<T> *parameter, T min, T max);

    template <typename T>
    void registerParameter(string name, ofParameter<T> *parameter);

    template <typename T>
    void lerpTo(ofParameter<T> *parameter, T goToValue, int numFrames) {
        parameters[ofToString(parameter)]->lerpTo(goToValue, numFrames);
    }
    template <typename T>
    void easeTo(ofParameter<T> *parameter, T goToValue, int numFrames) {
        parameters[ofToString(parameter)]->easeTo(goToValue, numFrames);
    }

    void setEasingCoefficient(float easeCoeff) {
        for (auto &p: parameters) {
            parameters[p.first]->setEasingCoefficient(easeCoeff);
        }
    }
    
    void setGuiPosition(int x, int y) {gui.setPosition(x, y);}
    void setVisible(bool b) { visible = b; }
    void toggleVisible() { visible = !visible; }

    void clearParameters() {
        parameters.clear();
        gui.clear();
    }
    
private:
    
    void update(ofEventArgs &data) {
        for (auto &p: parameters) {
            parameters[p.first]->update();
        }
    }
    void draw(ofEventArgs &data) {
        if (visible) {
            gui.draw();
        }
    }
    
    ofxPanel gui;
    string name;
    bool visible;
    map<string, ParameterBase *> parameters;
};


template <typename T>
void Control::registerParameter(string name, ofParameter<T> *parameter, T min, T max) {
    parameter->set(name, (min+max)*0.5, min, max);
    parameters[ofToString(parameter)] = new Parameter<T>(parameter, min, max);
    gui.add(*parameter);
}

template <typename T>
void Control::registerParameter(string name, ofParameter<T> *parameter) {
    parameter->set(name, *parameter);
    parameters[ofToString(parameter)] = new Parameter<T>(parameter);
    gui.add(*parameter);
}
