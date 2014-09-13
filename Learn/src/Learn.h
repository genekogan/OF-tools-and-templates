#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxLearn.h"
#include "LearnParameter.h"
#include "Presets.h"



class Learn
{
public:
    void setup();
    void update();
    void draw();
    
    void addInput(ofxParameter<float> *parameter);
    void addInput(string name, float min, float max);

    void addOutput(ofxParameter<float> *parameter);
    void addOutput(string name, float min, float max);
    
private:
    void buildGuiMenu();
    void buildGuiInputs();
    void buildGuiOutputs();
    void buildGuiPreview();
    
    void guiTrainEvent(ofxUIEventArgs &e);
    void guiPresetsEvent(ofxUIEventArgs &e);
    void guiInputsEvent(ofxUIEventArgs &e);
    void guiOutputsEvent(ofxUIEventArgs &e);
    
    void startRecording();
    void recordInstance();

    void setPredictingMode(bool predicting);
    
    vector<InputParameter *> inputs;
    vector<OutputParameter *> outputs;
    
    ofxUIScrollableCanvas *guiI;
    ofxUIScrollableCanvas *guiO;
    ofxUIScrollableCanvas *guiIP, *guiOP;
    ofxUICanvas *guiTrain, *guiPresets;
    
    float startTime, trainCountdown, trainDuration;
    int instanceRate, framesPerInstance;
    bool recording, predicting;
    bool inputsVisible;
};
