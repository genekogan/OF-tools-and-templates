#include "Aalto.h"


//-----------
void Aalto::setup(){
    noteMode = MANUAL;
    
    // setup audio
    aalto = ofxAudioUnitSampler('aumu','Aalt', 'MLbs');
    aalto.showUI();
    aalto.connectTo(mixer);
    mixer.connectTo(output);
    output.start();
    
    // setup sequencer
    sequencer.setup(6, 8);
    sequencer.addBeatListener(this, &Aalto::sequencerStepEvent);
    sequencer.setPosition(220, 200, 320, 160);

    // setup gui
    setupGui();
    setupGuiPresets();

    // setup manta
    manta.setup();
    
    // setup instrument parameters
    vector<AudioUnitParameterInfo> params = aalto.getParameterList();
    string newGroupName = "parameter group";
    for (int i=0; i<params.size(); i++) {
        ParameterMapping parameter;
        parameter.name = params[i].name;
        parameter.parameterId = i;
        parameter.min = params[i].minValue;
        parameter.max = params[i].maxValue;
        parameter.rmin = params[i].minValue;
        parameter.rmax = params[i].maxValue;
        parameters.push_back(parameter);
        int clumpId = params[i].clumpID;
        if (clumpId + 1 > parameterGroups.size()) {
            newGroupName = ofToString(params[i].name);
            vector<string> newGroup;
            parameterGroups[newGroupName] = newGroup;
            parameterGroupNames.push_back(newGroupName);
        }
        parameterGroups[newGroupName].push_back(params[i].name);
    }
    
    setActive(true);
}

//-----------
void Aalto::setActive(bool active) {
    if (active) {
        manta.addPadListener(this, &Aalto::mantaPadEvent);
        manta.addPadClickListener(this, &Aalto::padClickEvent);
        manta.addPadVelocityListener(this, &Aalto::mantaPadVelocityEvent);
    }
    else {
        manta.removePadListener(this, &Aalto::mantaPadEvent);
        manta.removePadClickListener(this, &Aalto::padClickEvent);
        manta.removePadVelocityListener(this, &Aalto::mantaPadVelocityEvent);
    }
}

//-----------
void Aalto::update() {
    if (guiToSwitchParameters) {
        setupGuiParameterGroup(guiToSwitchParametersName);
        guiToSwitchParameters = false;
    }
    
    bool mantaActive = true;
    if (mantaActive) {
        for (int r=0; r<6; r++) {
            for (int c=0; c<8; c++) {
                sequencer.setValue(r, c, manta.getPad(5 -r, c)/196.0);
            }
        }
    }
    sequencer.update();
    
    for (int i=0; i<128; i++) {
        if (noteEvents.count(i)>0) {
            if (noteEvents[i] == -1) continue;
            if (ofGetFrameNum() > noteEvents[i] + 2) {
                noteEvents[i] = -1;
                noteEvent(NOTE_OFF, i, 120);
            }
        }
    }
}

//-----------
void Aalto::noteEvent(NoteType type, int note, int velocity) {
    if (type == NOTE_ON) {
        aalto.midiNoteOn(note, velocity);
        noteStatus[note] = true;
    }
    else if (type == NOTE_OFF) {
        aalto.midiNoteOff(note, velocity);
        noteStatus[note] = false;
    }
    else if (type == NOTE_AUTO) {
        aalto.midiNoteOn(note, velocity);
        noteEvents[note] = ofGetFrameNum();
    }
}

//-----------
void Aalto::chooseNoteMode(string &s) {
    if (s=="manual") {
        noteMode = MANUAL;
    }
    else if (s=="auto") {
        noteMode = AUTO;
    }
}

//-----------
void Aalto::chooseSequencerMode(string &s) {
    if (s=="notes") {
        sequencerMode = NOTES;
    }
    else if (s=="parameters") {
        sequencerMode = PARAMETERS;
    }
}

//-----------
void Aalto::mantaPadEvent(ofxMantaEvent &e) {
    int idx = e.col + 8*e.row;
    if (mappings.count(idx) > 0) {
        aalto.setParameter(mappings[idx].parameterId, 0,
                           ofMap(e.value / 196.0, 0, 1,
                                 mappings[idx].rmin,
                                 mappings[idx].rmax));
    }
    else {
        
    }
}

//-----------
void Aalto::guiParametersEvent(ofxUIEventArgs &e) {
    if (e.getParentName() == "parameter groups") {
        if (parameterGroups.count(e.getName())>0) {
            guiToSwitchParametersName = e.getName();
            guiToSwitchParameters = true;
        }
    }
    else if (e.getParentName() == "parameters") {
        for (int i=0; i<parameters.size(); i++) {
            if (parameters[i].name == e.getName()) {
                mappings[guiActivePad] = parameters[i];
                manta.markPad(guiActivePad / 8, guiActivePad % 8, true);
                setupGuiPadInspector();
                return;
            }
        }
    }
    else if (e.getName() == "remove") {
        mappings.erase(guiActivePad);
        manta.markPad(guiActivePad / 8, guiActivePad % 8, false);
        setupGuiPadInspector();
    }
    else if (e.getName() == "range") {
        mappings[guiActivePad].rmin = guiParameterRange->getValueLow();
        mappings[guiActivePad].rmax = guiParameterRange->getValueHigh();
        guiParameterMin->setTextString(ofToString(mappings[guiActivePad].rmin));
        guiParameterMax->setTextString(ofToString(mappings[guiActivePad].rmax));
    }
    else if (e.getName() == "min") {
        mappings[guiActivePad].rmin = ofToFloat(guiParameterMin->getTextString());
        guiParameterRange->setValueLow(mappings[guiActivePad].rmin);
    }
    else if (e.getName() == "max") {
        mappings[guiActivePad].rmax = ofToFloat(guiParameterMax->getTextString());
        guiParameterRange->setValueHigh(mappings[guiActivePad].rmax);
    }
}

