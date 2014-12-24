#pragma once

#include "ofMain.h"
#include "MantaController.h"
#include "AudioUnitPlayer.h"


class AudioUnitMantaController : public AudioUnitPlayer
{
public:
    void setup(MantaController *manta, ofxAudioUnitSampler *audioUnit);
    void setActive(bool active);

    void draw();
    
protected:

    void mantaFeaturesUpdate();
    bool selectedElementMapped();
    
    void setupGui();
    void guiEvent(ofxUIEventArgs &e);
    void parameterSelected(AudioUnitParameter *parameter);
    void mantaClickEvent(MantaElement & evt);

    void mantaPadEvent(ofxMantaEvent &e);
    void mantaPadVelocityEvent(ofxMantaEvent &e);
    void mantaSliderEvent(ofxMantaEvent &e);
    void mantaButtonEvent(ofxMantaEvent &e);
    void mantaElementDoEffects(MantaElement m, float lerpValue);
    
    MantaController *manta;
    map<MantaElement, AudioUnitParameter*> mantaEffectsMap;
    map<MantaElement, int> mantaNoteMap;
    MantaElement selectedMantaElement;
    bool active;
};