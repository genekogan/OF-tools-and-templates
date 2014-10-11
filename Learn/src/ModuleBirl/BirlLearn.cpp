#include "BirlLearn.h"


//-------
BirlLearner::BirlLearner() : Learn(false) {
    guiPresets = new ofxUICanvas("Presets");
    guiMode = new ofxUICanvas("choose mode");
    oscInActive = false;
    setGuiInputsVisible(false);
    setupGui();
    setPerformanceMode();
}

//-------
void BirlLearner::setup(Birl &birl) {
    this->birl = birl;
    vector<float> & keys = birl.getKeys();
    vector<float> & keysDiscrete = birl.getKeysDiscrete();
    vector<float> & pressure = birl.getPressure();
    vector<float> & embouchure = birl.getEmbouchure();
    for (int i=0; i<keys.size(); i++) {
        addInput("key_"+ofToString(i), &keys[i], 0, 1);
    }
    for (int i=0; i<keysDiscrete.size(); i++) {
        addInput("keyDiscrete_"+ofToString(i), &keysDiscrete[i], 0, 1);
    }
    for (int i=0; i<pressure.size(); i++) {
        addInput("pressure_"+ofToString(i), &pressure[i], 0, 1);
    }
    for (int i=0; i<embouchure.size(); i++) {
        addInput("embouchure_"+ofToString(i), &embouchure[i], 0, 1);
    }
    setGuiInputsVisible(false);
}

//-------
void BirlLearner::setupOscSender(string host, int port) {
    if (host != "") {
        this->oscOutputHost = host;
        ((ofxUITextInput *) gui3->getWidget("oscHost"))->setTextString(oscOutputHost);
    }
    if (port > 0) {
        this->oscOutputPort = port;
        ((ofxUITextInput *) gui3->getWidget("oscPortOut"))->setTextString(ofToString(oscOutputPort));
    }
    oscOutActive = oscManager.setupSender(oscOutputHost, oscOutputPort);
}

//-------
BirlOutputParameter * BirlLearner::addOutput(string name, float *val, float min, float max) {
    BirlOutputParameter *newOutput = new BirlOutputParameter(name, val, min, max);
    newOutput->setInputParameters(inputs);
    newOutput->addParameterChangedListener((Learn*) this, &Learn::outputParameterChanged);
    newOutput->addParameterDeletedListener((Learn*) this, &Learn::outputParameterDeleted);
    newOutput->addParameterViewedListener((Learn*) this, &Learn::outputParameterViewed);
    newOutput->addParameterSelectedListener(this, &BirlLearner::parameterSelected);
    newOutput->setupGui();
    newOutput->setVisible(true);
    newOutput->setPreviewMode(mode);
    outputs.push_back(newOutput);
    if (oscManager.getSending()) {
        oscManager.registerToOsc(newOutput, true);
    }
    setGuiMode();
    return newOutput;
}

//-------
void BirlLearner::setPerformanceMode() {
    setGuiMode(BIRL_PERFORM);
}

//-------
void BirlLearner::setTrainingMode() {
    setGuiMode(BIRL_INPUTS);
    if (outputs.size() == 0) {
        addOutput("pitch", 40, 80); // one default parameter
    }
}

//-------
void BirlLearner::resetGuiPositions() {
    for (int i=0; i<outputs.size(); i++) {
        outputs[i]->setGuiPosition(220, 100+55*i);
        ((BirlOutputParameter *) outputs[i])->setGuiPreviewPosition(220, 100+30*i);
    }
}

//-------
void BirlLearner::resetInputs() {
    for (int i=0; i<outputs.size(); i++) {
        outputs[i]->setInputParameters(inputs);
    }
    if (oscManager.getReceiving()) {
        oscManager.clearInputTrackers();
        oscManager.registerToOscReceiver((vector<ParameterBase *> &) inputs);
        oscManager.registerToOscReceiver((vector<ParameterBase *> &) outputs);
    }
    resetGuiPositions();
}

//-----------
void BirlLearner::resetInputButtons() {
    LearnInputParameter *inputFirstKey = inputs[0];
    LearnInputParameter *inputFirstKeyDiscrete = inputs[birl.getKeys().size()];
    LearnInputParameter *inputFirstPressure = inputs[birl.getKeys().size() + birl.getKeysDiscrete().size()];
    LearnInputParameter *inputFirstEmbouchure = inputs[birl.getKeys().size() + birl.getKeysDiscrete().size() + birl.getPressure().size()];
    for (int i=0; i<outputs.size(); i++) {
        ((BirlOutputParameter *) outputs[i])->inputKeys = outputs[i]->getInputActive(inputFirstKey);
        ((BirlOutputParameter *) outputs[i])->inputKeysDiscrete = outputs[i]->getInputActive(inputFirstKeyDiscrete);
        ((BirlOutputParameter *) outputs[i])->inputPressure = outputs[i]->getInputActive(inputFirstPressure);
        ((BirlOutputParameter *) outputs[i])->inputEmbouchure = outputs[i]->getInputActive(inputFirstEmbouchure);
    }
}

