#include "Instrument.h"


//-----------
Instrument::Instrument() {
    noteAutoFrameLength = 30;
    noteVelocity = 120;
    noteManual = false;
    mantaSendNotes = true;
    sequencerManta = false;
    sequencerSmooth = true;
    sequencerMode = NOTES;
}

//-----------
void Instrument::connectTo(ofxAudioUnitMixer &mixer, int channel) {
    au.connectTo(mixer, channel);
}

//-----------
void Instrument::setup(InstrumentType type){
    if (type == AALTO) {
        au = ofxAudioUnitSampler('aumu','Aalt','MLbs');
    }
    else if (type == KAIVO) {
        au = ofxAudioUnitSampler('aumu','Kaiv','MLbs');
    }
    au.showUI();

    // setup manta
    manta.setup();

    // setup sequencer
    sequencer.setup(6, 8);
    ofAddListener(sequencer.getSequencer().sequencerEvent, this, &Instrument::sequencerStepEvent);
    ofAddListener(sequencer.interpolatedSequencerEvent, this, &Instrument::sequencerInterpolatedStepEvent);
    sequencer.setSmooth(sequencerSmooth);
    sequencer.setPosition(165, 225, 260, 120);
    
    // setup gui
    setupGui();
    setupGuiPresets();
    setActive(true);
    setVisible(true);

    // setup instrument parameters
    vector<AudioUnitParameterInfo> params = au.getParameterList();
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
}

//-----------
void Instrument::setActive(bool active) {
    if (active) {
        manta.addPadListener(this, &Instrument::mantaPadEvent);
        manta.addPadVelocityListener(this, &Instrument::mantaPadVelocityEvent);
        manta.addSliderListener(this, &Instrument::mantaSliderEvent);
        manta.addButtonListener(this, &Instrument::mantaButtonEvent);
        manta.addPadClickListener(this, &Instrument::padClickEvent);
        manta.addSliderClickListener(this, &Instrument::sliderClickEvent);
        manta.addButtonClickListener(this, &Instrument::buttonClickEvent);
    }
    else {
        manta.removePadListener(this, &Instrument::mantaPadEvent);
        manta.removePadVelocityListener(this, &Instrument::mantaPadVelocityEvent);
        manta.removeSliderListener(this, &Instrument::mantaSliderEvent);
        manta.removeButtonListener(this, &Instrument::mantaButtonEvent);
        manta.removePadClickListener(this, &Instrument::padClickEvent);
        manta.removeSliderClickListener(this, &Instrument::sliderClickEvent);
        manta.removeButtonClickListener(this, &Instrument::buttonClickEvent);
    }
}

