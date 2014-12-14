#include "AudioUnitMantaController.h"


void AudioUnitMantaController::setActive(bool active) {
    this->active = active;
    if (active) {
        manta->addPadListener(this, &AudioUnitMantaController::mantaPadEvent);
        manta->addPadVelocityListener(this, &AudioUnitMantaController::mantaPadVelocityEvent);
        manta->addSliderListener(this, &AudioUnitMantaController::mantaSliderEvent);
        manta->addButtonListener(this, &AudioUnitMantaController::mantaButtonEvent);
    }
    else {
        manta->removePadListener(this, &AudioUnitMantaController::mantaPadEvent);
        manta->removePadVelocityListener(this, &AudioUnitMantaController::mantaPadVelocityEvent);
        manta->removeSliderListener(this, &AudioUnitMantaController::mantaSliderEvent);
        manta->removeButtonListener(this, &AudioUnitMantaController::mantaButtonEvent);
    }

}


//-----------
void AudioUnitMantaController::noteEvent(NoteType type, int note, int velocity) {
    if      (type == NOTE_ON) {
        au->midiNoteOn(note, velocity);
        noteStatus[note] = true;
    }
    else if (type == NOTE_OFF) {
        au->midiNoteOff(note, velocity);
        noteStatus[note] = false;
    }
    else if (type == NOTE_AUTO) {
        au->midiNoteOn(note, velocity);
        noteEvents[note] = ofGetFrameNum();
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
    int idx = e.col + 8 * e.row;
    if (mantaMap.count(idx) > 0) {
        au->setParameter(mantaMap[idx].parameterId, 0, ofMap(e.value, 0, 196.0, mantaMap[idx].rmin, mantaMap[idx].rmax));
    }
}

//-----------
void AudioUnitMantaController::mantaPadVelocityEvent(ofxMantaEvent &e) {
    int idx = e.col + 8 * e.row;
    if (mantaMap.count(idx) > 0) {
        au->setParameter(mantaMap[idx].parameterId, 0, ofMap(e.value, 0, 196.0, mantaMap[idx].rmin, mantaMap[idx].rmax));
    }
}

//-----------
void AudioUnitMantaController::mantaSliderEvent(ofxMantaEvent &e) {
    if (e.value == -1)  return;
    int idx = 48 + e.id;
    if (mantaMap.count(idx) > 0) {
        au->setParameter(mantaMap[idx].parameterId, 0, ofMap(e.value, 0, 4096, mantaMap[idx].rmin, mantaMap[idx].rmax));
    }
}

//-----------
void AudioUnitMantaController::mantaButtonEvent(ofxMantaEvent &e) {
    int idx = 50 + e.id;
    if (mantaMap.count(idx) > 0) {
        au->setParameter(mantaMap[idx].parameterId, 0, ofMap(e.value, 0, 196.0, mantaMap[idx].rmin, mantaMap[idx].rmax));
    }
}

//-----------
void AudioUnitMantaController::guiParametersEvent(ofxUIEventArgs &e) {
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
}

//-----------