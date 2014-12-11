#pragma once

#include "ofMain.h"
#include "Control.h"
#include "LearnParameter.h"
#include "Analyze.h"
#include "ThreadedLearner.h"


#define DEFAULT_INPUT_X 10
#define DEFAULT_INPUT_Y 75
#define DEFAULT_INPUT_H 55
#define DEFAULT_OUTPUT_X 420
#define DEFAULT_OUTPUT_Y 75
#define DEFAULT_OUTPUT_H 55
#define DEFAULT_OUTPUT_SELECTOR_X 218
#define DEFAULT_OUTPUT_SELECTOR_Y 75




class Learn
{
public:
    ~Learn();
    Learn(bool init=true);
    
    void update();
    void draw();
    
    // managing parameters
    ////////////////////////////////////////////////////
    /*
     virtual LearnInputParameter  * addInput (string name, float *value, float min, float max, bool rangeLocked=false);
     virtual LearnOutputParameter * addOutput(string name, float *value, float min, float max, bool rangeLocked=false);
     virtual LearnInputParameter  * addInput (string name, float min, float max);
     virtual LearnOutputParameter * addOutput(string name, float min, float max);
     void addParameterAsInput(string name, LearnInputParameter* newInput);
     void addParametersAsInput(string name, vector<LearnInputParameter*> &newInputs);
     void removeParameterAsInput(string name);
     void removeParameterGroupAsInput(string name);
     void initializeOutput(LearnOutputParameter *output, bool sendOsc=true, bool receiveOsc=true);
     vector<LearnInputParameter*> * getInputs() {return &inputs;}
     vector<LearnOutputParameter*> * getOutputs() {return &outputs;}
     void clearInputs();
     void clearOutputs();
     */
    virtual LearnInputGroup * addInput(string name, float *value, float min, float max, bool rangeLocked=false);
    virtual LearnInputGroup * addInputGroup(string name);
    virtual LearnInputGroup * addInput(string name, float min, float max);
    void addInputToGroup(string groupName, string parameterName, float *value, float min, float max, bool rangeLocked=false);
    void addInputToGroup(string groupName, string parameterName, float min, float max, bool rangeLocked=false);
    
    
    virtual LearnOutputParameter * addOutput(string name, float *value, float min, float max, bool rangeLocked=false);
    virtual LearnOutputParameter * addOutput(string name, float min, float max);
    
    //void addParameterAsInput(string name, LearnInputParameter* newInput);
    void addParametersAsInputGroup(string name, vector<LearnInputParameter*> &newInputs);
    
    void removeInputGroup(string name);
    //void removeParameterGroupAsInput(string name);
    
    void initializeInput(LearnInputParameter *input);
    void initializeOutput(LearnOutputParameter *output, bool sendOsc=true, bool receiveOsc=true);
    //vector<LearnInputParameter*> * getInputs() {return &inputs;}
    vector<LearnInputGroup*> * getInputs() {return &inputs;}
    vector<LearnOutputParameter*> * getOutputs() {return &outputs;}
    void clearInputs();
    void clearOutputs();
    
    
    int getNumberOfInputParameters() {
        int n = 0;
        for (int i=0; i<inputs.size(); i++) {
            n += inputs[i]->getInputs().size();
        }
        return n;
    }
    
    ////////////////////////////////////////////////////
    
    
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
    
    // gui
    void setGuiInputPosition(int x, int y, int h);
    void setGuiOutputPosition(int x, int y, int h);
    void setGuiInputSelectorPosition(int x, int y);
    
    // touch osc
    void saveInputsToTouchOsc();
    void saveOutputsToTouchOsc();
    void saveInputsAndOutputsToTouchOsc();
    
    // events
    virtual void inputParameterChanged(LearnParameter & input);
    virtual void inputParameterDeleted(LearnParameter & input);
    virtual void inputGroupDeleted(LearnInputGroup & input);
    virtual void outputParameterChanged(LearnParameter & output);
    virtual void outputParameterDeleted(LearnParameter & output);
    virtual void outputParameterViewed(LearnOutputParameter & output);
    virtual void parameterSelected(LearnParameter & parameter);
    
    // presets
    virtual bool savePreset(string filename="");
    virtual void loadPreset(string path);
    
    
protected:
    
    // input feature mapppings
    struct InputFeature {
        float *value;
        float min, max;
        string name;
        InputFeature(float *value, float min, float max, string name="") {
            this->value = value;
            this->min = min;
            this->max = max;
            this->name = name;
        }
    };
    
    // managing parameters
    virtual void resetInputs();
    //    virtual void resetInputGroups();
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
    void loadHelperGetParameters(ofXml &xml);
    virtual void resetPresets();
    
    // style
    void setFont(string path);
    void setFontSizes(int small, int medium, int large);
    
    
    ////////////////////////////////////////////////////
    // parameters and groups
    
    //vector<LearnInputParameter *> inputs;
    vector<LearnInputGroup *> inputs;
    vector<LearnOutputParameter *> outputs;
    //vector<LearnOutputParameter::GuiInputGroup> inputGroups;
    map<string, vector<InputFeature> > inputFeatures;
    
    ////////////////////////////////////////////////////
    
    
    // osc management
    OscManager oscManager;
    bool oscInActive, oscOutActive;
    string oscOutputHost;
    int oscOutputPort, oscInputPort;
    
    // gui
    ofxUICanvas *gui1, *gui2, *gui3;
    ofxUIDropDownList *guiSelector;
    ofxUILabel *guiStatusLabel;
    int guiInputX, guiInputY, guiInputH;
    int guiOutputX, guiOutputY, guiOutputH;
    int guiOutputSelectorX, guiOutputSelectorY;
    bool visible, inputsVisible, outputsVisible;
    int summaryX, summaryY;
    bool summaryVisible, dragging;
    int draggedFrames;
    int activeOutput;
    
    //vector<int> activeInputs;
    vector<LearnInputParameter *> activeInputs;
    
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
    //Analyze analyze;
    Analyze analyze;
};