//-----------
void Instrument::update() {
    if (guiToSwitchParameters) {
        setupGuiParameterGroup(guiToSwitchParametersName);
        guiToSwitchParameters = false;
    }
    
    manta.update();
    
    if (mantaMap.count(54) > 0)
        au.setParameter(mantaMap[54].parameterId, 0, ofMap(manta.getNumPads(), 0, 10, mantaMap[54].rmin, mantaMap[54].rmax));
    if (mantaMap.count(55) > 0)
        au.setParameter(mantaMap[55].parameterId, 0, ofMap(manta.getPadSum(), 0, 1024, mantaMap[55].rmin, mantaMap[55].rmax));
    if (mantaMap.count(56) > 0)
        au.setParameter(mantaMap[56].parameterId, 0, ofMap(manta.getPadAverage(), 0, 196, mantaMap[56].rmin, mantaMap[56].rmax));
    if (mantaMap.count(57) > 0)
        au.setParameter(mantaMap[57].parameterId, 0, ofMap(manta.getPerimeter(), 0, 2, mantaMap[57].rmin, mantaMap[57].rmax));
    if (mantaMap.count(58) > 0)
        au.setParameter(mantaMap[58].parameterId, 0, ofMap(manta.getAverageInterFingerDistance(), 0, 1, mantaMap[58].rmin, mantaMap[58].rmax));
    if (mantaMap.count(59) > 0)
        au.setParameter(mantaMap[59].parameterId, 0, ofMap(manta.getCentroidX(), 0, 1, mantaMap[59].rmin, mantaMap[59].rmax));
    if (mantaMap.count(60) > 0)
        au.setParameter(mantaMap[60].parameterId, 0, ofMap(manta.getCentroidY(), 0, 1, mantaMap[60].rmin, mantaMap[60].rmax));
    if (mantaMap.count(61) > 0)
        au.setParameter(mantaMap[61].parameterId, 0, ofMap(manta.getWeightedCentroidX(), 0, 1, mantaMap[61].rmin, mantaMap[61].rmax));
    if (mantaMap.count(62) > 0)
        au.setParameter(mantaMap[62].parameterId, 0, ofMap(manta.getWeightedCentroidY(), 0, 1, mantaMap[62].rmin, mantaMap[62].rmax));
    
    if (sequencerManta) {
        for (int r=0; r<6; r++) {
            for (int c=0; c<8; c++) {
                sequencer.setValue(r, c, manta.getPad(5 -r, c) * 0.0051);
            }
        }
    }
    sequencer.update();

    for (int i=0; i<128; i++) {
        if (noteEvents.count(i)>0) {
            if (noteEvents[i] == -1) continue;
            if (ofGetFrameNum() > noteEvents[i] + noteAutoFrameLength) {
                noteEvents[i] = -1;
                noteEvent(NOTE_OFF, i, noteVelocity);
            }
        }
    }
}

//-----------
void Instrument::noteEvent(NoteType type, int note, int velocity) {
    if      (type == NOTE_ON) {
        au.midiNoteOn(note, velocity);
        noteStatus[note] = true;
    }
    else if (type == NOTE_OFF) {
        au.midiNoteOff(note, velocity);
        noteStatus[note] = false;
    }
    else if (type == NOTE_AUTO) {
        au.midiNoteOn(note, velocity);
        noteEvents[note] = ofGetFrameNum();
    }
}

//-----------
void Instrument::chooseSequencerMode(string &s) {
    sequencerMode = (s == "notes") ? NOTES : PARAMETERS;
}

//-----------
void Instrument::toggleSmooth(string &s) {
    this->sequencerSmooth = !sequencerSmooth;
    sequencer.setSmooth(sequencerSmooth);
}


//-----------
void Instrument::mantaPadEvent(ofxMantaEvent &e) {
    int idx = e.col + 8 * e.row;
    if (mantaMap.count(idx) > 0) {
        au.setParameter(mantaMap[idx].parameterId, 0, ofMap(e.value, 0, 196.0, mantaMap[idx].rmin, mantaMap[idx].rmax));
    }
}

//-----------
void Instrument::mantaSliderEvent(ofxMantaEvent &e) {
    if (e.value == -1)  return;
    int idx = 48 + e.id;
    if (mantaMap.count(idx) > 0) {
        au.setParameter(mantaMap[idx].parameterId, 0, ofMap(e.value, 0, 4096, mantaMap[idx].rmin, mantaMap[idx].rmax));
    }
}

//-----------
void Instrument::mantaButtonEvent(ofxMantaEvent &e) {
    int idx = 50 + e.id;
    if (mantaMap.count(idx) > 0) {
        au.setParameter(mantaMap[idx].parameterId, 0, ofMap(e.value, 0, 196.0, mantaMap[idx].rmin, mantaMap[idx].rmax));
    }
}

