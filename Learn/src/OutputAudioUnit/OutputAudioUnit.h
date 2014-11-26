#pragma once

#include "ofMain.h"
#include "ofxAudioUnit.h"
#include "ofxUI.h"
#include "Learn.h"


class OutputAudioUnit
{
public:
    ~OutputAudioUnit();
    OutputAudioUnit();
    
    void setupLearn(Learn *learn) {this->learn = learn;}
    void setupAudioUnit(ofxAudioUnitSampler *audioUnit);

    void update();
    void draw();

    void setGuiPosition(int x, int y);
    
protected:
    
    struct ParameterMapping {
        string name;
        int parameterId;
        float min, max;
        float *val;
    };
    
    void setupGui();
    void guiEvent(ofxUIEventArgs &e);
    void setupGuiParameterGroup(string parameterGroupName);
    void resetParameterMappings();
    
    Learn *learn;
    ofxAudioUnitSampler *audioUnit;

    map<string, vector<string> > parameterGroups;
    vector<string> parameterGroupNames;
    vector<ParameterMapping> parameters;
    vector<ParameterMapping*> activeParameters;
    
    ofxUICanvas *gui;
    ofxUIDropDownList *guiParameterGroups, *guiParameters;
    bool visible;
    bool guiToSwitchParameters;
    string guiToSwitchParametersName;
};
