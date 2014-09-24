#pragma once

#include "ofMain.h"
#include "ofxLearn.h"
#include "ofxSpreadsheet.h"
#include "Control.h"



//-----------
class LearnParameter : public Parameter<float>
{
public:
    LearnParameter(string name, float *value, float min=0, float max=1);
    ~LearnParameter();
    
    virtual void setVisible(bool visible){
        this->visible = visible;
        gui->setVisible(visible);
    }
    
    void setGuiPosition(int x, int y) {gui->setPosition(x, y);}
    bool isVisible() {return visible;}

    template<typename ListenerClass, typename ListenerMethod>
    void addParameterChangeListener(ListenerClass *listener, ListenerMethod method) {
        ofAddListener(parameterChangeEvent, listener, method);
    }
    
    template<typename ListenerClass, typename ListenerMethod>
    void addParameterDeletedListener(ListenerClass *listener, ListenerMethod method) {
        ofAddListener(parameterDeletedEvent, listener, method);
    }
    
protected:
    
    virtual void setupGui() { }
    void guiEvent(ofxUIEventArgs &e);

    ofxUICanvas *gui;
    ofxUISlider *guiValue;
    ofxUITextInput *guiValueText, *guiMin, *guiMax, *guiOsc, *guiName;
    bool visible;
    
    ofEvent<LearnParameter> parameterChangeEvent, parameterDeletedEvent;
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
    ~LearnOutputParameter();
    LearnOutputParameter(string name, float *value, float min=0, float max=1);
    
    virtual void draw();

    void setInputParameters(vector<LearnInputParameter *> &allInputs);
    void setupHeaders();

    void addInstance();
    void clearInstances();
    int getNumInstances() {return data.getNumberOfEntries();}
    int getNumInputs() {return activeInputs.size();}
    
    void setVisible(bool visible);
    void setGuiPosition(int x, int y);
    void setupGui();
    void setupGuiInputSelector();

    bool getRecording() {return record;}
    bool getTrained() {return trained;}
    
    void trainClassifierFast();
    void trainClassifierAccurate();
    void predict();
    
protected:
    
    template <typename T> vector<T> grabFeatureVector(bool labelFirst) {
        vector<T> instance;
        if (labelFirst) instance.push_back((T) get());
        for (int i=0; i<activeInputs.size(); i++) {
            instance.push_back(activeInputs[i]->get());
        }
        return instance;
    }
    
    void addSpreadsheetDataToLearn();
    
    void guiEvent(ofxUIEventArgs &e);
    void guiInputSelectEvent(ofxUIEventArgs &e);

    ofxUICanvas *guiInputSelect;
    ofxUIDropDownList *selector;
    ofxUILabelToggle *guiInputs, *guiExamples;

    ofxLearn learn;
    vector<LearnInputParameter *> allInputs, activeInputs;
    ofxSpreadsheet data;
    
    bool record, trained;
    bool viewExamples, viewInputs;
};

