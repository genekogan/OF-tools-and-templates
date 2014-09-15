#pragma once

#include "ofMain.h"
#include "Parameter.h"
#include "ofxLearn.h"
#include "ofxSpreadsheet.h"


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
    
    void setMin(float min) { this->min = min; }
    void setMax(float max) { this->max = max; }
    
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
    InputParameter(string name, float min, float max) : LearnParameter(name, min, max) {}
    
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
        data.setup(560, 480);
    }

    OutputParameter(ofxParameter<float> *parameter) : LearnParameter(parameter) {
        data.setup(560, 480);
    }
    
    void setRecording(bool recording) {
        this->recording = recording;
    }

    bool getRecording() { return recording; }

    bool& getRecordingRef() { return recording; }

    void addInput(InputParameter *input) {
        inputs.push_back(input);
        vector<string> header;
        for (int i=0; i<inputs.size(); i++)
            header.push_back(inputs[i]->getName());
        data.setHeaders(header);
    }

    void clearInputs() {
        inputs.clear();
    }
    
    void addTrainingInstance() {
        data.addEntry(grabFeatureVector<float>());
    }
    
    void predict() {
        setValue(learn.predict(grabFeatureVector<double>()));
    }
    
    void clearTrainingExamples() {
        learn.clearTrainingInstances();
    }
    
    void trainClassifierFast() {
        learn.trainClassifier(FAST);
        trained = true;
    }
    
    void trainClassifierAccurate() {
        learn.trainClassifier(ACCURATE);
        trained = true;
    }
    
    bool getTrained() {
        return trained;
    }
    
    int getNumExamples() {
        return data.getNumberOfEntries();
    }
    
    vector<InputParameter *> & getInputs() {
        return inputs;
    }
    
    void drawSpreadsheet(int x, int y) {
        data.draw(x, y);
    }
    
    
private:
    
    template <typename T>
    vector<T> grabFeatureVector() {
        vector<T> instance;
        for (int i=0; i<inputs.size(); i++)
            instance.push_back(inputs[i]->getValue());
        return instance;
    }

    ofxLearn learn;
    vector<InputParameter *> inputs;
    ofxSpreadsheet data;
    bool recording, predicting, trained;
};

