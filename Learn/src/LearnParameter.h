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
        setupSpreadsheet(480, 480);
    }

    OutputParameter(ofxParameter<float> *parameter) : LearnParameter(parameter) {
        setupSpreadsheet(480, 480);
    }

    void setupSpreadsheet(int width, int height) {
        data.setup(width, height);
        data.highlightColumn(0);
    }

    void setRecording(bool recording) {
        this->recording = recording;
    }

    bool getRecording() { return recording; }

    bool& getRecordingRef() { return recording; }

    void addInput(InputParameter *input) {
        inputs.push_back(input);
        vector<string> header;
        header.push_back(getName());
        for (int i=0; i<inputs.size(); i++)
            header.push_back(inputs[i]->getName());
        data.setHeaders(header);
    }

    void clearInputs() {
        inputs.clear();
    }
    
    void addTrainingInstance() {
        data.addEntry(grabLabelFeatureVector<float>());
    }
    
    void predict() {
        setValue(learn.predict(grabLabelFeatureVector<double>()));
    }
    
    void clearTrainingExamples() {
        data.clear();
    }
    
    void trainClassifierFast() {
        addSpreadsheetDataToLearn();
        learn.trainClassifier(FAST);
        trained = true;
    }
    
    void trainClassifierAccurate() {
        addSpreadsheetDataToLearn();
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
    vector<T> grabLabelFeatureVector() {
        vector<T> instance;
        instance.push_back((T) getValue());
        for (int i=0; i<inputs.size(); i++)
            instance.push_back(inputs[i]->getValue());
        return instance;
    }

    void addSpreadsheetDataToLearn() {
        vector<vector<float> > entries = data.getEntries();
        for (int i=0; i<entries.size(); i++) {
            double label = (double) entries[i][0];
            vector<double> instance;
            for (int j=1; j<entries[i].size(); j++) {
                instance.push_back((double) entries[i][j]);
            }
            learn.addTrainingInstance(instance, label);
        }
    }
    
    ofxLearn learn;
    vector<InputParameter *> inputs;
    ofxSpreadsheet data;
    bool recording, predicting, trained;
};

