#include "AudioUnitPlayer.h"


//-----------
AudioUnitPlayer::AudioUnitPlayer() {
    noteAutoFrameLength = 10;
    noteAutoOffVelocity = 100;
    gui = new ofxUICanvas("AudioUnit Selector");
    ofAddListener(gui->newGUIEvent, this, &AudioUnitPlayer::guiEvent);
}

//-----------
void AudioUnitPlayer::setupAudioUnit(ofxAudioUnitSampler *audioUnit) {
    this->audioUnit = audioUnit;
    setupParameters();
    setupGui();
}

//-----------
void AudioUnitPlayer::setupParameters() {
    parameters.clear();
    parameterGroupNames.clear();
    parameterGroups.clear();
    
    // setup audiounit parameters
    vector<AudioUnitParameterInfo> params = audioUnit->getParameterList();
    string newGroupName = "parameter group";
    for (int i=0; i<params.size(); i++) {
        AudioUnitParameter *parameter = new AudioUnitParameter();
        parameter->parameterId = i;
        parameter->name = params[i].name;
        parameter->min = params[i].minValue;
        parameter->max = params[i].maxValue;
        parameter->rmin = parameter->min;
        parameter->rmax = parameter->max;
        parameter->value = new float();
        parameters.push_back(parameter);
        
        int clumpId = params[i].clumpID;
        if (clumpId + 1 > parameterGroups.size()) {
            newGroupName = ofToString(params[i].name);
            vector<AudioUnitParameter*> newGroup;
            parameterGroups[newGroupName] = newGroup;
            parameterGroupNames.push_back(newGroupName);
        }
        parameterGroups[newGroupName].push_back(parameter);
    }
}

//-----------
void AudioUnitPlayer::setParameter(int idx, float value) {
    *parameters[idx]->value = value;
    audioUnit->setParameter(parameters[idx]->parameterId, 0,
                            *parameters[idx]->value);
}

//-----------
void AudioUnitPlayer::setParameterScaled(int idx, float value) {
    *parameters[idx]->value = ofLerp(parameters[idx]->min, parameters[idx]->max, value);
    audioUnit->setParameter(parameters[idx]->parameterId, 0,
                            *parameters[idx]->value);
}

//-----------
void AudioUnitPlayer::update() {
    if (guiToSwitchParameters) {
        setupGuiParameterGroup(guiToSwitchParametersName);
        guiToSwitchParameters = false;
    }

    // manage note events
    for (int i=0; i<128; i++) {
        if (noteEvents.count(i) > 0) {
            if (noteEvents[i] == -1) continue;
            if (ofGetFrameNum() > noteEvents[i] + noteAutoFrameLength) {
                noteEvents[i] = -1;
                noteEvent(NOTE_OFF, i, noteAutoOffVelocity);
            }
        }
    }
}

//-----------
void AudioUnitPlayer::draw() {
    if (sequencer.getActive()) {
        sequencer.draw();
    }
}

//-----------
void AudioUnitPlayer::noteEvent(NoteType type, int note, int velocity) {
    if      (type == NOTE_ON) {
        audioUnit->midiNoteOn(note, velocity);
        noteStatus[note] = true;
    }
    else if (type == NOTE_OFF) {
        audioUnit->midiNoteOff(note, velocity);
        noteStatus[note] = false;
    }
    else if (type == NOTE_AUTO) {
        audioUnit->midiNoteOn(note, velocity);
        noteEvents[note] = ofGetFrameNum();
    }
}
//-----------
void AudioUnitPlayer::setupGui() {
    gui->addLabelButton("Show AudioUnit", false);
    gui->addSpacer();
    gui->addLabel("add output");
    setupGuiParameterSelector();
}

