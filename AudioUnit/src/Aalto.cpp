#include "Aalto.h"


//-----------
void Aalto::setup(){
    aalto = ofxAudioUnitSampler('aumu','Aalt', 'MLbs');
    aalto.showUI();
    aalto.connectTo(mixer);
    mixer.connectTo(output);
    output.start();
    
    
    //a2 = ofxAudioUnitSampler('aumu', 'tYn6', 'UHfX');
    
    setupGui();
    setupGuiPresets();
    
    
    sequencer.setup(6, 8);
    sequencer.addBeatListener(this, &Aalto::sequencerStep);
    sequencer.setPosition(300, 500);
    
    manta.setup();
    manta.addPadListener(this, &Aalto::mantaPadEvent);
    manta.addPadVelocityListener(this, &Aalto::mantaPadVelocityEvent);

    //manta.removePadListener(this, &Aalto::mantaPadEvent);
    //manta.removePadVelocityListener(this, &Aalto::mantaPadVelocityEvent);

    
    for (int r=0; r<6; r++) {
        for (int c=0; c<8; c++) {
            int note = (2*r - (int)(r/2) + c) % 7;
            int octave = floor((2*r - floor(r/2) + c)/7);
            int note2 = theory.getNote(60+12*octave, note);
        }
    }
    
    noteMode = MANUAL;
    padMode = NOTES;
    
//    addParameterMapping(0, 123, 0.5, 0.8);
//    addParameterMapping(1, 123, 0.5, 0.8);
//    addParameterMapping(2, 123, 0.5, 0.8);
//    addParameterMapping(3, 123, 0.5, 0.8);
    
    
    
    
    vector<AudioUnitParameterInfo> params = aalto.getParameterList();
    string newGroupName = "parameter group";
    for (int i=0; i<params.size(); i++) {
        ParameterMapping parameter;
        parameter.name = params[i].name;
        parameter.parameterId = i;
        parameter.min = params[i].minValue;
        parameter.max = params[i].maxValue;
        parameters.push_back(parameter);
        int clumpId = params[i].clumpID;
        while (clumpId + 1 > parameterGroups.size()) {
            newGroupName = ofToString(params[i].name);
            vector<string> newGroup;
            parameterGroups[newGroupName] = newGroup;
        }
        parameterGroups[newGroupName].push_back(params[i].name);
    }

    
    manta.addPadClickListener(this, &Aalto::padClickEvent);
    //manta.removePadClickListener(this, &Aalto::padClickEvent);

    for (map<string, vector<string> >::iterator it=parameterGroups.begin(); it!=parameterGroups.end(); ++it) {
        parameterGroupNames.push_back(it->first);
    }
    
    
    guiP = new ofxUICanvas("parameter inspector");
    guiP->setPosition(465, 5);
    ofAddListener(guiP->newGUIEvent, this, &Aalto::guiParametersEvent);
    
    
    
    
    setupGui();
}

//-----------
void Aalto::update() {
    
    if (guiToSwitchParameters) {
        setGuiParameterGroup(guiToSwitchParametersName);
        guiToSwitchParameters = false;
    }
    
    
    
    bool mantaActive = false;
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
void Aalto::choosePadMode(string &s) {
    if (s=="notes") {
        padMode = NOTES;
    }
    else if (s=="parameters") {
        padMode = PARAMETERS;
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
    if (padMode == PARAMETERS) {
        int idx = e.col + 8*e.row;
        if (mappings.count(idx) > 0) {
        
            aalto.setParameter(mappings[idx].parameterId, 0,
                               ofMap(e.value / 196.0, 0, 1,
                                     mappings[idx].rmin,
                                     mappings[idx].rmax));
        }
 
    }
}

//-----------
void Aalto::mantaPadVelocityEvent(ofxMantaEvent &e) {
    int velocity = e.value;
    
    if (padMode == NOTES) {
        int degree = (2*e.row - (int)(e.row/2) + e.col) % 7;
        int octave = floor((2*e.row - floor(e.row/2) + e.col)/7);
        int note = theory.getNote(60 + 12 * octave, degree);
        if (noteMode == MANUAL) {
            if (e.value == 0) {
                noteEvent(NOTE_OFF, note, 100);
            }
            else {
                noteEvent(NOTE_ON, note, velocity);
            }
        }
        else if (noteMode == AUTO) {
            if (e.value > 0) {
                noteEvent(NOTE_AUTO, note, velocity);
            }
        }
    }
}

//-----------
void Aalto::sequencerStep(vector<float> &column) {
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
    control.addMenu("pads", padOptions, this, &Aalto::choosePadMode);
    control.addMenu("sequencer", seqOptions, this, &Aalto::chooseSequencerMode);
}

//-----------
void Aalto::setupGuiPresets() {
    vector<string> presets;
    ofDirectory dir(ofToDataPath("presetsAudio/aalto/"));
    dir.allowExt("aupreset");
    dir.listDir();
    for(int i = 0; i < dir.numFiles(); i++) {
        presets.push_back(dir.getName(i));
    }
    control.addMenu("presets", presets, this, &Aalto::loadPreset);
}

//-----------
void Aalto::printParameterList() {
    vector<AudioUnitParameterInfo> params = aalto.getParameterList();
    for (int i=0; i<params.size(); i++) {
        cout << "void set"<<params[i].name<<"(float value) { aalto.setParameter("<<i<<", 0, value); }   //"<< params[i].minValue << "->"<<params[i].maxValue << endl;
    }
    
    cout << "==========="<<endl;
    params = mixer.getParameterList();
    for (int i=0; i<params.size(); i++) {
        cout << "void set"<<params[i].name<<"(float value) { aalto.setParameter("<<i<<", 0, value); }   //"<< params[i].minValue << "->"<<params[i].maxValue << endl;
    }
    cout << "==========="<<endl;
    params = output.getParameterList();
    for (int i=0; i<params.size(); i++) {
        cout << "void set"<<params[i].name<<"(float value) { aalto.setParameter("<<i<<", 0, value); }   //"<< params[i].minValue << "->"<<params[i].maxValue << endl;
    }
    cout << "==========="<<endl;
    params = a2.getParameterList();
    for (int i=0; i<params.size(); i++) {
        cout << params[i].clumpID<< " void set"<<params[i].name<<"(float value) { aalto.setParameter("<<i<<", 0, value); }   //"<< params[i].minValue << "->"<<params[i].maxValue << endl;
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
    aalto.loadCustomPresetAtPath(ofToDataPath("presetsAudio/aalto/"+filename));
}
