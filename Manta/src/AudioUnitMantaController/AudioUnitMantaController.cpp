#include "AudioUnitMantaController.h"


//-----------
void AudioUnitMantaController::setup(MantaController *manta, ofxAudioUnitSampler *audioUnit) {
    this->manta = manta;
    setupAudioUnit(audioUnit);
    setActive(true);
}

//-----------
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
void AudioUnitMantaController::draw() {
    AudioUnitPlayer::draw();
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
}

//-----------
void AudioUnitMantaController::mantaPadEvent(ofxMantaEvent &e) {
    mantaElementDoEffects(MantaElement(PAD, e.col + 8 * e.row, 0), e.value);
}

//-----------
void AudioUnitMantaController::mantaSliderEvent(ofxMantaEvent &e) {
    mantaElementDoEffects(MantaElement(SLIDER, e.col + 8 * e.row, 0), e.value);
}

//-----------
void AudioUnitMantaController::mantaButtonEvent(ofxMantaEvent &e) {
    mantaElementDoEffects(MantaElement(BUTTON, e.col + 8 * e.row, 0), e.value);
}

//-----------
void AudioUnitMantaController::mantaElementDoEffects(MantaElement m, float value) {
    if (mantaEffectsMap.count(m)>0) {
        audioUnit->setParameter(mantaEffectsMap[m]->parameterId, 0,
                                ofMap(value, 0, 196.0, mantaEffectsMap[m]->rmin, mantaEffectsMap[m]->rmax));
    }
}

//-----------
void AudioUnitMantaController::parameterSelected(AudioUnitParameter *parameter) {
    if (selectedMantaElement.selection == -1) return;
    mantaEffectsMap[selectedMantaElement] = parameter;
    setupGui();
}

//-----------
void AudioUnitMantaController::mantaClickEvent(MantaElement & evt) {
    selectedMantaElement = evt;
    setupGui();
}

//-----------
void AudioUnitMantaController::setupGui() {
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
    else if (e.getName() == "Sequencer") {
        setupSequencer();
    }

    AudioUnitPlayer::guiEvent(e);
}


