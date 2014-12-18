#pragma once

#include "ofMain.h"
#include "ofxAudioUnit.h"
#include "Control.h"


// theory
class Theory {
public:
    vector<int> major, minor;
    
    Theory() {
        major.push_back(0); major.push_back(2); major.push_back(4); major.push_back(5); major.push_back(7); major.push_back(9); major.push_back(11);
        minor.push_back(0); minor.push_back(2); minor.push_back(3); minor.push_back(5); minor.push_back(7); minor.push_back(8); minor.push_back(10);
    }
    void getChord(int chord[], int root, int octave=0) {
        for (int i=0; i<3; i++) {
            chord[i] = octave * 12 + major[(root + 2*i) % 7];
        }
    }
    int getNote(int root, int degree, int octave=0) {
        return octave * 12 + root + major[degree % 7];
    }
    int getNoteAtScaleDegree(int root, int degree, int idx, int octave=0) {
        return octave * 12 + root + major[(degree+idx) % 7];
    }
};





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
    virtual void draw();
    virtual void setParameter(int idx, float value);
    virtual void setParameterScaled(int idx, float value);
    void noteEvent(NoteType type, int note, int velocity);
    
    // midi events
    void setNoteAutoFrameLength(int noteAutoFrameLength) {this->noteAutoFrameLength = noteAutoFrameLength;}
    void setNoteAutoOffVelocity(int noteAutoOffVelocity) {this->noteAutoOffVelocity = noteAutoOffVelocity;}
    int getNoteAutoFrameLength() {return noteAutoFrameLength;}
    int getNoteAutoOffVelocity() {return noteAutoOffVelocity;}

    // sequencer
    void toggleSequencerSmooth(string &s);
    
    // parameters
    vector<AudioUnitParameter*> & getParameters() {return parameters;}
    map<string, vector<AudioUnitParameter*> > & getParameterGroups() {return parameterGroups;}
    vector<string> & getParameterGroupNames() {return parameterGroupNames;}

    // gui
    void setGuiPosition(int x, int y);
    void setVisible(bool visible);
    void toggleVisible() {setVisible(!visible);}
    
protected:
    
    // gui
    virtual void parameterSelected(AudioUnitParameter *parameter);
    virtual void setupGui();
    void setupGuiParameterSelector();
    void setupGuiParameterGroup(string parameterGroupName);
    void setupParameters();
    virtual void guiEvent(ofxUIEventArgs &e);
    
    // sequencer events
    void setupSequencer();
    void sequencerStepEvent(vector<float> &column);
    void sequencerInterpolatedStepEvent(vector<float> &column);
    void processColumn(vector<float> &column);
    
    // audio unit reference
    ofxAudioUnitSampler *audioUnit;
    
    // theory
    Theory theory;
    
    // sequencer
    Sequencer sequencer;
    bool sequencerSmooth;
    
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
