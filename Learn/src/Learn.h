#pragma once

#include "ofMain.h"
#include "Control.h"
#include "LearnParameter.h"
#include "Analyze.h"
#include "ThreadedLearner.h"


/*
analysis to-do
- auto mapping
- GMM w/ N clusters
- assign the N cluster means to N evenly distributed output values
*/


class Learn
{
public:
    ~Learn();
    Learn(bool init=true);
    
    void update();
    void draw();
    
    // managing parameters
    virtual LearnInputParameter  * addInput (string name, float *value, float min, float max);
    virtual LearnOutputParameter * addOutput(string name, float *value, float min, float max);
    LearnInputParameter  * addInput (string name, float min, float max);
    LearnOutputParameter * addOutput(string name, float min, float max);
    void addParameterAsInput(string name, LearnInputParameter* newInput);
    void addParametersAsInput(string name, vector<LearnInputParameter*> &newInputs);
    void initializeOutput(LearnOutputParameter *output, bool sendOsc=true, bool receiveOsc=true);
    vector<LearnInputParameter*> * getInputs() {return &inputs;}
    vector<LearnOutputParameter*> * getOutputs() {return &outputs;}
    void clearInputs();
    void clearOutputs();
    
    // osc
    void setupOscSender(string host, int port);
    void setupOscReceiver(int port);
    void enableOscInputs(bool enable);
    void enableOscOutputs(bool enable);

    // visibility
    void setVisible(bool visible);
    void setGuiInputsVisible(bool visible);
    void setGuiOutputsVisible(bool visible);
    void toggleVisible() {setVisible(!visible);}
    void toggleGuiInputsVisible() {setGuiInputsVisible(!inputsVisible);}
    void toggleGuiOutputsVisible() {setGuiOutputsVisible(!outputsVisible);}
    void setGuiSummaryView(bool viewSummary);
    void toggleGuiSummaryView() {setGuiSummaryView(!summaryVisible);}
    
    // touch osc
    void saveInputsToTouchOsc();
    void saveOutputsToTouchOsc();
    void saveInputsAndOutputsToTouchOsc();
    
    // events
    virtual void inputParameterChanged(LearnParameter & input);
    virtual void inputParameterDeleted(LearnParameter & input);
    virtual void outputParameterChanged(LearnParameter & output);
    virtual void outputParameterDeleted(LearnParameter & output);
    virtual void outputParameterViewed(LearnOutputParameter & output);
    virtual void parameterSelected(LearnParameter & parameter);
    
    // presets
    virtual bool savePreset(string filename="");
    virtual void loadPreset(string path);

    
protected:
    
    // managing parameters
    virtual void resetInputs();
    virtual void resetInputGroups();
    virtual void resetOutputs();

    // osc events
    void oscEventSetRecording(bool &b);
    void oscEventSetTrainFast(bool &b);
    void oscEventSetTrainAccurate(bool &b);
    void oscEventSetPredicting(bool &b);

    // gui
    virtual void setupGui();
    virtual void resetGuiPositions();
    void gui1Event(ofxUIEventArgs &e);
    void gui2Event(ofxUIEventArgs &e);
    void gui3Event(ofxUIEventArgs &e);
    void mousePressed(ofMouseEventArgs &e);
    void mouseDragged(ofMouseEventArgs &e);
    void mouseReleased(ofMouseEventArgs &e);
    void summaryClickParameters(int x, int y);
    void summaryActiveParameter(int x, int y);
    void summaryActivateParameter(int idx);
    void drawSummary();
    
    // get input mappings
    void startAnalyzer();
    void stopAnalyzer();
    
    // record + learn
    void startRecording();
    void stopRecording();
    void recordInstance();
    void trainClassifiers(string trainStrategy);
    
    // presets
    void saveInputs(string filename, ofXml &xml);
    void saveOutputs(string filename, ofXml &xml);
    string loadPresetDialog(string filename);
    void loadInputs(ofXml &xml);
    void loadOutputs(ofXml &xml, bool loadExamples=true, bool loadClassifier=true);
    virtual void resetPresets();
    
    // style
    void setFont(string path);
    void setFontSizes(int small, int medium, int large);
    
    // parameters and groups
    vector<LearnInputParameter *> inputs;
    vector<LearnOutputParameter *> outputs;
    vector<LearnOutputParameter::GuiInputGroup> inputGroups;
    
    // osc management
    OscManager oscManager;
    bool oscInActive, oscOutActive;
    string oscOutputHost;
    int oscOutputPort, oscInputPort;
    
    // gui
    ofxUICanvas *gui1, *gui2, *gui3;
    ofxUIDropDownList *guiSelector;
    ofxUILabel *guiStatusLabel;
    bool visible, inputsVisible, outputsVisible;
    int summaryX, summaryY;
    bool summaryVisible, dragging;
    int draggedFrames;
    int activeOutput;
    vector<int> activeInputs;
    
    // recording variables
    float startTime, trainCountdown, trainDuration;
    int instanceRate, framesPerInstance, currentNewInstances;
    bool inRecording, analyzing, countingDown, recording, predicting, training;
    int newInputCounter, newOutputCounter;
    
    // style
    bool customFont;
    string fontPath;
    int fontSmall, fontMedium, fontLarge;
    
    // learn
    ThreadedLearner threadedLearn;
    string trainStrategy;
    int idxTraining;
    
    // analyze
    Analyze analyze;
};