//-------
void BirlLearner::setupGui() {
    guiStatusLabel = new ofxUILabel(120.0f, "", OFX_UI_FONT_SMALL, 16.0f);

    gui1->setColorOutline(ofColor(255,200));
    gui1->setDrawOutline(true);
    gui1->clearWidgets();
    gui1->setPosition(220, 5);
    gui1->setWidth(220);
    gui1->setHeight(60);
    gui1->addLabelButton("add output", false, 100, 50);
    gui1->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui1->addLabelButton("next", false, 100, 50);

    gui2->setColorOutline(ofColor(255,200));
    gui2->setDrawOutline(true);
    gui2->clearWidgets();
    gui2->setPosition(220, 5);
    gui2->setWidth(750);
    gui2->setHeight(60);
    gui2->addLabelButton("back", false, 100, 50);
    gui2->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui2->addLabelToggle("record", &inRecording, 100, 50);
    gui2->addLabelButton("train fast", false, 100, 22);
    gui2->addWidgetSouthOf(new ofxUILabelButton("train accurate", false,  100, 22, 0, 0, OFX_UI_FONT_SMALL), "train fast")->setPadding(2);
    gui2->addWidgetEastOf(new ofxUISlider("countdown", 0.0, 5.0, &trainCountdown, 100.0f, 9.0f), "train fast")->setPadding(2);
    gui2->addWidgetSouthOf(new ofxUISlider("duration", 0.0, 5.0, &trainDuration, 100.0f, 9.0f), "countdown")->setPadding(2);
    gui2->addWidgetEastOf(new ofxUIIntSlider("instanceRate", 1, 30, &instanceRate, 100.0f, 9.0f), "countdown")->setPadding(2);
    
    gui2->addWidgetSouthOf(guiStatusLabel, "instanceRate")->setPadding(4);
    //gui2->addWidgetEastOf(guiStatusLabel, "duration")->setPadding(4);
    
    gui2->addWidgetEastOf(new ofxUILabelToggle("predict", &predicting, 100, 50, 0, 0, OFX_UI_FONT_SMALL), "instanceRate");
    //gui2->addWidgetEastOf(new ofxUILabelToggle("predict", &predicting, 100, 50, 0, 0, OFX_UI_FONT_SMALL), "");
    
    gui2->addLabelButton("save", false, 100, 50);

    gui3->setColorOutline(ofColor(255,200));
    gui3->setDrawOutline(true);
    gui3->clearWidgets();
    gui3->setPosition(220, 5);
    gui3->setWidth(600);
    gui3->setHeight(60);
    gui3->addLabelToggle("predict", &predicting, 120, 50);
    gui3->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui3->addLabelToggle("send osc", &oscOutActive, 120, 50);
    gui3->addLabel("osc host");
    gui3->addTextInput("oscHost", ofToString(oscOutputHost), 120.0f)->setAutoClear(false);
    gui3->addWidgetSouthOf(new ofxUISpacer(0, 4, "gui3spacer"), "osc host");
    gui3->addWidgetSouthOf(new ofxUILabel("osc port", OFX_UI_FONT_MEDIUM), "gui3spacer");
    gui3->addTextInput("oscPortOut", ofToString(oscOutputPort), 120.0f)->setAutoClear(false);

    vector<string> presets;
    guiPresets->setDrawBack(false);
    guiPresets->clearWidgets();
    guiPresets->setPosition(ofGetWidth()-195, 0);
    guiPresets->setWidth(190);
    guiSelector = guiPresets->addDropDownList("Load Preset", presets);
    guiSelector->setColorOutline(ofColor(0, 255, 0, 200));
    guiSelector->setDrawOutline(true);
    guiSelector->setAllowMultiple(false);
    guiSelector->setAutoClose(false);
    guiPresets->autoSizeToFitWidgets();
    
    guiMode->setColorOutline(ofColor(255,200));
    guiMode->setDrawOutline(true);
    guiMode->clearWidgets();
    guiMode->setPosition(5, 5);
    guiMode->addLabelToggle("Perform", false, 80.0f);
    guiMode->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiMode->addLabelToggle("Train", false, 80.0f);
    guiMode->autoSizeToFitWidgets();

    ofAddListener(gui1->newGUIEvent, this, &BirlLearner::gui1Event);
    ofAddListener(gui2->newGUIEvent, this, &BirlLearner::gui2Event);
    ofAddListener(gui3->newGUIEvent, this, &BirlLearner::gui3Event);
    ofAddListener(guiMode->newGUIEvent, this, &BirlLearner::guiModeEvent);
    ofAddListener(guiPresets->newGUIEvent, this, &BirlLearner::guiPresetsEvent);
    
    resetPresets();
}

