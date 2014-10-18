#include "Aalto.h"


//-----------
void Aalto::setup(){
    noteManual = false;
    mantaSendNotes = true;
    sequencerManta = false;
    sequencerMode = NOTES;
    
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
        manta.addPadVelocityListener(this, &Aalto::mantaPadVelocityEvent);
        manta.addSliderListener(this, &Aalto::mantaSliderEvent);
        manta.addButtonListener(this, &Aalto::mantaButtonEvent);
        
        manta.addPadClickListener(this, &Aalto::padClickEvent);
        manta.addSliderClickListener(this, &Aalto::sliderClickEvent);
        manta.addButtonClickListener(this, &Aalto::buttonClickEvent);
    }
    else {
        manta.removePadListener(this, &Aalto::mantaPadEvent);
        manta.removePadVelocityListener(this, &Aalto::mantaPadVelocityEvent);
        manta.removeSliderListener(this, &Aalto::mantaSliderEvent);
        manta.removeButtonListener(this, &Aalto::mantaButtonEvent);

        manta.removePadClickListener(this, &Aalto::padClickEvent);
        manta.removeSliderClickListener(this, &Aalto::sliderClickEvent);
        manta.removeButtonClickListener(this, &Aalto::buttonClickEvent);
    }
}

//-----------
void Aalto::update() {
    if (guiToSwitchParameters) {
        setupGuiParameterGroup(guiToSwitchParametersName);
        guiToSwitchParameters = false;
    }
    
    manta.update();
    
    if (mappings.count(54) > 0)
        aalto.setParameter(mappings[54].parameterId, 0, ofMap(manta.getNumFingers(), 0, 10, mappings[54].rmin, mappings[54].rmax));
    if (mappings.count(55) > 0)
        aalto.setParameter(mappings[55].parameterId, 0, ofMap(manta.getPadSum(), 0, 1024, mappings[55].rmin, mappings[55].rmax));
    if (mappings.count(56) > 0)
        aalto.setParameter(mappings[56].parameterId, 0, ofMap(manta.getPadAverage(), 0, 196, mappings[56].rmin, mappings[56].rmax));
    if (mappings.count(57) > 0)
        aalto.setParameter(mappings[57].parameterId, 0, ofMap(manta.getPerimeter(), 0, 2, mappings[57].rmin, mappings[57].rmax));
    if (mappings.count(58) > 0)
        aalto.setParameter(mappings[58].parameterId, 0, ofMap(manta.getAverageInterFingerDistance(), 0, 1, mappings[58].rmin, mappings[58].rmax));
    if (mappings.count(59) > 0)
        aalto.setParameter(mappings[59].parameterId, 0, ofMap(manta.getCentroidX(), 0, 1, mappings[59].rmin, mappings[59].rmax));
    if (mappings.count(60) > 0)
        aalto.setParameter(mappings[60].parameterId, 0, ofMap(manta.getCentroidY(), 0, 1, mappings[60].rmin, mappings[60].rmax));
    if (mappings.count(61) > 0)
        aalto.setParameter(mappings[61].parameterId, 0, ofMap(manta.getWeightedCentroidX(), 0, 1, mappings[61].rmin, mappings[61].rmax));
    if (mappings.count(62) > 0)
        aalto.setParameter(mappings[62].parameterId, 0, ofMap(manta.getWeightedCentroidY(), 0, 1, mappings[62].rmin, mappings[62].rmax));
    
    
    if (sequencerManta) {
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
    if      (type == NOTE_ON) {
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
void Aalto::chooseSequencerMode(string &s) {
    if      (s=="notes") {
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
void Aalto::mantaSliderEvent(ofxMantaEvent &e) {
    if (e.value == -1)  return;
    int idx = 48 + e.id;
    if (mappings.count(idx) > 0) {
        aalto.setParameter(mappings[idx].parameterId, 0,
                           ofMap(e.value / 4096.0, 0, 1,
                                 mappings[idx].rmin,
                                 mappings[idx].rmax));
    }
    else {
        
    }
}

//-----------
void Aalto::mantaButtonEvent(ofxMantaEvent &e) {
    int idx = 50 + e.id;
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
                if (guiActiveManta < 48)
                    manta.markPad(guiActiveManta / 8, guiActiveManta % 8, true);
                else if (guiActiveManta < 50)
                    manta.markSlider(guiActiveManta - 48, true);
                else if (guiActiveManta < 54)
                    manta.markButton(guiActiveManta - 50, true);
                mappings[guiActiveManta] = parameters[i];
                setupGuiPadInspector();
                return;
            }
        }
    }
    else if (e.getName() == "remove") {
        if (guiActiveManta < 48)
            manta.markPad(guiActiveManta / 8, guiActiveManta % 8, false);
        else if (guiActiveManta < 50)
            manta.markSlider(guiActiveManta - 48, false);
        else if (guiActiveManta < 54)
            manta.markButton(guiActiveManta - 50, false);
        mappings.erase(guiActiveManta);
        setupGuiPadInspector();
    }
    else if (e.getName() == "range") {
        mappings[guiActiveManta].rmin = guiParameterRange->getValueLow();
        mappings[guiActiveManta].rmax = guiParameterRange->getValueHigh();
        guiParameterMin->setTextString(ofToString(mappings[guiActiveManta].rmin));
        guiParameterMax->setTextString(ofToString(mappings[guiActiveManta].rmax));
    }
    else if (e.getName() == "min") {
        mappings[guiActiveManta].rmin = ofToFloat(guiParameterMin->getTextString());
        guiParameterRange->setValueLow(mappings[guiActiveManta].rmin);
    }
    else if (e.getName() == "max") {
        mappings[guiActiveManta].rmax = ofToFloat(guiParameterMax->getTextString());
        guiParameterRange->setValueHigh(mappings[guiActiveManta].rmax);
    }
}

//-----------
void Aalto::guiStatsEvent(ofxUIEventArgs &e) {
    if      (e.getName() == "numFingers") {
        guiActiveManta = 54;
        setupGuiPadInspector();
    }
    else if (e.getName() == "padSum") {
        guiActiveManta = 55;
        setupGuiPadInspector();
    }
    else if (e.getName() == "padAverage") {
        guiActiveManta = 56;
        setupGuiPadInspector();
    }
    else if (e.getName() == "perimeter") {
        guiActiveManta = 57;
        setupGuiPadInspector();
    }
    else if (e.getName() == "fingerDist") {
        guiActiveManta = 58;
        setupGuiPadInspector();
    }
    else if (e.getName() == "centroidX") {
        guiActiveManta = 59;
        setupGuiPadInspector();
    }
    else if (e.getName() == "centroidY") {
        guiActiveManta = 60;
        setupGuiPadInspector();
    }
    else if (e.getName() == "wCentroidX") {
        guiActiveManta = 61;
        setupGuiPadInspector();
    }
    else if (e.getName() == "wCentroidY") {
        guiActiveManta = 62;
        setupGuiPadInspector();
    }
}

//-----------
void Aalto::setupGuiPadInspector() {
    guiP->clearWidgets();
    if (mappings.count(guiActiveManta) == 0) {
        vector<string> parameterNames;
        if (guiActiveManta < 48)
            guiP->addLabel("pad ("+ofToString(guiActiveManta/8)+", "+ofToString(guiActiveManta%8)+")");
        else if (guiActiveManta < 50)
            guiP->addLabel("slider ("+ofToString(guiActiveManta-48)+")");
        else if (guiActiveManta < 54)
            guiP->addLabel("button ("+ofToString(guiActiveManta-50)+")");
        else if (guiActiveManta == 54)  guiP->addLabel("numFingers");
        else if (guiActiveManta == 55)  guiP->addLabel("padSum");
        else if (guiActiveManta == 56)  guiP->addLabel("padAverage");
        else if (guiActiveManta == 57)  guiP->addLabel("perimeter");
        else if (guiActiveManta == 58)  guiP->addLabel("fingerDist");
        else if (guiActiveManta == 59)  guiP->addLabel("centroidX");
        else if (guiActiveManta == 60)  guiP->addLabel("centroidY");
        else if (guiActiveManta == 61)  guiP->addLabel("wCentroidX");
        else if (guiActiveManta == 62)  guiP->addLabel("wCentroidY");
        guiP->addSpacer();
        guiParameterGroups = guiP->addDropDownList("parameter groups", parameterGroupNames);
        guiParameters = guiP->addDropDownList("parameters", parameterNames);
        guiParameterGroups->setAutoClose(true);
        guiParameters->setAutoClose(true);
    }
    else {
        if (guiActiveManta < 48)
            guiP->addLabel("pad ("+ofToString(guiActiveManta/8)+", "+ofToString(guiActiveManta%8)+")");
        else if (guiActiveManta < 50)
            guiP->addLabel("slider ("+ofToString(guiActiveManta-48)+")");
        else if (guiActiveManta < 54)
            guiP->addLabel("button ("+ofToString(guiActiveManta-50)+")");
        else if (guiActiveManta == 54)  guiP->addLabel("numFingers");
        else if (guiActiveManta == 55)  guiP->addLabel("padSum");
        else if (guiActiveManta == 56)  guiP->addLabel("padAverage");
        else if (guiActiveManta == 57)  guiP->addLabel("perimeter");
        else if (guiActiveManta == 58)  guiP->addLabel("fingerDist");
        else if (guiActiveManta == 59)  guiP->addLabel("centroidX");
        else if (guiActiveManta == 60)  guiP->addLabel("centroidY");
        else if (guiActiveManta == 61)  guiP->addLabel("wCentroidX");
        else if (guiActiveManta == 62)  guiP->addLabel("wCentroidY");
        guiP->addSpacer();
        guiP->addLabel(mappings[guiActiveManta].name);
        guiParameterRange = guiP->addRangeSlider("pad range",
                                                 mappings[guiActiveManta].min,
                                                 mappings[guiActiveManta].max,
                                                 mappings[guiActiveManta].rmin,
                                                 mappings[guiActiveManta].rmax);
        guiP->addLabel("min: ");
        guiP->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
        guiParameterMin = guiP->addTextInput("min", ofToString(mappings[guiActiveManta].rmin), 80.0f);
        guiP->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
        guiP->addLabel("max: ");
        guiP->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
        guiParameterMax = guiP->addTextInput("max", ofToString(mappings[guiActiveManta].rmax), 80.0f);
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
    guiActiveManta = pad;
    setupGuiPadInspector();
}

//-----------
void Aalto::sliderClickEvent(int & slider) {
    guiActiveManta = 48 + slider;
    setupGuiPadInspector();
}

//-----------
void Aalto::buttonClickEvent(int & button) {
    guiActiveManta = 50 + button;
    setupGuiPadInspector();
}

//-----------
void Aalto::mantaPadVelocityEvent(ofxMantaEvent &e) {
    int velocity = e.value;
    if (mappings.count(e.row * 8 + e.col) > 0) {
    }
    else {
        if (mantaSendNotes) {
            int degree = (2*e.row - (int)(e.row/2) + e.col) % 7;
            int octave = floor((2*e.row - floor(e.row/2) + e.col)/7);
            int note = theory.getNote(60 + 12 * octave, degree);
            if (noteManual) {
                if (velocity == 0) {
                    noteEvent(NOTE_OFF, note, 100);
                }
                else {
                    noteEvent(NOTE_ON, note, velocity);
                }
            }
            else {
                if (velocity > 0) {
                    noteEvent(NOTE_AUTO, note, velocity);
                }
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
    manta.draw(205, 5, 240);
    manta.drawStats(450, 5, 240);
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
    
    control.addParameter("noteManual", &noteManual);
    control.addParameter("play manta", &mantaSendNotes);
    control.addParameter("sequencerManta", &sequencerManta);
    control.addMenu("sequencer", seqOptions, this, &Aalto::chooseSequencerMode);
    
    guiP = new ofxUICanvas("parameter inspector");
    guiP->setPosition(810, 5);
    guiP->setHeight(185);
    ofAddListener(guiP->newGUIEvent, this, &Aalto::guiParametersEvent);

    guiS = new ofxUICanvas("parameter inspector");
    guiS->setPosition(695, 5);
    guiS->setWidth(110);
    guiS->setHeight(185);
    guiS->clearWidgets();
    guiS->addLabelButton("numFingers", false);
    guiS->addLabelButton("padSum", false);
    guiS->addLabelButton("padAverage", false);
    guiS->addLabelButton("perimeter", false);
    guiS->addLabelButton("fingerDist", false);
    guiS->addLabelButton("centroidX", false);
    guiS->addLabelButton("centroidY", false);
    guiS->addLabelButton("wCentroidX", false);
    guiS->addLabelButton("wCentroidY", false);

    ofAddListener(guiS->newGUIEvent, this, &Aalto::guiStatsEvent);
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
