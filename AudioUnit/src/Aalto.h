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
    void guiStatsEvent(ofxUIEventArgs &e);
    void noteEvent(NoteType type, int note, int velocity=127);
    
    void padClickEvent(int & pad);
    void sliderClickEvent(int & slider);
    void buttonClickEvent(int & button);

    void sequencerStepEvent(vector<float> &column);
    
    void mantaPadEvent(ofxMantaEvent &e);
    void mantaPadVelocityEvent(ofxMantaEvent &e);
    void mantaSliderEvent(ofxMantaEvent &e);
    void mantaButtonEvent(ofxMantaEvent &e);
    
    Control control;
    OutputMode sequencerMode;

    MantaController manta;
    Sequencer sequencer;
    Theory theory;

    ofxAudioUnitSampler aalto;
    ofxAudioUnitMixer mixer;
	ofxAudioUnitOutput output;
    
    bool noteStatus[128];
    bool noteManual;
    bool sequencerManta;
    bool mantaSendNotes;
    map<int, int> noteEvents;
    
    map<string, vector<string> > parameterGroups;
    vector<string> parameterGroupNames;
    vector<ParameterMapping> parameters;
    map<int, ParameterMapping> mappings;

    ofxUICanvas *guiP, *guiS;
    ofxUIDropDownList *guiParameterGroups, *guiParameters;
    ofxUIRangeSlider *guiParameterRange;
    ofxUITextInput *guiParameterMin, *guiParameterMax;
    
    int guiActiveManta;
    bool guiToSwitchParameters;
    string guiToSwitchParametersName;
};

