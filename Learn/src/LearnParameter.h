#pragma once

#include "ofMain.h"
#include "ofxLearn.h"
#include "ofxSpreadsheet.h"
#include "Control.h"


#define DEFAULT_LEARN_TYPE "MLP"  // SVM, MLP
#define DEFAULT_MLP_HIDDEN_LAYERS 2
#define DEFAULT_MLP_TARGET_RMSE 0.001
#define DEFAULT_MLP_MAX_SAMPLES 200000



// spreadhseet, normalization, etc
// what to do about svm





//-----------
class LearnParameter : public Parameter<float>
{
public:
    LearnParameter(string name, float *value, float min=0, float max=1);
    ~LearnParameter();
    
    void set(float value);
    void setName(string name);
    void setOscAddress(string oscAddress);
    void setMin(float min);
    void setMax(float max);
    
    virtual void setVisible(bool visible);
    void setGuiPosition(int x, int y) {gui->setPosition(x, y);}
    bool isVisible() {return visible;}
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
    
protected:
    
    virtual void setupGui() { }

    void guiEvent(ofxUIEventArgs &e);
    void guiSetName();
    void guiSetOscAddress();
    void guiSetMin();
    void guiSetMax();
    void guiSetValueText();

    ofxUICanvas *gui;
    ofxUISlider *guiValue;
    ofxUITextInput *guiValueText, *guiMin, *guiMax, *guiOsc, *guiName;
    bool visible;
    
    ofEvent<LearnParameter> pSelectedEvent, pChangedEvent, pDeletedEvent;
};



//-----------
class LearnInputParameter : public LearnParameter
{
public:
    LearnInputParameter(string name, float *value, float min=0, float max=1);

protected:
    void setupGui();
    void guiEvent(ofxUIEventArgs &e);
};



//-----------
class LearnOutputParameter : public LearnParameter
{
public:
    enum LearnModel { SVM, MLP };
    
    struct GuiInputGroup {
        string name;
        vector<LearnInputParameter *> inputs;
    };

    ~LearnOutputParameter();
    LearnOutputParameter(string name, float *value, float min=0, float max=1);
    
    virtual void draw();

    void setInputParameters(vector<LearnInputParameter *> &allInputs);
    void setInputGroups(vector<GuiInputGroup> inputGroups);
    void setupHeaders(int p=-1);
    void addDataPage();
    void setPage(int p);

    void addInstance(vector<float> instance);
    void addInstance();
    void clearInstances();
    int getNumInstances();
    void exportData(string filename="");
    vector<vector<vector<float> > > getInstances();

    void addInput(LearnInputParameter * input);
    bool removeInput(LearnInputParameter * input);
    int getNumInputs() {return activeInputs.size();}
    vector<LearnInputParameter *> & getActiveInputs() {return activeInputs;}
    bool getInputActive(LearnInputParameter * input);
    
    void activateAllInputs();
    
    void setupGui();
    void setupGuiInputSelector();

    void setVisible(bool visible);
    void setGuiPosition(int x, int y);
    void setInputsVisible(bool b);
    void setExamplesVisible(bool b);
    void setDataSize(int width, int height);

    virtual void setTrained(bool trained);
    virtual void setRecording(bool record);
    bool getRecording() {return record;}
    bool getTrained() {return trained;}
    
    void setTrainingMlp(int numHiddenLayers, float targetRmse, int maxSamples);
    void setTrainingSvm();
    void trainClassifier(TrainMode trainMode);
    void trainClassifierFast() {trainClassifier(FAST);}
    void trainClassifierAccurate() {trainClassifier(ACCURATE);}

    void setMlpNumHiddenLayers(int n) {return learn.setMlpNumHiddenLayers(n);}
    void setMlpMaxSamples(int n) {learn.setMlpMaxSamples(n);}
    void setMlpTargetRmse(float r) {learn.setMlpTargetRmse(r);}

    LearnModel getLearnModel() {return learnModel;}
    int getMlpNumHiddenLayers() {return learn.getMlpNumHiddenLayers();}
    int getMlpMaxSamples() {return learn.getMlpMaxSamples();}
    float getMlpTargetRmse() {return learn.getMlpTargetRmse();}
    vector<double> getMlpCoefficients1() {return mlpCoefficientsW1;}
    vector<double> getMlpCoefficients2() {return mlpCoefficientsW3;}
    
    void setupMlpCoefficients();
    double predictMlp(vector<double> example);
    virtual void predict() {predict(grabFeatureVector<double>(false, true));}
    virtual void predict(vector<double> instance);

    void loadClassifierSvm(string path);
    void loadClassifierMlp(vector<double> w1, vector<double> w3);
    void saveClassifier(string path);
    
    template<typename ListenerClass, typename ListenerMethod>
    void addParameterViewedListener(ListenerClass *listener, ListenerMethod method) {
        ofAddListener(pViewedEvent, listener, method);
    }
    
    virtual void setFont(string path);
    virtual void setFontSizes(int small, int medium, int large);
    
protected:
    inline double sigmoid(double x);

    template <typename T> vector<T> grabFeatureVector(bool labelFirst, bool normalize=false);
    void addSpreadsheetDataToLearn();
    
    void guiEvent(ofxUIEventArgs &e);
    void guiInputSelectEvent(ofxUIEventArgs &e);
    void guiDataEvent(ofxUIEventArgs &e);
    void dataChangedEvent(bool &b);

    ofxUICanvas *guiInputSelect, *guiData;
    ofxUIDropDownList *guiSelector;
    ofxUILabelToggle *guiInputs, *guiExamples;
    ofxUILabelButton *guiDataStatus, *guiDataPage;
    
    ofxLearn learn;
    LearnModel learnModel;
    int mlpNumHiddenLayers, mlpMaxSamples;
    float mlpTargetRmse;
    vector<double> mlpCoefficientsW1, mlpCoefficientsW3;
    vector<LearnInputParameter *> allInputs, activeInputs;
    vector<GuiInputGroup> inputGroups;

    vector<ofxSpreadsheet *> data;
    int page, dataWidth, dataHeight;
    
    bool record, trained, training;
    bool viewExamples, viewInputs;
    bool inputGroupsEnabled;
    
    ofEvent<LearnOutputParameter> pViewedEvent;
};


//--------
template <typename T> vector<T> LearnOutputParameter::grabFeatureVector(bool labelFirst, bool normalize) {
    vector<T> instance;
    if (labelFirst) instance.push_back((T) get());
    for (int i=0; i<activeInputs.size(); i++) {
        //instance.push_back(activeInputs[i]->get());
        if (normalize) {
            float val = (activeInputs[i]->get() - activeInputs[i]->getMin()) / (activeInputs[i]->getMax() - activeInputs[i]->getMin());
            instance.push_back(val);
        }
        else {
            instance.push_back(activeInputs[i]->get());
        }
    }
    return instance;
}


