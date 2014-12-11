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
//    ofEvent<LearnParameter> pSelectedEvent, pChangedEvent;
};



//-----------
class LearnInputParameter : public LearnParameter
{
public:
    LearnInputParameter(string name, float *value, float min=0, float max=1, bool rangeLocked=false);

protected:
    void setupGui();
    void guiEvent(ofxUIEventArgs &e);
};


//////////////////////////////////////////////////////////////////

//-----------
class LearnInputGroup
{
public:
    LearnInputGroup(string name) {
        this->name = name;
    }
    void addInputParameter(LearnInputParameter * newInput);
    
    void addInputParameters(vector<LearnInputParameter *> newInputs) {
        for (int i=0; i<newInputs.size(); i++) {
            inputs.push_back(newInputs[i]);
        }
    }
    LearnInputParameter * addInputParameter(string name, float *value, float min, float max, bool rangeLocked=false) {
        LearnInputParameter *newInput = new LearnInputParameter(name, value, min, max, rangeLocked);
        inputs.push_back(newInput);
        return newInput;
    }
    void addInputParameter(string name, float min, float max, bool rangeLocked=false) {
        addInputParameter(name, new float(), min, max, rangeLocked);
    }
    
    void removeParameter(LearnInputParameter * parameter) {
        cout << "remove a param ("<<inputs.size()<<") ";
        vector<LearnInputParameter *>::iterator it=inputs.begin();
        while (it != inputs.end()) {
            if (*it == parameter) {
                delete (*it);
                it = inputs.erase(it);
            }
            else ++it;
        }
        cout << "now " << inputs.size() << endl;
    }
    
    vector<LearnInputParameter *> & getInputs() { return inputs;}
    string getName() {return name;}
    void setName(string name) {this->name = name;}
    
protected:
    vector<LearnInputParameter *> inputs;
    string name;
};

//////////////////////////////////////////////////////////////////


//-----------
class LearnOutputParameter : public LearnParameter
{
public:
    enum LearnModel { SVM, MLP };

    /*
    struct GuiInputGroup {
        string name;
        vector<LearnInputParameter *> inputs;
    };
     */
    
    ~LearnOutputParameter();
    LearnOutputParameter(string name, float *value, float min=0, float max=1, bool rangeLocked=false);
    
    virtual void draw();

    //////////////////////////////////////////////////////////////////
    //void setInputParameters(vector<LearnInputParameter *> &allInputs);
    //void setInputGroups(vector<GuiInputGroup> inputGroups);
    void setInputParameters(vector<LearnInputGroup *> &allInputs);
    //////////////////////////////////////////////////////////////////
    
    
    void setupHeaders(int p=-1);
    void addDataPage();
    void setPage(int p);

    void addInstance(vector<float> instance);
    void addInstance();
    void clearInstances();
    int getNumInstances();
    void exportData(string filename="");
    vector<vector<vector<float> > > getInstances();

    //////////////////////////////////////////////////////////////////
//    void addInput(LearnInputParameter * input);
//    bool removeInput(LearnInputParameter * input);
    void addInput(LearnInputGroup * input);
    bool removeInput(LearnInputGroup * input);

    int getNumInputs() {return activeInputs.size();}
    int getNumParameters() {
        int n = 0;
        for (int i=0; i<activeInputs.size(); i++) {
            n += activeInputs[i]->getInputs().size();
        }
        return n;
    }
    //vector<LearnInputParameter *> & getActiveInputs() {return activeInputs;}
    vector<LearnInputGroup *> & getActiveInputs() {return activeInputs;}
    //vector<string> & getActiveInputGroups() {return activeInputGroups;}
    //bool getInputActive(LearnInputParameter * input);
    bool getInputActive(LearnInputGroup * input);
    void activateAllInputs();

    //////////////////////////////////////////////////////////////////

    
    void setupGui();
    void setupGuiInputSelector();
    
    void setVisible(bool visible);
    void setGuiPosition(int x, int y);
    void setGuiInputsPosition(int x, int y);
    void setInputsVisible(bool b);
    void setExamplesVisible(bool b);
    void setDataSize(int width, int height);

    virtual void setTrained(bool trained);
    virtual void setRecording(bool record);
    void toggleRecording() {setRecording(!record);}
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
    virtual void predict();
    virtual void predict(vector<double> instance);

    void setDirect();
    
    void loadClassifierSvm(string path);
    void loadClassifierMlp(vector<double> w1, vector<double> w3);
    void saveClassifier(string path);
    
    virtual void setFont(string path);
    virtual void setFontSizes(int small, int medium, int large);
    
    template<typename ListenerClass, typename ListenerMethod>
    void addParameterViewedListener(ListenerClass *listener, ListenerMethod method) {
        ofAddListener(pViewedEvent, listener, method);
    }

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
    ofxUILabelToggle *guiDirect, *guiInputs, *guiExamples, *guiRecord;
    ofxUILabelButton *guiDataStatus, *guiDataPage;
    ofxUISlider *guiLerpRate;
    
    ofxLearn learn;
    LearnModel learnModel;
    int mlpNumHiddenLayers, mlpMaxSamples;
    float mlpTargetRmse;
    vector<double> mlpCoefficientsW1, mlpCoefficientsW3;
    float directLerp;

    //////////////////////////////////////////////////////////////////////
    //vector<LearnInputParameter *> allInputs, activeInputs;
    //vector<GuiInputGroup> inputGroups;
    //vector<string> activeInputGroups;
    vector<LearnInputGroup *> allInputs, activeInputs;
    ////////////////////////////////////////////////////////
    
    vector<ofxSpreadsheet *> data;
    int page, dataWidth, dataHeight;
    
    bool direct;
    bool record, trained, training;
    bool viewExamples, viewInputs;
    //bool inputGroupsEnabled;
    
    ofEvent<LearnOutputParameter> pViewedEvent;
};


//--------
template <typename T> vector<T> LearnOutputParameter::grabFeatureVector(bool labelFirst, bool normalize) {
    vector<T> instance;
    if (labelFirst) instance.push_back((T) get());
    for (int i=0; i<activeInputs.size(); i++) {

        ////////////////////////////////////////////////////////
        /*
        if (normalize) {
            float val = (activeInputs[i]->get() - activeInputs[i]->getMin()) / (activeInputs[i]->getMax() - activeInputs[i]->getMin());
            instance.push_back(val);
        }
        else {
            instance.push_back(activeInputs[i]->get());
        }
        */
        
        vector<LearnInputParameter *> params = activeInputs[i]->getInputs();
        for (int j=0; j<params.size(); j++) {
            if (normalize) {
                float val = (params[j]->get() - params[j]->getMin()) / (params[j]->getMax() - params[j]->getMin());
                instance.push_back(val);
            }
            else {
                instance.push_back(params[j]->get());
            }
            
            /*
            cout << "GOT FEAT VEC "<<endl;
            for (int k=0; k<instance.size(); k++) {
                cout << instance[k]<<",";
            }
            cout << endl;
            */
        }

        ////////////////////////////////////////////////////////
    }
    return instance;
}


