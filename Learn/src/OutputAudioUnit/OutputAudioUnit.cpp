#include "OutputAudioUnit.h"


//-----------
void OutputAudioUnit::setupAudioUnit(ofxAudioUnitSampler *audioUnit) {
    AudioUnitPlayer::setupAudioUnit(audioUnit);
    gui->removeWidgets();
    setupGui();
}

//-----------
void OutputAudioUnit::update() {
    AudioUnitPlayer::update();
    
    // update active parameters
    for (int i=0; i<activeParameters.size(); i++) {
        audioUnit->setParameter(activeParameters[i]->parameterId, 0,
                                *activeParameters[i]->value);
    }
}

//-----------
void OutputAudioUnit::parameterSelected(AudioUnitParameter *parameter) {
    activeParameters.push_back(parameter);
    learn->addOutput(parameter->name, parameter->value, parameter->min, parameter->max);
}

//-----------
void OutputAudioUnit::resetParameterMappings() {
    activeParameters.clear();
    vector<LearnOutputParameter*> *learnOutputs = learn->getOutputs();
    for (int i=0; i<learnOutputs->size(); i++) {
        for (int j=0; j<parameters.size(); j++) {
            if (learnOutputs->at(i)->getName() == parameters[j]->name) {
                learnOutputs->at(i)->setReference(parameters[j]->value);
                activeParameters.push_back(parameters[j]);
                break;
            }
        }
    }
}
