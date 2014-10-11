#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "LearnParameter.h"
#include "Control.h"



class Learn
{
public:
    Learn(bool init=true);
    void update();
    void draw();
    
    void enableOscInputs(bool enable);
    void enableOscOutputs(bool enable);
    void setupOscSender(string host, int port);
    void setupOscReceiver(int port);
    
    virtual void setupGui();
    virtual void resetGuiPositions();
    virtual void resetInputs();
    virtual void resetOutputs();
    virtual void resetPresets();

    virtual LearnInputParameter  * addInput (string name, float *value, float min, float max);
    virtual LearnOutputParameter * addOutput(string name, float *value, float min, float max);
    LearnInputParameter  * addInput (string name, float min, float max);
    LearnOutputParameter * addOutput(string name, float min, float max);
    
    void setVisible(bool visible);
    void setGuiInputsVisible(bool visible);
    void setGuiOutputsVisible(bool visible);
    void toggleVisible() {setVisible(!visible);}
    
    void inputParameterChanged(LearnParameter & input);
    void inputParameterDeleted(LearnParameter & input);
    void outputParameterChanged(LearnParameter & output);
    void outputParameterDeleted(LearnParameter & output);
    void outputParameterViewed(LearnOutputParameter & output);
    void parameterSelected(LearnParameter & parameter);
    
    void saveInputsToTouchOsc();
    void saveOutputsToTouchOsc();
    void saveInputsAndOutputsToTouchOsc();
    
    bool savePreset(string filename="");
    void saveInputs(string filename, ofXml &xml);
    void saveOutputs(string filename, ofXml &xml);
    
    string loadPresetDialog(string filename);
    void loadPreset(string path);
    void loadInputs(ofXml &xml);
    void loadOutputs(ofXml &xml);
    
protected:
    
    void gui1Event(ofxUIEventArgs &e);
    void gui2Event(ofxUIEventArgs &e);
    void gui3Event(ofxUIEventArgs &e);
    void oscEventSetRecording(bool &b);
    void oscEventSetPredicting(bool &b);

    void startRecording();
    void stopRecording();
    void recordInstance();
    void trainClassifiers(string trainStrategy);
    void setPredictingMode(bool predicting);
    
    vector<LearnInputParameter *> inputs;
    vector<LearnOutputParameter *> outputs;
    
    OscManager oscManager;
    bool oscInActive, oscOutActive;
    string oscOutputHost;
    int oscOutputPort, oscInputPort;
    
    ofxUICanvas *gui1, *gui2, *gui3;
    ofxUIDropDownList *guiSelector;
    ofxUILabel *guiStatusLabel;
    bool visible, inputsVisible, outputsVisible;
    
    float startTime, trainCountdown, trainDuration;
    int instanceRate, framesPerInstance, currentNewInstances;
    bool inRecording, countingDown, recording, predicting;
    int newInputCounter, newOutputCounter;
};