//-------
void BirlLearner::guiModeEvent(ofxUIEventArgs &e) {
    if (e.getName() == "Train") {
        setTrainingMode();
    }
    else if (e.getName() == "Perform") {
        setPerformanceMode();
    }
}

//-------
void BirlLearner::guiPresetsEvent(ofxUIEventArgs &e) {
    if (e.getParentName() == "Load Preset") {
        string path = loadPresetDialog(e.getName());
        loadPreset(path);
    }
}

//-------
void BirlLearner::setGuiMode(BirlMode mode) {
    this->mode = mode;
    gui1->setVisible(mode == BIRL_INPUTS);
    gui2->setVisible(mode == BIRL_TRAIN);
    gui3->setVisible(mode == BIRL_PERFORM);
    guiPresets->setVisible(mode == BIRL_INPUTS || mode == BIRL_PERFORM);
    for (int i=0; i<outputs.size(); i++) {
        ((BirlOutputParameter *) outputs[i])->setPreviewMode(mode);
        if (mode != BIRL_TRAIN)  outputs[i]->setExamplesVisible(false);
    }
    ((ofxUILabelToggle *) guiMode->getWidget("Train"))->setValue(mode != BIRL_PERFORM);
    ((ofxUILabelToggle *) guiMode->getWidget("Perform"))->setValue(mode == BIRL_PERFORM);
    resetGuiPositions();
}

//-------
void BirlLearner::gui1Event(ofxUIEventArgs &e) {
    if (e.getName() == "next") {
        if (e.getButton()->getValue() == 1) return;
        if (outputs.size() == 0)    {
            ofSystemAlertDialog("You must have at least one output parameter to proceed.");
            return;
        }
        for (int i=0; i<outputs.size(); i++) {
            if (outputs[i]->getNumInputs() == 0) {
                ofSystemAlertDialog("All output parameters must have at least one input parameter selected for training.");
                return;
            }
        }
        setGuiMode(BIRL_TRAIN);
    }
    Learn::gui1Event(e);
}

//-------
void BirlLearner::gui2Event(ofxUIEventArgs &e) {
    if (e.getName() == "back") {
        if (e.getButton()->getValue() == 1) return;
        setGuiMode(BIRL_INPUTS);
    }
    else if (e.getName() == "save") {
        if (e.getButton()->getValue() == 1) return;
        bool saved = savePreset("");
        if (saved) {
            resetPresets();
        }
    }
    Learn::gui2Event(e);
}

//-------
void BirlLearner::gui3Event(ofxUIEventArgs &e) {
    if (e.getName() == "back") {
        if (e.getButton()->getValue() == 1) return;
        setGuiMode(BIRL_TRAIN);
    }
    Learn::gui3Event(e);
}

//-------
void BirlLearner::parameterSelected(LearnParameter & parameter) {
    for (int i=0; i<outputs.size(); i++) {
        if (outputs[i] == &parameter)   continue;
        ((BirlOutputParameter *) outputs[i])->deselect();
    }
}

//-------
bool BirlLearner::savePreset(string filename) {
    if (filename=="") {
        filename = ofSystemTextBoxDialog("Choose a filename");
    }
    if (filename=="")   return false;
    string path = ofToDataPath("presets/"+filename+".xml");
    ofXml xml;
    xml.addChild("BirlPreset");
    xml.setTo("BirlPreset");
    saveOutputs(filename, xml);
    xml.save(path);
    return true;
}

//-------
void BirlLearner::loadPreset(string filename) {
    Presets presets;
    ofXml xml;
    bool xmlLoaded = xml.load(filename);
    if (!xmlLoaded) {
        cout << "failed to load preset " << filename << endl;
        return;
    }
    xml.setTo("BirlPreset");
    loadOutputs(xml);
    resetInputButtons();
    setGuiMode();
    predicting = true;
}

