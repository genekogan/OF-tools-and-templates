#include "AudioUnitMantaController.h"


void AudioUnitMantaController::setActive(bool active) {
    this->active = active;
    if (active) {
        manta->addPadListener(this, &AudioUnitMantaController::mantaPadEvent);
        manta->addPadVelocityListener(this, &AudioUnitMantaController::mantaPadVelocityEvent);
        manta->addSliderListener(this, &AudioUnitMantaController::mantaSliderEvent);
        manta->addButtonListener(this, &AudioUnitMantaController::mantaButtonEvent);
        manta->addClickListener(this, &AudioUnitMantaController::mantaClickEvent);
    }
    else {
        manta->removePadListener(this, &AudioUnitMantaController::mantaPadEvent);
        manta->removePadVelocityListener(this, &AudioUnitMantaController::mantaPadVelocityEvent);
        manta->removeSliderListener(this, &AudioUnitMantaController::mantaSliderEvent);
        manta->removeButtonListener(this, &AudioUnitMantaController::mantaButtonEvent);
        manta->removeClickListener(this, &AudioUnitMantaController::mantaClickEvent);
    }
}



//-----------
void AudioUnitMantaController::chooseSequencerMode(string &s) {
//    sequencerMode = (s == "notes") ? NOTES : PARAMETERS;
}

//-----------
void AudioUnitMantaController::toggleSmooth(string &s) {
//    this->sequencerSmooth = !sequencerSmooth;
//    sequencer.setSmooth(sequencerSmooth);
}



//-----------
void AudioUnitMantaController::mantaPadEvent(ofxMantaEvent &e) {
    MantaElement m(PAD, e.col + 8 * e.row, 0);
    if (mantaEffectsMap.count(m)>0) {
        audioUnit->setParameter(mantaEffectsMap[m]->parameterId, 0,
                                ofMap(e.value, 0, 196.0, mantaEffectsMap[m]->rmin, mantaEffectsMap[m]->rmax));
    }
    

}

//-----------
void AudioUnitMantaController::mantaPadVelocityEvent(ofxMantaEvent &e) {
    if (e.value > 0) {
        MantaElement m(PAD, e.col + 8 * e.row, 0);
        if (mantaNoteMap.count(m) > 0) {
            int note = mantaNoteMap[m];
            int velocity = e.value;
            noteEvent(NOTE_AUTO, note, velocity);
        }
    }
    
    
    
    /*
    int idx = e.col + 8 * e.row;
    if (mantaMap.count(idx) > 0) {
        audioUnit->setParameter(mantaMap[idx].parameterId, 0, ofMap(e.value, 0, 196.0, mantaMap[idx].min, mantaMap[idx].max));
    }*/
}

//-----------
void AudioUnitMantaController::mantaSliderEvent(ofxMantaEvent &e) {
    /*
    if (e.value == -1)  return;
    int idx = 48 + e.id;
    if (mantaMap.count(idx) > 0) {
        audioUnit->setParameter(mantaMap[idx].parameterId, 0, ofMap(e.value, 0, 4096, mantaMap[idx].min, mantaMap[idx].max));
    }*/
}

//-----------
void AudioUnitMantaController::mantaButtonEvent(ofxMantaEvent &e) {
    /*
    int idx = 50 + e.id;
    if (mantaMap.count(idx) > 0) {
        audioUnit->setParameter(mantaMap[idx].parameterId, 0, ofMap(e.value, 0, 196.0, mantaMap[idx].min, mantaMap[idx].max));
    }*/
}

//-----------
void AudioUnitMantaController::parameterSelected(AudioUnitParameter *parameter) {
    if (selectedMantaElement.selection == -1) return;
    mantaEffectsMap[selectedMantaElement] = parameter;
    setupGui();
}

