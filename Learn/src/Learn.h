#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxLearn.h"
#include "LearnParameter.h"
#include "Presets.h"



class Learn
{
public:
    Learn();
    ~Learn();
    void update();
    void draw();
    
    void addInput(ofxParameter<float> *parameter);
    void addInput(string name, float min, float max);

    void addOutput(ofxParameter<float> *parameter);
    void addOutput(string name, float min, float max);
    
private:
    void buildGuiInputOutput();
    void buildGuiMenu();
    void buildGuiInputs();
    void buildGuiOutputs();
    void buildGuiPreview();
    void buildGuiInputSelector(int idx);
    
    void guiTrainEvent(ofxUIEventArgs &e);
    void guiMenuEvent(ofxUIEventArgs &e);
    void guiInputsEvent(ofxUIEventArgs &e);
    void guiOutputsEvent(ofxUIEventArgs &e);
    void guiInputSelectorEvent(ofxUIEventArgs &e);
    
    void startRecording();
    void recordInstance();
    void trainClassifiers(string trainStrategy);
    void setPredictingMode(bool predicting);
    
    vector<InputParameter *> inputs;
    vector<OutputParameter *> outputs;
    
    ofxUIScrollableCanvas *guiI;
    ofxUIScrollableCanvas *guiO;
    ofxUIScrollableCanvas *guiIP;
    ofxUIScrollableCanvas *guiOP;
    ofxUICanvas *guiTrain;
    ofxUICanvas *guiMenu;
    ofxUICanvas *guiInputSelector;
    
    float startTime, trainCountdown, trainDuration;
    int instanceRate, framesPerInstance;
    bool recording, predicting;
    bool inputsVisible, dataVisible;
    int idxSelectedOutput;
};
