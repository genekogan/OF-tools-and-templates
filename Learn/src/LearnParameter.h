#pragma once

#include "ofMain.h"
#include "ofxLearn.h"
#include "ofxSpreadsheet.h"
#include "Control.h"



#define DEFAULT_LEARN_TYPE "MLP"  // SVM, MLP
#define DEFAULT_MLP_HIDDEN_LAYERS 2
#define DEFAULT_MLP_TARGET_RMSE 0.001
#define DEFAULT_MLP_MAX_SAMPLES 1000000

// hack to fix ofxUI rect width bug
#define GUI_INPUT_WIDTH 386
#define GUI_OUTPUT_WIDTH 586




//-----------
class LearnParameter : public Parameter<float>
{
public:
    LearnParameter(string name, float *value, float min=0, float max=1, bool rangeLocked=false);
    ~LearnParameter();
    
    void set(float value);
    void setName(string name);
    void setOscAddress(string oscAddress);
    void setMin(float min);
    void setMax(float max);
    void setWarp(float warp);
    void setReference(float *value);
    
    virtual void setVisible(bool visible);
    void setGuiPosition(int x, int y) {gui->setPosition(x, y);}
    bool isVisible() {return visible;}
    void updateGui();
    void deselect();

    template<typename ListenerClass, typename ListenerMethod>
    void addParameterSelectedListener(ListenerClass *listener, ListenerMethod method) {
        ofAddListener(pSelectedEvent, listener, method);
    }
    template<typename ListenerClass, typename ListenerMethod>
    void addParameterChangedListener(ListenerClass *listener, ListenerMethod method) {
        ofAddListener(pChangedEvent, listener, method);
    }
    template<typename ListenerClass, typename ListenerMethod>
    void addParameterDeletedListener(ListenerClass *listener, ListenerMethod method) {
        ofAddListener(pDeletedEvent, listener, method);
    }

    virtual void setFont(string path);
    virtual void setFontSizes(int small, int medium, int large);
    
    void setRangeLocked(bool rangeLocked) {this->rangeLocked = rangeLocked;}
    bool getRangeLocked() {return rangeLocked;}
    
protected:
    
    virtual void setupGui() { }

    void guiEvent(ofxUIEventArgs &e);
    void guiSetName();
    void guiSetOscAddress();
    void guiSetMin();
    void guiSetMax();
    void guiSetWarp();
    void guiSetValueText();

    bool rangeLocked;
    
    ofxUICanvas *gui;
    ofxUISlider *guiValue;
    ofxUITextInput *guiValueText, *guiMin, *guiMax, *guiWarp, *guiOsc, *guiName;
    bool visible;
    
    ofEvent<LearnParameter> pSelectedEvent, pChangedEvent, pDeletedEvent;
};



//-----------
class LearnInputParameter : public LearnParameter
{
public:
    LearnInputParameter(string name, float *value,
                        float min=0, float max=1,
                        bool rangeLocked=false);
protected:
    void setupGui();
    void guiEvent(ofxUIEventArgs &e);
};




//-----------
class LearnInputGroup
{
public:
    LearnInputGroup(string name);

    vector<LearnInputParameter *> & getInputs() { return inputs;}

    LearnInputParameter * addInputParameter(string name, float *value, float min, float max, bool rangeLocked=false);
    void addInputParameter(string name, float min, float max, bool rangeLocked=false);
    void addInputParameter(LearnInputParameter * newInput);
    void addInputParameters(vector<LearnInputParameter *> newInputs);
    void removeParameter(LearnInputParameter * parameter);
    
    void clearParameters() {
        for (int i=0; i<inputs.size(); i++) {
            delete inputs[i];
        }
        inputs.clear();
    }
    
    void setName(string name) {this->name = name;}
    string getName() {return name;}
    
protected:
    
    vector<LearnInputParameter *> inputs;
    string name;
};




//-----------
class LearnOutputParameter : public LearnParameter
{
public:
    enum LearnModel { SVM, MLP };
    
    ~LearnOutputParameter();
    LearnOutputParameter(string name, float *value, float min=0, float max=1, bool rangeLocked=false);
    
    virtual void draw();
    
    // manage inputs
    void addInput(LearnInputGroup * input);
    bool removeInput(LearnInputGroup * input);
    vector<LearnInputGroup *> & getActiveInputs() {return activeInputs;}
    int getNumInputs() {return activeInputs.size();}
    int getNumParameters();
    bool getInputActive(LearnInputGroup * input);
    void activateAllInputs();
    
