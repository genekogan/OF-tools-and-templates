#pragma once

#include "ofMain.h"
#include "ofxAudioUnit.h"
#include "ofxUI.h"


class AudioUnitPlayer
{
public:
    struct AudioUnitParameter {
        string name;
        int parameterId;
        float min, max;
        float rmin, rmax;
        float *value;
        AudioUnitParameter() {}
    };

    enum NoteMode   { MANUAL, AUTO };
    enum NoteType   { NOTE_ON, NOTE_OFF, NOTE_AUTO };
    enum OutputMode { PARAMETERS, NOTES };

    AudioUnitPlayer();
    ~AudioUnitPlayer();
    
    virtual void setupAudioUnit(ofxAudioUnitSampler *audioUnit);
    void showUI() {audioUnit->showUI();}

    virtual void update();
    virtual void setParameter(int idx, float value);
    virtual void setParameterScaled(int idx, float value);
    void noteEvent(NoteType type, int note, int velocity);
    
    void setNoteAutoFrameLength(int noteAutoFrameLength) {this->noteAutoFrameLength = noteAutoFrameLength;}
    void setNoteAutoOffVelocity(int noteAutoOffVelocity) {this->noteAutoOffVelocity = noteAutoOffVelocity;}
    int getNoteAutoFrameLength() {return noteAutoFrameLength;}
    int getNoteAutoOffVelocity() {return noteAutoOffVelocity;}

    vector<AudioUnitParameter*> & getParameters() {return parameters;}
    map<string, vector<AudioUnitParameter*> > & getParameterGroups() {return parameterGroups;}
    vector<string> & getParameterGroupNames() {return parameterGroupNames;}

    void setGuiPosition(int x, int y);
    void setVisible(bool visible);
    void toggleVisible() {setVisible(!visible);}
    
protected:
    
    virtual void parameterSelected(AudioUnitParameter *parameter);
    
    virtual void setupGui();
    void setupGuiParameterSelector();
    void setupGuiParameterGroup(string parameterGroupName);
    void setupParameters();
    virtual void guiEvent(ofxUIEventArgs &e);
    
    // audio unit reference
    ofxAudioUnitSampler *audioUnit;

    // parameters
    vector<string> parameterGroupNames;
    map<string, vector<AudioUnitParameter*> > parameterGroups;
    vector<AudioUnitParameter*> parameters;
    
    // note events
    map<int, int> noteEvents;
    bool noteStatus[128];
    int noteAutoFrameLength;
    int noteAutoOffVelocity;
    
    // gui
    ofxUICanvas *gui;
    ofxUIDropDownList *guiParameterGroups, *guiParameters;
    bool visible;
    bool guiToSwitchParameters;
    string guiToSwitchParametersName;

};