//-----------
void Instrument::guiParametersEvent(ofxUIEventArgs &e) {
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
                    setupGuiPadInspector();
                    return;
                }
            }
            else {
                if (parameters[i].name == e.getName()) {
                    if (guiActiveManta < 48)
                        manta.markPad(guiActiveManta / 8, guiActiveManta % 8, true);
                    else if (guiActiveManta < 50)
                        manta.markSlider(guiActiveManta - 48, true);
                    else if (guiActiveManta < 54)
                        manta.markButton(guiActiveManta - 50, true);
                    mantaMap[guiActiveManta] = parameters[i];
                    setupGuiPadInspector();
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
                manta.markPad(guiActiveManta / 8, guiActiveManta % 8, false);
            else if (guiActiveManta < 50)
                manta.markSlider(guiActiveManta - 48, false);
            else if (guiActiveManta < 54)
                manta.markButton(guiActiveManta - 50, false);
            mantaMap.erase(guiActiveManta);
        }
        setupGuiPadInspector();
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
void Instrument::guiStatsEvent(ofxUIEventArgs &e) {
    if      (e.getName() == "numFingers")   setupGuiPadInspector(54);
    else if (e.getName() == "padSum")       setupGuiPadInspector(55);
    else if (e.getName() == "padAverage")   setupGuiPadInspector(56);
    else if (e.getName() == "perimeter")    setupGuiPadInspector(57);
    else if (e.getName() == "fingerDist")   setupGuiPadInspector(58);
    else if (e.getName() == "centroidX")    setupGuiPadInspector(59);
    else if (e.getName() == "centroidY")    setupGuiPadInspector(60);
    else if (e.getName() == "wCentroidX")   setupGuiPadInspector(61);
    else if (e.getName() == "wCentroidY")   setupGuiPadInspector(62);
    for (int i=0; i<6; i++) {
        if (e.getName() == "seq_row_"+ofToString(i)) {
            setupGuiSeqInspector(i);
        }
    }
}

//-----------
void Instrument::setupGuiPadInspector(int guiActiveManta) {
    guiActiveIsSequencer = false;
    this->guiActiveManta = guiActiveManta;
    guiP->clearWidgets();
    if      (guiActiveManta < 48)
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

    if (mantaMap.count(guiActiveManta) == 0) {
        vector<string> parameterNames;
        guiParameterGroups = guiP->addDropDownList("parameter groups", parameterGroupNames);
        guiParameters = guiP->addDropDownList("parameters", parameterNames);
        guiParameterGroups->setAutoClose(true);
        guiParameters->setAutoClose(true);
    }
    else {
        guiP->addLabel(mantaMap[guiActiveManta].name);
        guiParameterRange = guiP->addRangeSlider("pad range", mantaMap[guiActiveManta].min, mantaMap[guiActiveManta].max, mantaMap[guiActiveManta].rmin, mantaMap[guiActiveManta].rmax);
        guiP->addLabel("min: ");
        guiP->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
        guiParameterMin = guiP->addTextInput("min", ofToString(mantaMap[guiActiveManta].rmin), 80.0f);
        guiParameterMin->setAutoClear(false);
        guiP->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
        guiP->addLabel("max: ");
        guiP->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
        guiParameterMax = guiP->addTextInput("max", ofToString(mantaMap[guiActiveManta].rmax), 80.0f);
        guiParameterMax->setAutoClear(false);
        guiP->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
        guiP->addLabelButton("remove", false);
        guiP->addSpacer();
    }
}

//-----------
void Instrument::setupGuiSeqInspector(int guiActiveSeq) {
    guiActiveIsSequencer = true;
    this->guiActiveSeq = guiActiveSeq;
    guiP->clearWidgets();
    guiP->addLabel("sequencer ("+ofToString(guiActiveSeq)+")");
    guiP->addSpacer();
    
    if (seqMap.count(guiActiveSeq) == 0) {
        vector<string> parameterNames;
        guiParameterGroups = guiP->addDropDownList("parameter groups", parameterGroupNames);
        guiParameters = guiP->addDropDownList("parameters", parameterNames);
        guiParameterGroups->setAutoClose(true);
        guiParameters->setAutoClose(true);
    }
    else {
        guiP->addLabel(seqMap[guiActiveSeq].name);
        guiParameterRange = guiP->addRangeSlider("pad range", seqMap[guiActiveSeq].min, seqMap[guiActiveSeq].max, mantaMap[guiActiveManta].rmin, mantaMap[guiActiveManta].rmax);
        guiP->addLabel("min: ");
        guiP->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
        guiParameterMin = guiP->addTextInput("min", ofToString(seqMap[guiActiveSeq].rmin), 80.0f);
        guiParameterMin->setAutoClear(false);
        guiP->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
        guiP->addLabel("max: ");
        guiP->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
        guiParameterMax = guiP->addTextInput("max", ofToString(seqMap[guiActiveSeq].rmax), 80.0f);
        guiParameterMax->setAutoClear(false);
        guiP->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
        guiP->addLabelButton("remove", false);
        guiP->addSpacer();
    }
}

//-----------
void Instrument::setupGuiParameterGroup(string parameterGroupName) {
    guiParameters->clearToggles();
    guiParameters->addToggles(parameterGroups[parameterGroupName]);
    guiParameters->open();
}

//-----------
void Instrument::padClickEvent(int & pad) {
    setupGuiPadInspector(pad);
}

//-----------
void Instrument::sliderClickEvent(int & slider) {
    setupGuiPadInspector(48 + slider);
}

//-----------
void Instrument::buttonClickEvent(int & button) {
    setupGuiPadInspector(50 + button);
}

//-----------
void Instrument::mantaPadVelocityEvent(ofxMantaEvent &e) {
    int velocity = e.value;
    int idx = e.row * 8 + e.col;
    if (mantaSendNotes && mantaMap.count(idx) == 0) {
        int degree = (2*e.row - (int)(e.row/2) + e.col) % 7;
        int octave = floor((2*e.row - floor(e.row/2) + e.col)/7);
        int note = theory.getNote(60 + 12 * octave, degree);
        if (noteManual) {
            noteEvent(velocity==0 ? NOTE_OFF : NOTE_ON, note, velocity==0 ? 0 : ofRandom(90,120));
        }
        else {
            if (velocity > 0) {
                noteEvent(NOTE_AUTO, note, velocity);
            }
        }
    }
}

//-----------
void Instrument::sequencerStepEvent(vector<float> &column) {
    if (!sequencer.getSmooth()) {
        processColumn(column);
    }
}

//-----------
void Instrument::sequencerInterpolatedStepEvent(vector<float> &column) {
    if (sequencer.getSmooth()) {
        processColumn(column);
    }
}

//-----------
void Instrument::processColumn(vector<float> &column) {
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
    }
}

