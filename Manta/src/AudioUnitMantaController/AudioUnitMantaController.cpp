#include "AudioUnitMantaController.h"


//-----------
void AudioUnitMantaController::setup(MantaController *manta, ofxAudioUnitSampler *audioUnit) {
    this->manta = manta;
    manta->setSelectionView(true);
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
    mantaElementDoEffects(MantaElement(PAD, e.col + 8 * e.row, 0),
                          (float) e.value / 196.0);
    mantaFeaturesUpdate();
}

//-----------
void AudioUnitMantaController::mantaSliderEvent(ofxMantaEvent &e) {
    mantaElementDoEffects(MantaElement(SLIDER, e.col + 8 * e.row, 0),
                          (float) e.value / 4096.0);
    mantaFeaturesUpdate();
}

//-----------
void AudioUnitMantaController::mantaButtonEvent(ofxMantaEvent &e) {
    mantaElementDoEffects(MantaElement(BUTTON, e.col + 8 * e.row, 0),
                          (float) e.value / 1.0);
    mantaFeaturesUpdate();
}

//-----------
void AudioUnitMantaController::mantaFeaturesUpdate() {
    return;
    mantaElementDoEffects(MantaElement(NUM_PADS, 0, 0), manta->getNumPads() / 48.0f);
    mantaElementDoEffects(MantaElement(PAD_SUM, 0, 0), manta->getPadSum() / 3000.0f);
    mantaElementDoEffects(MantaElement(PAD_AVG, 0, 0), manta->getPadAverage() / 196.0f);
    mantaElementDoEffects(MantaElement(CENTROID_X, 0, 0), manta->getCentroidX());
    mantaElementDoEffects(MantaElement(CENTROID_Y, 0, 0), manta->getCentroidY());
    mantaElementDoEffects(MantaElement(W_CENTROID_X, 0, 0), manta->getWeightedCentroidX());
    mantaElementDoEffects(MantaElement(W_CENTROID_Y, 0, 0), manta->getWeightedCentroidY());
    mantaElementDoEffects(MantaElement(AVG_FING_DIST, 0, 0), manta->getAverageInterFingerDistance() / 2.0f);
    mantaElementDoEffects(MantaElement(PERIMETER, 0, 0), manta->getPerimeter() / 3.0f);
    mantaElementDoEffects(MantaElement(WIDTH, 0, 0), manta->getWidth());
    mantaElementDoEffects(MantaElement(HEIGHT, 0, 0), manta->getHeight());
    mantaElementDoEffects(MantaElement(WH_RATIO, 0, 0), manta->getWidthHeightRatio() / 10.0f);
}

//-----------
void AudioUnitMantaController::mantaElementDoEffects(MantaElement m, float lerpValue) {
    if (mantaEffectsMap.count(m)>0) {
        audioUnit->setParameter(mantaEffectsMap[m]->parameterId, 0,
                                ofLerp(mantaEffectsMap[m]->rmin, mantaEffectsMap[m]->rmax, lerpValue));
    }
}

//-----------
void AudioUnitMantaController::parameterSelected(AudioUnitParameter *parameter) {
    if (selectedMantaElement.selection == -1) return;
    mantaEffectsMap[selectedMantaElement] = parameter;
    if (selectedMantaElement.type == PAD) {
        int row = floor(selectedMantaElement.element / 8);
        int col = selectedMantaElement.element % 8;
        manta->markPad(row, col, true);
        manta->addPadToSelection(row, col, 2);
    }
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
    
    gui->addLabel("MIDI Selection");
    gui->addLabelButton("View Selection", false);
    gui->addLabelButton("Selection -> MIDI", false);
    gui->addSpacer();

    if (selectedMantaElement.element == -1 || manta->getSizeSelection()>1) {
        gui->autoSizeToFitWidgets();
        return;
    }
    
    string inputLabel = "I: ";
    if      (selectedMantaElement.type == PAD)      inputLabel+="Pad "+ofToString(selectedMantaElement.element);
    else if (selectedMantaElement.type == SLIDER)   inputLabel+="Slider "+ofToString(selectedMantaElement.element);
    else if (selectedMantaElement.type == BUTTON)   inputLabel+="Button "+ofToString(selectedMantaElement.element);
    else if (selectedMantaElement.type == NUM_PADS)      inputLabel+="Num Pads";
    else if (selectedMantaElement.type == PAD_SUM)      inputLabel+="Pad Sum";
    else if (selectedMantaElement.type == PAD_AVG)      inputLabel+="Pad Avg";
    else if (selectedMantaElement.type == CENTROID_X)      inputLabel+="CentroidX";
    else if (selectedMantaElement.type == CENTROID_Y)      inputLabel+="CentroidY";
    else if (selectedMantaElement.type == W_CENTROID_X)      inputLabel+="WCentroidX";
    else if (selectedMantaElement.type == W_CENTROID_Y)      inputLabel+="WCentroidY";
    else if (selectedMantaElement.type == AVG_FING_DIST)      inputLabel+="Avg Fing Dist";
    else if (selectedMantaElement.type == PERIMETER)      inputLabel+="Perimeter";
    else if (selectedMantaElement.type == WIDTH)      inputLabel+="Width";
    else if (selectedMantaElement.type == HEIGHT)      inputLabel+="Height";
    else if (selectedMantaElement.type == WH_RATIO)      inputLabel+="W/H Ratio";
    
    if      (selectedMantaElement.selection == 1)   inputLabel+=" vel";
    
    if (selectedElementMapped()) {
        string outputLabel = "O: "+mantaEffectsMap[selectedMantaElement]->name;
        gui->addLabel(inputLabel);
        gui->addLabel(outputLabel);
        gui->addRangeSlider("range",
                            mantaEffectsMap[selectedMantaElement]->min,
                            mantaEffectsMap[selectedMantaElement]->max,
                            mantaEffectsMap[selectedMantaElement]->rmin,
                            mantaEffectsMap[selectedMantaElement]->rmax);
        gui->addLabelButton("remove effect", false);
    }
    else {
        inputLabel += " assign:";
        gui->addLabel(inputLabel);
        AudioUnitPlayer::setupGuiParameterSelector();
    }
    
    gui->autoSizeToFitWidgets();
}

