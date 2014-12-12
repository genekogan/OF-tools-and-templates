#pragma once

#include "ofMain.h"
#include "MantaController.h"

#include "ofxAudioUnit.h"


class AudioUnitMantaController {
public:
    void setup(MantaController *manta, ofxAudioUnitSampler *au) {
        this->manta = manta;
        this->au = au;
        setActive(true);
    }
    
    void setActive(bool active);
    
private:
    
    enum NoteMode   { MANUAL, AUTO };
    enum NoteType   { NOTE_ON, NOTE_OFF, NOTE_AUTO };
    enum OutputMode { PARAMETERS, NOTES };
    
    struct ParameterMapping {
        string name;
        int parameterId;
        float min, max;
        float rmin, rmax;
    };
    

    
    void mantaPadEvent(ofxMantaEvent &e);
    void mantaPadVelocityEvent(ofxMantaEvent &e);
    void mantaSliderEvent(ofxMantaEvent &e);
    void mantaButtonEvent(ofxMantaEvent &e);
    void guiParametersEvent(ofxUIEventArgs &e);
    
    MantaController *manta;
    
    ofxAudioUnitSampler *au;
    
    
    
    
    
    map<int, int> noteEvents;
    bool noteStatus[128];
    bool noteManual, sequencerManta, mantaSendNotes, sequencerSmooth;
    int noteAutoFrameLength;
    int noteVelocity;
    
    map<string, vector<string> > parameterGroups;
    vector<string> parameterGroupNames;
    vector<ParameterMapping> parameters;
    map<int, ParameterMapping> mantaMap, seqMap;
    
    ofxUICanvas *guiP, *guiS;
    ofxUIDropDownList *guiParameterGroups, *guiParameters;
    ofxUIRangeSlider *guiParameterRange;
    ofxUITextInput *guiParameterMin, *guiParameterMax;
    
    int guiActiveManta, guiActiveSeq;
    bool guiActiveIsSequencer;
    bool guiToSwitchParameters;
    string guiToSwitchParametersName;
    
    int x, y;
    bool visible;

    
    
    
    
    bool active;
};