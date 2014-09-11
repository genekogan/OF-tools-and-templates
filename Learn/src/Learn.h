#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxLearn.h"
#include "Inputs.h"
#include "Outputs.h"
#include "LearnParameter.h"
#include "Presets.h"



class Learn
{
public:
    void setup();
    void update();
    void draw();
    
    void buildGuiInputs();
    void buildGuiOutputs();

    
    void addInput(ofxParameter<float> *parameter) {
        InputParameter *newInput = new InputParameter(parameter);
        inputs.push_back(newInput);
        buildGuiInputs();
    }
    void addInput(string name, float min, float max) {
        InputParameter *newInput = new InputParameter(name, min, max);
        inputs.push_back(newInput);
        buildGuiInputs();
    }
    
    void addOutput(string name, float min, float max) {
        OutputParameter *newOutput = new OutputParameter(name, min, max);
        outputs.push_back(newOutput);
        buildGuiOutputs();
    }
    
private:
    vector<InputParameter *> inputs;
    vector<OutputParameter *> outputs;
    
    ofxUIScrollableCanvas *guiI;
    ofxUIScrollableCanvas *guiO;

};
