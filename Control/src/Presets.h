#pragma once

#include "Control.h"


class Control;
class MetaController;
class Sequencer;


class Presets {
public:
    Presets();
    
    bool getMetaEnabled() {return metaEnabled;}
    void setMetaEnabled(bool metaEnabled) {this->metaEnabled = metaEnabled;}
    
    bool savePreset(Control &control, string filename="");
    void loadPreset(Control &control, string path, int numFrames = 0);
    void loadPreset(Control &control, ofXml &xml, int numFrames = 0);
    
    ofXml getXml(Control &control);
    ofXml getXml(vector<ParameterBase *> &parameters);
    ofXml getXml(ParameterBase * parameter);
    ofXml getXml(vector<ofxUIDropDownList *> menus);
    ofXml getXml(ofxUIDropDownList * menu);
    ofXml getXml(MetaController *meta);
    
protected:

    bool metaEnabled;
    
    template <typename T>
    void updateParameter(ofXml &xml, vector<ParameterBase *> &parameters, ParameterBase::Type type, int numFrames = 0);
    
    template <typename T>
    void parameterToXml(ofXml &xml, Parameter<T> *parameter);
};


template <typename T> void Presets::updateParameter(ofXml &xml, vector<ParameterBase *> &parameters, ParameterBase::Type type, int numFrames) {
    string name = xml.getValue<string>("Name");
    for (int i=0; i<parameters.size(); i++) {
        if (parameters[i]->getName()==name && parameters[i]->getType()==type) {
            ((Parameter <T> *) parameters[i])->lerpTo(xml.getValue<T>("Value"), numFrames);
            ((Parameter <T> *) parameters[i])->setMin(xml.getValue<T>("Min"));
            ((Parameter <T> *) parameters[i])->setMax(xml.getValue<T>("Max"));
        }
    }
}

template <typename T> void Presets::parameterToXml(ofXml &xml, Parameter<T> *parameter) {
    T value = parameter->get();
    T min = parameter->getMin();
    T max = parameter->getMax();
    float warp = ((ParameterBase *) parameter)->getWarp();
    xml.addValue("Value", ofToString(value));
    xml.addValue("Min", ofToString(min));
    xml.addValue("Max", ofToString(max));
    xml.addValue("Warp", ofToString(warp));
}

template <> inline void Presets::updateParameter<bool>(ofXml &xml, vector<ParameterBase *> &parameters, ParameterBase::Type type, int numFrames) {
    string name = xml.getValue<string>("Name");
    for (int i=0; i<parameters.size(); i++) {
        if (parameters[i]->getName()==name && parameters[i]->getType()==ParameterBase::BOOL) {
            ((Parameter <bool> *) parameters[i])->set(xml.getValue<bool>("Value"));
        }
    }
}

template <> inline void Presets::updateParameter<string>(ofXml &xml, vector<ParameterBase *> &parameters, ParameterBase::Type type, int numFrames) {
    string name = xml.getValue<string>("Name");
    for (int i=0; i<parameters.size(); i++) {
        if (parameters[i]->getName()==name && parameters[i]->getType()==ParameterBase::STRING) {
            ((Parameter <string> *) parameters[i])->set(xml.getValue<string>("Value"));
        }
    }
}

template <> inline void Presets::parameterToXml<bool>(ofXml &xml, Parameter<bool> *parameter) {
    bool value = parameter->get();
    xml.addValue("Value", ofToString(value));
}

template <> inline void Presets::parameterToXml<string>(ofXml &xml, Parameter<string> *parameter) {
    string value = parameter->get();
    xml.addValue("Value", ofToString(value));
}