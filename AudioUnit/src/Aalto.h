#pragma once

#include "ofMain.h"
#include "ofxAudioUnit.h"
#include "Theory.h"
#include "Sequencer.h"
#include "Control.h"
#include "MantaController.h"


class Aalto
{
public:
    
    struct ParameterMapping {
        string name;
        int parameterId;
        float min, max;
        float rmin, rmax;
    };

    enum NoteMode { MANUAL, AUTO };
    enum NoteType { NOTE_ON, NOTE_OFF, NOTE_AUTO };
    enum OutputMode { PARAMETERS, NOTES };
    
    void setup();
    void setActive(bool active);
    
    void update();
    void draw();
    
    void chooseNoteMode(string &s);
    void chooseSequencerMode(string &s);
    
    void setupGui();
    void setupGuiPresets();
    void setupGuiPadInspector();
    void setupGuiParameterGroup(string parameterGroupName);
    
    void savePreset(string filename="");
    void loadPreset(string &filename);

    void showUI() {aalto.showUI();}
    void printParameterList();

protected:
    
    void guiParametersEvent(ofxUIEventArgs &e);
    void padClickEvent(int & pad);
    void noteEvent(NoteType type, int note, int velocity=127);
    void mantaPadEvent(ofxMantaEvent &e);
    void mantaPadVelocityEvent(ofxMantaEvent &e);
    void sequencerStepEvent(vector<float> &column);

    Control control;
    OutputMode sequencerMode;
    NoteMode noteMode;

    MantaController manta;
    Sequencer sequencer;
    Theory theory;

    ofxAudioUnitSampler aalto;
    ofxAudioUnitMixer mixer;
	ofxAudioUnitOutput output;
    
    bool noteStatus[128];
    map<int, int> noteEvents;
    
    map<string, vector<string> > parameterGroups;
    vector<string> parameterGroupNames;
    vector<ParameterMapping> parameters;
    map<int, ParameterMapping> mappings;

    ofxUICanvas *guiP;
    ofxUIDropDownList *guiParameterGroups, *guiParameters;
    ofxUIRangeSlider *guiParameterRange;
    ofxUITextInput *guiParameterMin, *guiParameterMax;
    
    int guiActivePad;
    bool guiToSwitchParameters;
    string guiToSwitchParametersName;
};

