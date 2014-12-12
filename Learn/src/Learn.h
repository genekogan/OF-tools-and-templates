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
    
    // managing inputs
    virtual LearnInputGroup * addInput(string name, float *value, float min, float max, bool rangeLocked=false);
    virtual LearnInputGroup * addInputGroup(string name);
    virtual LearnInputGroup * addInput(string name, float min, float max);
    LearnInputParameter * addInputToGroup(string groupName, string parameterName, float *value, float min, float max, bool rangeLocked=false);
    LearnInputParameter * addInputToGroup(string groupName, string parameterName, float min, float max, bool rangeLocked=false);
    void addParametersAsInputGroup(string name, vector<LearnInputParameter*> &newInputs);
    vector<LearnInputGroup*> * getInputs() {return &inputs;}
    int getNumberOfInputGroups() {return inputs.size();}
    int getNumberOfInputParameters();
    void clearInputs();
    void clearOutputs();

    // managing outputs
    virtual LearnOutputParameter * addOutput(string name, float *value, float min, float max, bool rangeLocked=false);
    virtual LearnOutputParameter * addOutput(string name, float min, float max);
    void removeInputGroup(string name);
    vector<LearnOutputParameter*> * getOutputs() {return &outputs;}
    
    // osc
    void setupOscSender(string host, int port);
    void setupOscReceiver(int port);
    
    // visibility
    void setVisible(bool visible);
    void setGuiInputsVisible(bool visible);
    void setGuiOutputsVisible(bool visible);
    void setGuiSummaryView(bool viewSummary);
    void toggleVisible() {setVisible(!visible);}
    void toggleGuiInputsVisible() {setGuiInputsVisible(!inputsVisible);}
    void toggleGuiOutputsVisible() {setGuiOutputsVisible(!outputsVisible);}
    void toggleGuiSummaryView() {setGuiSummaryView(!summaryVisible);}
    
    // gui
    void setGuiInputPosition(int x, int y, int h);
    void setGuiOutputPosition(int x, int y, int h);
    void setGuiInputSelectorPosition(int x, int y);
    
    // touch osc
    void saveInputsToTouchOsc();
    void saveOutputsToTouchOsc();
    void saveInputsAndOutputsToTouchOsc();
    
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
    
    // managing inputs + outputs
    virtual void resetInputs();
    virtual void resetOutputs();
    void initializeInput(LearnInputParameter *input);
    void initializeOutput(LearnOutputParameter *output, bool sendOsc=true, bool receiveOsc=true);
    void addInputFeatureSet(string groupName);
    void addSingleInputFeature(string name, float * value, float min, float max);
    
    // osc events
    void enableOscInputs(bool enable);
    void enableOscOutputs(bool enable);
    void oscEventSetRecording(bool &b);
    void oscEventSetTrainFast(bool &b);
    void oscEventSetTrainAccurate(bool &b);
    void oscEventSetPredicting(bool &b);
    
    // gui events
    virtual void inputParameterChanged(LearnParameter & input);
    virtual void inputParameterDeleted(LearnParameter & input);
    virtual void inputGroupDeleted(LearnInputGroup & input);
    virtual void outputParameterChanged(LearnParameter & output);
    virtual void outputParameterDeleted(LearnParameter & output);
    virtual void outputParameterViewed(LearnOutputParameter & output);
    virtual void parameterSelected(LearnParameter & parameter);

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
    
    // analyzer
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
    vector<LearnInputGroup *> inputs;
    vector<LearnOutputParameter *> outputs;
    map<string, vector<InputFeature> > inputFeatures;
    
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
    Analyze analyze;
};