//-----------
void Instrument::draw() {
    if (visible) {
        manta.draw(x+165, y+5, 260);
        manta.drawStats(x+430, y+5, 260);
        sequencer.draw();
    }
}

//-----------
void Instrument::setupGui() {
    control.setName("aalto");
    
    vector<string> noteOptions, padOptions, seqOptions, seqRows;
    noteOptions.push_back("manual");
    noteOptions.push_back("auto");
    padOptions.push_back("notes");
    padOptions.push_back("parameters");
    seqOptions.push_back("notes");
    seqOptions.push_back("parameters");
    for (int i=0; i<6; i++) seqRows.push_back("seq_row_"+ofToString(i));
    
    control.addParameter("noteAutoLength", &noteAutoFrameLength, 1, 200);
    control.addParameter("defaultVelocity", &noteVelocity, 1, 127);
    control.addParameter("noteManual", &noteManual);
    control.addParameter("play manta", &mantaSendNotes);
    control.addParameter("sequencerManta", &sequencerManta);
    control.addEvent("toggle smooth", this, &Instrument::toggleSmooth);
    control.addMenu("sequencer", seqOptions, this, &Instrument::chooseSequencerMode);
    
    guiP = new ofxUICanvas("parameter inspector");
    guiP->setPosition(810, 5);
    guiP->setHeight(230);

    guiS = new ofxUICanvas("parameter inspector");
    guiS->setPosition(695, 5);
    guiS->setWidth(110);
    guiS->setHeight(230);
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
    guiS->addDropDownList("sequencer", seqRows);

    ofAddListener(guiS->newGUIEvent, this, &Instrument::guiStatsEvent);
    ofAddListener(guiP->newGUIEvent, this, &Instrument::guiParametersEvent);
}