//-----------
void AudioUnitPlayer::setupGuiParameterSelector() {
    vector<string> parameterNames;
    guiParameterGroups = gui->addDropDownList("parameter groups", parameterGroupNames);
    guiParameters = gui->addDropDownList("parameters", parameterNames);
    guiParameterGroups->setAutoClose(true);
    guiParameters->setAutoClose(true);
}

//-----------
void AudioUnitPlayer::setupGuiParameterGroup(string parameterGroupName) {
    vector<string> parameterGroupItems;
    for (int i=0; i<parameterGroups[parameterGroupName].size(); i++) {
        parameterGroupItems.push_back(parameterGroups[parameterGroupName][i]->name);
    }
    guiParameters->clearToggles();
    guiParameters->addToggles(parameterGroupItems);
    guiParameters->open();
}

//-----------
void AudioUnitPlayer::setGuiPosition(int x, int y) {
    gui->setPosition(x, y);
}

//-----------
void AudioUnitPlayer::parameterSelected(AudioUnitParameter *parameter) {
    ofLog(OF_LOG_NOTICE, "Selected parameter: "+parameter->name);
}

//-----------
void AudioUnitPlayer::guiEvent(ofxUIEventArgs &e) {
    if (e.getParentName() == "parameter groups") {
        guiToSwitchParametersName = e.getName();
        guiToSwitchParameters = true;
    }
    else if (e.getParentName() == "parameters") {
        for (int i=0; i<parameters.size(); i++) {
            if (parameters[i]->name == e.getName()) {
                parameterSelected(parameters[i]);
                return;
            }
        }
    }
    else if (e.getName() == "Show AudioUnit") {
        if (e.getButton()->getValue())  return;
        showUI();
    }
}


//-----------
void AudioUnitPlayer::setupSequencer() {
    if (sequencer.getActive())  return;
    sequencer.setup(6, 8);
    ofAddListener(sequencer.getSequencer().sequencerEvent, this, &AudioUnitPlayer::sequencerStepEvent);
    ofAddListener(sequencer.interpolatedSequencerEvent, this, &AudioUnitPlayer::sequencerInterpolatedStepEvent);
    sequencer.setSmooth(sequencerSmooth);
    sequencer.setPosition(165, 225, 260, 120);
}

//-----------
void AudioUnitPlayer::toggleSequencerSmooth(string &s) {
    this->sequencerSmooth = !sequencerSmooth;
    sequencer.setSmooth(sequencerSmooth);
}

//-----------
void AudioUnitPlayer::sequencerStepEvent(vector<float> &column) {
    if (!sequencer.getSmooth()) {
        processColumn(column);
    }
}

//-----------
void AudioUnitPlayer::sequencerInterpolatedStepEvent(vector<float> &column) {
    if (sequencer.getSmooth()) {
        processColumn(column);
    }
}

//-----------
void AudioUnitPlayer::processColumn(vector<float> &column) {
    /*
    if (sequencerMode == NOTES) {
        int col = sequencer.getColumn();
        for (int r=0; r<column.size(); r++) {
            if (column[r] > 0.0) {
                int note = theory.getNote(60, col + 2*(r+1) + floor((r+1)/2));
                noteEvent(NOTE_AUTO, note, 127.0*column[r]);
            }
        }
    }
    else if (sequencerMode == PARAMETERS) {
        int col = sequencer.getColumn();
        for (int r=0; r<column.size(); r++) {
            if (seqMap.count(r)) {
                au.setParameter(seqMap[r].parameterId, 0,
                                ofMap(column[r], 0, 1,
                                      seqMap[r].rmin, seqMap[r].rmax));
            }
        }
    }*/
}

//-----------
void AudioUnitPlayer::setVisible(bool visible) {
    this->visible = visible;
    gui->setVisible(visible);
}

//-----------
AudioUnitPlayer::~AudioUnitPlayer() {
    ofRemoveListener(gui->newGUIEvent, this, &AudioUnitPlayer::guiEvent);
    delete guiParameterGroups;
    delete guiParameters;
    delete gui;
}
