#pragma once

#include "ofMain.h"
#include "Parameter.h"
#include "ofxLearn.h"


enum LearnType  { PARAMETER, OSC, MIDI, MANTA, KINECT };


class LearnParameter
{
public:
    LearnParameter(string name, float min, float max) {
        this->type = OSC;
        this->name = name;
        this->min = min;
        this->max = max;
        value = (min + max) * 0.5;
        address = "osc/"+name;
    }
    
    LearnParameter(ofxParameter<float> *parameter) {
        this->type = PARAMETER;
        this->parameter = parameter;
        this->name = parameter->getName();
        this->min = parameter->getMin();
        this->max = parameter->getMax();
        this->value = parameter->get();
    }
    
    LearnType getType() { return type; }
    
    string getName() { return name; }
    void setName(string name) { this->name = name; }
    
    void setAddress(string address) { this->address = address; }
    string getAddress() { return address; }
    
    void setValue(float value) { this->value = value; }
    float getValue() { return value; }
    float& getValueRef() { return value; }
    
    float getMin() { return min; }
    float getMax() { return max; }
    
private:
    LearnType type;
    
    string name;
    string address;
    
    float min;
    float max;
    float value;

    ofxParameter<float> *parameter;
};




class InputParameter : public LearnParameter
{
public:
    InputParameter(string name, float min, float max) : LearnParameter(name, min, max) {
    }
    
    InputParameter(ofxParameter<float> *parameter) : LearnParameter(parameter) {
        parameter->addListener(this, &InputParameter::parameterChanged);
    }
    
    void parameterChanged(float &v) {
        setValue(v);
    }

};


class OutputParameter : public LearnParameter
{
public:
    OutputParameter(string name, float min, float max) : LearnParameter(name, min, max) {
    }
    OutputParameter(ofxParameter<float> *parameter) : LearnParameter(parameter) {
    }

    int getNumExamples() { return 5; }
    bool& getRecordingRef() { return isRecording; }

private:
    ofxLearn learn;
    bool isRecording;
};