    // manage data
    void setInputParameters(vector<LearnInputGroup *> &allInputs);
    void addInstance(vector<float> instance);
    void addInstance();
    void clearInstances();
    int getNumInstances();
    void exportData(string filename="");
    vector<vector<vector<float> > > getInstances();

    // spreadsheet data
    void setupHeaders(int p=-1);
    void addDataPage();
    void setPage(int p);

    // visibility
    void setVisible(bool visible);
    void setGuiPosition(int x, int y);
    void setGuiInputsPosition(int x, int y);
    void setInputsVisible(bool b);
    void setExamplesVisible(bool b);
    void setDataSize(int width, int height);

    // set training/recording status
    virtual void setTrained(bool trained);
    virtual void setRecording(bool record);
    void toggleRecording() {setRecording(!record);}
    bool getRecording() {return record;}
    bool getTrained() {return trained;}
    
    // learn/manage model + data
    void setTrainingMlp(int numHiddenLayers, float targetRmse, int maxSamples);
    void setTrainingSvm();
    void setMlpNumHiddenLayers(int n) {return learn.setMlpNumHiddenLayers(n);}
    void setMlpMaxSamples(int n) {learn.setMlpMaxSamples(n);}
    void setMlpTargetRmse(float r) {learn.setMlpTargetRmse(r);}
    void trainClassifier(TrainMode trainMode);
    void trainClassifierFast() {trainClassifier(FAST);}
    void trainClassifierAccurate() {trainClassifier(ACCURATE);}
    LearnModel getLearnModel() {return learnModel;}
    int getMlpNumHiddenLayers() {return learn.getMlpNumHiddenLayers();}
    int getMlpMaxSamples() {return learn.getMlpMaxSamples();}
    float getMlpTargetRmse() {return learn.getMlpTargetRmse();}
    vector<double> getMlpCoefficients1() {return mlpCoefficientsW1;}
    vector<double> getMlpCoefficients2() {return mlpCoefficientsW3;}
    
    // learn/prediction
    virtual void predict();
    virtual void predict(vector<double> instance);
    
    // save load
    void loadClassifierSvm(string path);
    void loadClassifierMlp(vector<double> w1, vector<double> w3);
    void saveClassifier(string path);
    
    // style
    virtual void setFont(string path);
    virtual void setFontSizes(int small, int medium, int large);
    
    // add listener
    template<typename ListenerClass, typename ListenerMethod>
    void addParameterViewedListener(ListenerClass *listener, ListenerMethod method) {
        ofAddListener(pViewedEvent, listener, method);
    }

protected:

    inline double sigmoid(double x);

    // manage data
    template <typename T> vector<T> grabFeatureVector(bool labelFirst, bool normalize=false);
    void addSpreadsheetDataToLearn();

    // learn
    void setupMlpCoefficients();
    double predictMlp(vector<double> example);
    void setDirect();

    // gui
    void setupGui();
    void setupGuiInputSelector();

    // gui events
    void guiEvent(ofxUIEventArgs &e);
    void guiInputSelectEvent(ofxUIEventArgs &e);
    void guiDataEvent(ofxUIEventArgs &e);
    void dataChangedEvent(bool &b);
    ofEvent<LearnOutputParameter> pViewedEvent;

    // gui elements
    ofxUICanvas *guiInputSelect, *guiData;
    ofxUIDropDownList *guiSelector;
    ofxUILabelToggle *guiDirect, *guiInputs, *guiExamples, *guiRecord;
    ofxUILabelButton *guiDataStatus, *guiDataPage;
    ofxUISlider *guiLerpRate;
    
    // learn
    ofxLearn learn;
    LearnModel learnModel;
    int mlpNumHiddenLayers, mlpMaxSamples;
    float mlpTargetRmse;
    vector<double> mlpCoefficientsW1, mlpCoefficientsW3;
    float directLerp;

    // inputs
    vector<LearnInputGroup *> allInputs, activeInputs;
    
    //  data/spreadsheet
    vector<ofxSpreadsheet *> data;
    int page, dataWidth, dataHeight;
    
    // status
    bool direct;
    bool record, trained, training;
    bool viewExamples, viewInputs;
};


//--------
template <typename T> vector<T> LearnOutputParameter::grabFeatureVector(bool labelFirst, bool normalize) {
    vector<T> instance;
    if (labelFirst) instance.push_back((T) get());
    for (int i=0; i<activeInputs.size(); i++) {
        vector<LearnInputParameter *> params = activeInputs[i]->getInputs();
        for (int j=0; j<params.size(); j++) {
            float val = normalize ? (params[j]->get() - params[j]->getMin()) / (params[j]->getMax() - params[j]->getMin()) : params[j]->get();
            instance.push_back(val);
        }
    }
    return instance;
}