//-----------
void Aalto::setupGuiPadInspector() {
    guiP->clearWidgets();
    if (mappings.count(guiActivePad) == 0) {
        vector<string> parameterNames;
        guiP->addLabel("parameter ("+ofToString(guiActivePad/8)+", "+ofToString(guiActivePad%8)+")");
        guiP->addSpacer();
        guiParameterGroups = guiP->addDropDownList("parameter groups", parameterGroupNames);
        guiParameters = guiP->addDropDownList("parameters", parameterNames);
        guiParameterGroups->setAutoClose(true);
        guiParameters->setAutoClose(true);
    }
    else {
        guiP->addLabel("parameter ("+ofToString(guiActivePad/8)+", "+ofToString(guiActivePad%8)+")");
        guiP->addSpacer();
        guiP->addLabel(mappings[guiActivePad].name);
        guiParameterRange = guiP->addRangeSlider("pad range",
                                                 mappings[guiActivePad].min,
                                                 mappings[guiActivePad].max,
                                                 mappings[guiActivePad].rmin,
                                                 mappings[guiActivePad].rmax);
        guiP->addLabel("min: ");
        guiP->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
        guiParameterMin = guiP->addTextInput("min", ofToString(mappings[guiActivePad].rmin), 80.0f);
        guiP->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
        guiP->addLabel("max: ");
        guiP->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
        guiParameterMax = guiP->addTextInput("max", ofToString(mappings[guiActivePad].rmax), 80.0f);
        guiP->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
        guiP->addLabelButton("remove", false);
        guiP->addSpacer();
    }
}

//-----------
void Aalto::setupGuiParameterGroup(string parameterGroupName) {
    guiParameters->clearToggles();
    guiParameters->addToggles(parameterGroups[parameterGroupName]);
    guiParameters->open();
}

//-----------
void Aalto::padClickEvent(int & pad) {
    guiActivePad = pad;
    setupGuiPadInspector();
}

//-----------
void Aalto::mantaPadVelocityEvent(ofxMantaEvent &e) {
    int velocity = e.value;
    if (mappings.count(e.row * 8 + e.col) > 0) {
    }
    else {
        int degree = (2*e.row - (int)(e.row/2) + e.col) % 7;
        int octave = floor((2*e.row - floor(e.row/2) + e.col)/7);
        int note = theory.getNote(60 + 12 * octave, degree);
        if (noteMode == MANUAL) {
            if (velocity == 0) {
                noteEvent(NOTE_OFF, note, 100);
            }
            else {
                noteEvent(NOTE_ON, note, velocity);
            }
        }
        else if (noteMode == AUTO) {
            if (velocity > 0) {
                noteEvent(NOTE_AUTO, note, velocity);
            }
        }
    }
}

//-----------
void Aalto::sequencerStepEvent(vector<float> &column) {
    if (sequencerMode == PARAMETERS) {
        
    }
    else if (sequencerMode == NOTES) {
        int col = sequencer.getColumn();
        for (int r=0; r<column.size(); r++) {
            if (column[r] > 0.0) {
                int note = theory.getNote(60, col + 2*(r+1) + floor((r+1)/2));
                noteEvent(NOTE_AUTO, note, 127.0*column[r]);
            }
        }
    }
}

//-----------
void Aalto::draw() {
    manta.draw(220, 5, 240);
    sequencer.draw();
}

//-----------
void Aalto::setupGui() {
    control.setName("aalto");
    
    vector<string> noteOptions, padOptions, seqOptions;
    noteOptions.push_back("manual");
    noteOptions.push_back("auto");
    padOptions.push_back("notes");
    padOptions.push_back("parameters");
    seqOptions.push_back("notes");
    seqOptions.push_back("parameters");
    
    control.addMenu("noteMode", noteOptions, this, &Aalto::chooseNoteMode);
    control.addMenu("sequencer", seqOptions, this, &Aalto::chooseSequencerMode);
    
    guiP = new ofxUICanvas("parameter inspector");
    guiP->setPosition(465, 5);
    guiP->setHeight(180);
    ofAddListener(guiP->newGUIEvent, this, &Aalto::guiParametersEvent);
}

//-----------
void Aalto::setupGuiPresets() {
    vector<string> presets;
    ofDirectory dir(ofToDataPath("presetsAudio/aalto/"));
    dir.allowExt("aupreset");
    dir.listDir();
    for(int i = 0; i < dir.numFiles(); i++) {
        string filename = dir.getName(i);
        ofStringReplace(filename, ".aupreset", "");
        presets.push_back(filename);
    }
    control.addMenu("presets", presets, this, &Aalto::loadPreset);
}

//-----------
void Aalto::printParameterList() {
    vector<AudioUnitParameterInfo> params = aalto.getParameterList();
    for (int i=0; i<params.size(); i++) {
        cout << "void set"<<params[i].name<<"(float value) { aalto.setParameter("<<i<<", 0, value); }   //"<< params[i].minValue << "->"<<params[i].maxValue << endl;
    }
}

//-----------
void Aalto::savePreset(string filename) {
    if (filename=="") {
        filename = ofSystemTextBoxDialog("choose a preset name");
    }
    if (filename=="")   return;
    aalto.saveCustomPresetAtPath(ofToDataPath("presetsAudio/aalto/"+filename+".aupreset"));
    setupGuiPresets();
}

//-----------
void Aalto::loadPreset(string &filename) {
    aalto.loadCustomPresetAtPath(ofToDataPath("presetsAudio/aalto/"+filename+".aupreset"));
}