//-----------
void Instrument::setupGuiPresets() {
    vector<string> presets;
    ofDirectory dir(ofToDataPath("presetsAudio/aalto/"));
    dir.allowExt("aupreset");
    dir.listDir();
    for(int i = 0; i < dir.numFiles(); i++) {
        string filename = dir.getName(i);
        ofStringReplace(filename, ".aupreset", "");
        presets.push_back(filename);
    }
    control.addMenu("presets", presets, this, &Instrument::loadPreset);
}

//-----------
void Instrument::printParameterList() {
    vector<AudioUnitParameterInfo> params = au.getParameterList();
    for (int i=0; i<params.size(); i++) {
        cout << "parameter :: "<<params[i].name<<" :: "<< params[i].minValue << "->"<<params[i].maxValue << endl;
    }
}

//-----------
void Instrument::savePreset(string filename) {
    if (filename=="") {
        filename = ofSystemTextBoxDialog("choose a preset name");
    }
    if (filename=="")   return;
    string path = ofToDataPath("presetsAudio/"+filename+".xml");
    string aaltoPath = ofToDataPath("presetsAudio/aalto/"+filename+".aupreset");

    au.saveCustomPresetAtPath(aaltoPath);
    
    ofXml xml;
    xml.addChild("AudioPreset");
    xml.setTo("AudioPreset");
    
    // aupreset
    xml.addValue("Aupreset", aaltoPath);
    
    // sequencer
    bool active = sequencer.getActive();
    bool discrete = sequencer.getDiscrete();
    int bpm = sequencer.getBpm();
    int cols = sequencer.getNumberCols();
    int rows = sequencer.getNumberRows();
    string values = ofToString(sequencer.getValue(0, 0));
    for (int r=0; r<rows; r++) {
        for (int c=0; c<cols; c++) {
            if (c==0 && r==0)   continue;
            values += ","+ofToString(sequencer.getValue(r, c));
        }
    }
    xml.addChild("Sequencer");
    xml.setTo("Sequencer");
    xml.addValue("Active", active ? "1" : "0");
    xml.addValue("Discrete", discrete ? "1" : "0");
    xml.addValue("Bpm", ofToString(bpm));
    xml.addValue("Rows", ofToString(rows));
    xml.addValue("Cols", ofToString(cols));
    xml.addValue("Values", values);
    xml.setToParent();
    
    // manta
    xml.addChild("Manta");
    xml.setTo("Manta");
    int idx = 0;
    for (int i=0; i<63; i++) {
        if (mantaMap.count(i) > 0) {
            ofXml xmlMapping;
            xml.addChild("Mapping");
            xml.setTo("Mapping["+ofToString(idx)+"]");
            xml.addValue("MantaId", ofToString(i));
            xml.addValue("Name", mantaMap[i].name);
            xml.addValue("ParameterId", ofToString(mantaMap[i].parameterId));
            xml.addValue("Min", ofToString(mantaMap[i].min));
            xml.addValue("Max", ofToString(mantaMap[i].max));
            xml.addValue("Rmin", ofToString(mantaMap[i].rmin));
            xml.addValue("Rmax", ofToString(mantaMap[i].rmax));
            xml.setToParent();
            idx++;
        }
    }
    xml.setToParent();
    
    // parameters
    xml.addChild("Parameters");
    xml.setTo("Parameters");
    xml.addValue("NoteManual", noteManual ? "1" : "0");
    xml.addValue("SequencerManta", sequencerManta ? "1" : "0");
    xml.addValue("MantaSendNotes", mantaSendNotes ? "1" : "0");
    xml.addValue("SequencerMode", sequencerMode == PARAMETERS ? "Parameters" : "Notes");
    xml.setToParent();

    xml.setToParent();
    xml.save(path);
    setupGuiPresets();
}

