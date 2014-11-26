#include "OutputAudioUnit.h"


//-----------
OutputAudioUnit::OutputAudioUnit() {
    gui = new ofxUICanvas("paramSelect");
    gui->setPosition(500, 400);
    ofAddListener(gui->newGUIEvent, this, &OutputAudioUnit::guiEvent);
}

//-----------
void OutputAudioUnit::setupAudioUnit(ofxAudioUnitSampler *audioUnit) {
    this->audioUnit = audioUnit;
    
    // setup audiounit parameters
    vector<AudioUnitParameterInfo> params = audioUnit->getParameterList();
    string newGroupName = "parameter group";
    for (int i=0; i<params.size(); i++) {
        ParameterMapping parameter;
        parameter.name = params[i].name;
        parameter.parameterId = i;
        parameter.min = params[i].minValue;
        parameter.max = params[i].maxValue;
        parameter.val = new float();
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

    setupGui();
}

//-----------
void OutputAudioUnit::setupGui() {
    gui->clearWidgets();
    gui->addLabel("add output");
    vector<string> parameterNames;
    guiParameterGroups = gui->addDropDownList("parameter groups", parameterGroupNames);
    guiParameters = gui->addDropDownList("parameters", parameterNames);
    guiParameterGroups->setAutoClose(true);
    guiParameters->setAutoClose(true);
}

//-----------
void OutputAudioUnit::setupGuiParameterGroup(string parameterGroupName) {
    guiParameters->clearToggles();
    guiParameters->addToggles(parameterGroups[parameterGroupName]);
    guiParameters->open();
}

//-----------
void OutputAudioUnit::guiEvent(ofxUIEventArgs &e) {
    if (e.getParentName() == "parameter groups") {
        guiToSwitchParametersName = e.getName();
        guiToSwitchParameters = true;
    }
    else if (e.getParentName() == "parameters") {
        for (int i=0; i<parameters.size(); i++) {
            if (parameters[i].name == e.getName()) {
                activeParameters.push_back(&parameters[i]);
                learn->addOutput(parameters[i].name, parameters[i].val, parameters[i].min, parameters[i].max);
            }
        }
    }
}

//-----------
void OutputAudioUnit::resetParameterMappings() {
    activeParameters.clear();
    vector<LearnOutputParameter*> *learnOutputs = learn->getOutputs();
    for (int i=0; i<learnOutputs->size(); i++) {
        for (int j=0; j<parameters.size(); j++) {
            if (learnOutputs->at(i)->getName() == parameters[j].name) {
                learnOutputs->at(i)->setReference(parameters[j].val);
                activeParameters.push_back(&parameters[j]);
                break;
            }
        }
    }
}

//-----------
void OutputAudioUnit::update() {
    if (guiToSwitchParameters) {
        setupGuiParameterGroup(guiToSwitchParametersName);
        guiToSwitchParameters = false;
    }
    for (int i=0; i<activeParameters.size(); i++) {
        audioUnit->setParameter(activeParameters[i]->parameterId, 0, *activeParameters[i]->val);
    }
}

//-----------
void OutputAudioUnit::draw() {
    if (visible) {
        
    }
}

//-----------
void OutputAudioUnit::setGuiPosition(int x, int y) {
    gui->setPosition(x, y);
}

//-----------
OutputAudioUnit::~OutputAudioUnit() {
    ofRemoveListener(gui->newGUIEvent, this, &OutputAudioUnit::guiEvent);
    gui->disable();
    delete guiParameterGroups;
    delete guiParameters;
    delete gui;
}