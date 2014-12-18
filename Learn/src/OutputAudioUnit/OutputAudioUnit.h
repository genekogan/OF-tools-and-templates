#pragma once

#include "ofMain.h"
#include "Control.h"
#include "Learn.h"
#include "AudioUnitPlayer.h"


class OutputAudioUnit : public AudioUnitPlayer
{
public:
    void setupAudioUnit(ofxAudioUnitSampler *audioUnit);
    void setupLearn(Learn *learn) {this->learn = learn;}
    
    void update();
    
protected:
    
    void parameterSelected(AudioUnitParameter *parameter);
    void resetParameterMappings();

    Learn *learn;
    vector<AudioUnitParameter*> activeParameters;
};