//-----------
void Instrument::loadPreset(string &filename) {
    string path = ofToDataPath("presetsAudio/"+filename+".xml");

    ofXml xml;
    xml.load(path);
    
    // sequencer
    if (xml.exists("Sequencer")) {
        xml.setTo("Sequencer");
        int rows = xml.getValue<int>("Rows");
        int cols = xml.getValue<int>("Cols");
        bool active = xml.getValue<string>("Active") == "1";
        bool discrete = xml.getValue<string>("Discrete") == "1";
        int bpm = xml.getValue<int>("Bpm");
        vector<string> valueStr = ofSplitString(xml.getValue<string>("Values"), ",");
        
        sequencer.setSize(rows, cols);
        sequencer.setActive(active);
        sequencer.setDiscrete(discrete);
        sequencer.setBpm(bpm);
        for (int i=0; i<valueStr.size(); i++) {
            int r = floor(i / cols);
            int c = i % cols;
            sequencer.setValue(r, c, ofToFloat(valueStr[i]));
        }
        xml.setToParent();
    }

    // manta
    if (xml.exists("Manta")) {
        mantaMap.erase(mantaMap.begin(), mantaMap.end());
        xml.setTo("Manta");
        if (xml.exists("Mapping")) {
            xml.setTo("Mapping[0]");
            do {
                int idx = xml.getValue<int>("MantaId");
                mantaMap[idx].parameterId = xml.getValue<int>("ParameterId");
                mantaMap[idx].name = xml.getValue<string>("Name");
                mantaMap[idx].min = xml.getValue<float>("Min");
                mantaMap[idx].max = xml.getValue<float>("Max");
                mantaMap[idx].rmin = xml.getValue<float>("Rmin");
                mantaMap[idx].rmax = xml.getValue<float>("Rmax");
            }
            while (xml.setToSibling());
            xml.setToParent();
        }
        xml.setToParent();
    }

    // parameters
    if (xml.exists("Parameters")) {
        xml.setTo("Parameters");
        noteManual = xml.getValue<string>("NoteManual") == "1";
        sequencerManta = xml.getValue<string>("SequencerManta") == "1";
        mantaSendNotes = xml.getValue<string>("MantaSendNotes") == "1";
        sequencerMode = xml.getValue<string>("SequencerMode") == "Notes" ? NOTES : PARAMETERS;
        xml.setToParent();
    }
    
    // aupreset
    if (xml.exists("Aupreset")) {
        string aaltoPath = xml.getValue<string>("Aupreset");
        au.loadCustomPresetAtPath(aaltoPath);
    }
}

//-----------
void Instrument::setGuiPosition(int x, int y) {
    this->x = x;
    this->y = y;
    control.setGuiPosition(x+5, y+5);
    sequencer.setPosition(x+165, y+210, 260, 130);
    guiP->setPosition(x+810, y+5);
    guiS->setPosition(x+695, y+5);
}

//-----------
void Instrument::setVisible(bool visible) {
    this->visible = visible;
    control.setVisible(visible);
    sequencer.setVisible(visible);
    guiP->setVisible(visible);
    guiS->setVisible(visible);
    
}

//-----------
Instrument::~Instrument() {
    setActive(false);
    ofRemoveListener(sequencer.getSequencer().sequencerEvent, this, &Instrument::sequencerStepEvent);
    ofRemoveListener(sequencer.interpolatedSequencerEvent, this, &Instrument::sequencerInterpolatedStepEvent);
    ofRemoveListener(guiS->newGUIEvent, this, &Instrument::guiStatsEvent);
    ofRemoveListener(guiP->newGUIEvent, this, &Instrument::guiParametersEvent);
    delete guiP;
    delete guiS;
}