//-----------
bool AudioUnitMantaController::selectedElementMapped() {
    // hack... for some reason, this doesn't work right:
    // return mantaEffectsMap.count(selectedMantaElement) > 0;
    // so this checks manually
    map<MantaElement, AudioUnitParameter*>::iterator it = mantaEffectsMap.begin();
    while (it != mantaEffectsMap.end()) {
        if (it->first.type      == selectedMantaElement.type      &&
            it->first.selection == selectedMantaElement.selection &&
            it->first.element   == selectedMantaElement.element) {
            return true;
        }
        ++it;
    }
    return false;
}

//-----------
void AudioUnitMantaController::guiEvent(ofxUIEventArgs &e) {
    if (e.getName() == "remove effect") {
        int row = floor(selectedMantaElement.element / 8);
        int col = selectedMantaElement.element % 8;
        mantaEffectsMap.erase(selectedMantaElement);
        manta->markPad(row, col, false);
        manta->removePadFromSelection(row, col, 2);
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
        vector<int> selection[2];
        for (map<MantaElement, int>::iterator it=mantaNoteMap.begin(); it!=mantaNoteMap.end(); ++it){
            if (it->first.type != PAD)  continue;
            selection[it->first.selection].push_back(it->first.element);
        }
        manta->setPadSelection(selection[0], 0);
        manta->setPadSelection(selection[1], 1);
        setupGui();
        return;
    }
    else if (e.getName() == "Selection -> MIDI") {
        manta->markAllPads(false);
        for (int r=0; r<6; r++) {
            for (int c=0; c<8; c++) {
                manta->removePadFromSelection(r, c, 3);
            }
        }
        mantaNoteMap.clear();
        vector<int> selectionValues = manta->getPadSelection();
        vector<int> selectionVelocities = manta->getPadVelocitySelection();
        for (int i=0; i<selectionValues.size(); i++) {
            MantaElement m(PAD, selectionValues[i], 0);
            int row = floor(selectionValues[i] / 8);
            int col = selectionValues[i] % 8;
            int degree = (2 * row - (int)(row / 2) + col) % 7;
            int octave = floor((2 * row - floor(row / 2) + col) / 7);
            mantaNoteMap[m] = theory.getNote(60, degree, octave);
            manta->markPad(row, col, true);
            manta->addPadToSelection(row, col, 3);
        }
        for (int i=0; i<selectionVelocities.size(); i++) {
            MantaElement m(PAD, selectionVelocities[i], 1);
            int row = floor(selectionVelocities[i] / 8);
            int col = selectionVelocities[i] % 8;
            int degree = (2 * row - (int)(row / 2) + col) % 7;
            int octave = floor((2 * row - floor(row / 2) + col) / 7);
            mantaNoteMap[m] = theory.getNote(60, degree, octave);
            manta->markPad(row, col, true);
            manta->addPadToSelection(row, col, 4);
        }
        return;
    }
    else if (e.getName() == "Sequencer") {
        setupSequencer();
    }

    AudioUnitPlayer::guiEvent(e);
}


