#pragma once

#include "ofMain.h"
#include "MantaController.h"
#include "AudioUnitPlayer.h"


class AudioUnitMantaController : public AudioUnitPlayer
{
public:
    
    
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
    
    Theory theory;
    
    
    
    
    AudioUnitMantaController() : AudioUnitPlayer() {
        //ofAddListener(gui->newGUIEvent, this, &AudioUnitMantaController::guiEvent);
    }
    
    ~AudioUnitMantaController() {
        //ofRemoveListener(gui->newGUIEvent, this, &AudioUnitMantaController::guiEvent);
    }
    
    void setup(MantaController *manta, ofxAudioUnitSampler *audioUnit) {
        this->manta = manta;
        setupAudioUnit(audioUnit);
        setActive(true);
    }
    
    void setupGui() {
        gui->clearWidgets();
        
        gui->addLabel("AudioUnit");
        gui->addLabelToggle("Sequencer", false);
        gui->addSpacer();
        gui->addLabel("Selection");
        gui->addLabelButton("View Selection", false);
        gui->addLabelButton("Selection -> MIDI", false);
        gui->addSpacer();
        
        
        
        if (selectedMantaElement.element == -1) {
            gui->addLabel("Inspector");
            gui->autoSizeToFitWidgets();
            return;
        }
        
        
        string label;
        if      (selectedMantaElement.type == PAD)      label="Pad "+ofToString(selectedMantaElement.element);
        else if (selectedMantaElement.type == SLIDER)   label="Slider "+ofToString(selectedMantaElement.element);
        else if (selectedMantaElement.type == BUTTON)   label="Button "+ofToString(selectedMantaElement.element);
        if      (selectedMantaElement.selection == 1)   label=" velocity";


        if (mantaEffectsMap.count(selectedMantaElement) > 0) {
            label += " => "+mantaEffectsMap[selectedMantaElement]->name;
            gui->addLabel(label);
            gui->addRangeSlider("range",
                                mantaEffectsMap[selectedMantaElement]->min,
                                mantaEffectsMap[selectedMantaElement]->max,
                                mantaEffectsMap[selectedMantaElement]->rmin,
                                mantaEffectsMap[selectedMantaElement]->rmax);
            gui->addLabelButton("remove effect", false);
        }
        else {
            label += " assign:";
            gui->addLabel(label);
            AudioUnitPlayer::setupGuiParameterSelector();
        }
    
    
        gui->autoSizeToFitWidgets();
    }
    
    void guiEvent(ofxUIEventArgs &e);
    
    void update() {
        AudioUnitPlayer::update();
    }
    
    void parameterSelected(AudioUnitParameter *parameter);
   // ofxUICanvas *gui;
    

    void mantaClickEvent(MantaElement & evt) {
        selectedMantaElement = evt;
        setupGui();
    }
    
    MantaElement selectedMantaElement;
    
    
    void setActive(bool active);
    void chooseSequencerMode(string &s);
    void toggleSmooth(string &s);

private:
    
//    void sequencerStepEvent(vector<float> &column);
//    void sequencerInterpolatedStepEvent(vector<float> &column);
//    void processColumn(vector<float> &column);

    void guiParametersEvent(ofxUIEventArgs &e);
//    void guiStatsEvent(ofxUIEventArgs &e);

    void mantaPadEvent(ofxMantaEvent &e);
    void mantaPadVelocityEvent(ofxMantaEvent &e);
    void mantaSliderEvent(ofxMantaEvent &e);
    void mantaButtonEvent(ofxMantaEvent &e);
    
    MantaController *manta;
    
    map<MantaElement, AudioUnitParameter*> mantaEffectsMap;
    map<MantaElement, int> mantaNoteMap;
    
    
    bool active;
};