//-----------
void AudioUnitMantaController::guiEvent(ofxUIEventArgs &e) {
    if (e.getName() == "remove effect") {
        mantaEffectsMap.erase(selectedMantaElement);
        setupGui();
        return;
    }
    else if (e.getName() == "range") {
        float _rmin = ((ofxUIRangeSlider *) gui->getWidget("range"))->getValueLow();
        float _rmax = ((ofxUIRangeSlider *) gui->getWidget("range"))->getValueHigh();
        mantaEffectsMap[selectedMantaElement]->rmin = _rmin;
        mantaEffectsMap[selectedMantaElement]->rmax = _rmax;
        return;
    }
    else if (e.getName() == "View Selection") {
        vector<int> selection;
        for (map<MantaElement, int>::iterator it=mantaNoteMap.begin(); it!=mantaNoteMap.end(); ++it){
            if (it->first.type != PAD)  continue;
            selection.push_back(it->first.element);
        }
        manta->setPadSelection(selection, 0);
        return;
    }
    else if (e.getName() == "Selection -> MIDI") {
        mantaNoteMap.clear();
        vector<int> selection = manta->getPadSelection();
        for (int i=0; i<selection.size(); i++) {
            MantaElement m(PAD, selection[i], 0);
            int row = floor(selection[i] / 8);
            int col = selection[i] % 8;
            int degree = (2 * row - (int)(row / 2) + col) % 7;
            int octave = floor((2 * row - floor(row / 2) + col) / 7);
            mantaNoteMap[m] = theory.getNote(60, degree, octave);
        }
        return;
    }
    
    AudioUnitPlayer::guiEvent(e);
    
}

//-----------
void AudioUnitMantaController::guiParametersEvent(ofxUIEventArgs &e) {
    /*
    if (e.getParentName() == "parameter groups") {
        if (parameterGroups.count(e.getName())>0) {
            guiToSwitchParametersName = e.getName();
            guiToSwitchParameters = true;
        }
    }
    else if (e.getParentName() == "parameters") {
        for (int i=0; i<parameters.size(); i++) {
            if (guiActiveIsSequencer) {
                if (parameters[i].name == e.getName()) {
                    seqMap[guiActiveSeq] = parameters[i];
                    //setupGuiPadInspector();
                    return;
                }
            }
            else {
                if (parameters[i].name == e.getName()) {
                    if (guiActiveManta < 48)
                        manta->markPad(guiActiveManta / 8, guiActiveManta % 8, true);
                    else if (guiActiveManta < 50)
                        manta->markSlider(guiActiveManta - 48, true);
                    else if (guiActiveManta < 54)
                        manta->markButton(guiActiveManta - 50, true);
                    mantaMap[guiActiveManta] = parameters[i];
                    //setupGuiPadInspector();
                    return;
                }
            }
        }
    }
    else if (e.getName() == "remove") {
        if (guiActiveIsSequencer) {
            seqMap.erase(guiActiveSeq);
        }
        else {
            if (guiActiveManta < 48)
                manta->markPad(guiActiveManta / 8, guiActiveManta % 8, false);
            else if (guiActiveManta < 50)
                manta->markSlider(guiActiveManta - 48, false);
            else if (guiActiveManta < 54)
                manta->markButton(guiActiveManta - 50, false);
            mantaMap.erase(guiActiveManta);
        }
        //setupGuiPadInspector();
    }
    else if (e.getName() == "range") {
        if (guiActiveIsSequencer) {
            seqMap[guiActiveSeq].rmin = guiParameterRange->getValueLow();
            seqMap[guiActiveSeq].rmax = guiParameterRange->getValueHigh();
            guiParameterMin->setTextString(ofToString(seqMap[guiActiveSeq].rmin));
            guiParameterMax->setTextString(ofToString(seqMap[guiActiveSeq].rmax));
        }
        else {
            mantaMap[guiActiveManta].rmin = guiParameterRange->getValueLow();
            mantaMap[guiActiveManta].rmax = guiParameterRange->getValueHigh();
            guiParameterMin->setTextString(ofToString(mantaMap[guiActiveManta].rmin));
            guiParameterMax->setTextString(ofToString(mantaMap[guiActiveManta].rmax));
        }
    }
    else if (e.getName() == "min") {
        if (guiActiveIsSequencer) {
            seqMap[guiActiveSeq].rmin = ofToFloat(guiParameterMin->getTextString());
            guiParameterRange->setValueLow(seqMap[guiActiveSeq].rmin);
        }
        else {
            mantaMap[guiActiveManta].rmin = ofToFloat(guiParameterMin->getTextString());
            guiParameterRange->setValueLow(mantaMap[guiActiveManta].rmin);
        }
    }
    else if (e.getName() == "max") {
        if (guiActiveIsSequencer) {
            seqMap[guiActiveSeq].rmax = ofToFloat(guiParameterMax->getTextString());
            guiParameterRange->setValueHigh(seqMap[guiActiveSeq].rmax);
        }
        else {
            mantaMap[guiActiveManta].rmax = ofToFloat(guiParameterMax->getTextString());
            guiParameterRange->setValueHigh(mantaMap[guiActiveManta].rmax);
        }
    }
     */
}

//-----------