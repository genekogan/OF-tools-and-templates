#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "LearnParameter.h"
#include "Control.h"



class Learn
{
public:
    Learn();
    void update();
    void draw();
    
    void setupOscInputs(int port=-1);
    void setupOscOutputs(string host="", int port=-1);
    void setupGui();
    
    virtual void addInput(string name, float *value, float min, float max);
    virtual void addOutput(string name, float *value, float min, float max);
    void addInput(string name, float min, float max);
    void addOutput(string name, float min, float max);
    
    void setVisible(bool visible);
    void setGuiInputsVisible(bool visible);
    void setGuiOutputsVisible(bool visible);
    void toggleVisible() {setVisible(!visible);}
    
    void inputParameterChanged(LearnParameter & input);
    void inputParameterDeleted(LearnParameter & input);
    void outputParameterChanged(LearnParameter & output);
    void outputParameterDeleted(LearnParameter & output);

    virtual void resetInputs();
    
protected:

    void setupTouchOsc();
    
    void gui1Event(ofxUIEventArgs &e);
    void gui2Event(ofxUIEventArgs &e);
    void gui3Event(ofxUIEventArgs &e);

    void startRecording();
    void stopRecording();
    void recordInstance();
    void trainClassifiers(string trainStrategy);
    void setPredictingMode(bool predicting);
    
    vector<LearnInputParameter *> inputs;
    vector<LearnOutputParameter *> outputs;
    
    OscManager oscManager;
    bool oscActive;
    string oscOutputHost;
    int oscOutputPort, oscInputPort;
    
    ofxUICanvas *gui1, *gui2, *gui3;
    bool visible;
    
    float startTime, trainCountdown, trainDuration;
    int instanceRate, framesPerInstance, currentNewInstances;
    bool inRecording, countingDown, recording, predicting;
    int newInputCounter, newOutputCounter;
};
