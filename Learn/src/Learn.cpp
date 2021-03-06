#include "Learn.h"


//-------
Learn::Learn(bool init) {
    trainDuration = 2;
    trainCountdown = 2;
    instanceRate = 15;
    startTime = -(trainDuration + trainCountdown);
    oscInActive = false;
    
    inputsVisible = true;
    outputsVisible = true;
    visible = true;
    
    guiInputX = DEFAULT_INPUT_X;
    guiInputY = DEFAULT_INPUT_Y;
    guiInputH = DEFAULT_INPUT_H;
    guiOutputX = DEFAULT_OUTPUT_X;
    guiOutputY = DEFAULT_OUTPUT_Y;
    guiOutputH = DEFAULT_OUTPUT_H;
    guiOutputSelectorX = DEFAULT_OUTPUT_SELECTOR_X;
    guiOutputSelectorY = DEFAULT_OUTPUT_SELECTOR_Y;
    
    training = false;
    predicting = false;
    recording = false;
    
    oscOutputHost = "localhost";
    oscOutputPort = 8000;
    oscInputPort = 9000;
    
    gui1 = new ofxUICanvas("setup");
    gui2 = new ofxUICanvas("train");
    gui3 = new ofxUICanvas("perform");
    
    visible = true;
    summaryX = 5;
    summaryY = 70;
    summaryVisible = false;
    dragging = false;
    activeOutput = -1;
    
    customFont = false;
    
    oscManager.registerOscEventListener("/toggleRecord", this, &Learn::oscEventSetRecording);
    oscManager.registerOscEventListener("/toggleTrainFast", this, &Learn::oscEventSetTrainFast);
    oscManager.registerOscEventListener("/toggleTrainAccurate", this, &Learn::oscEventSetTrainAccurate);
    oscManager.registerOscEventListener("/togglePredict", this, &Learn::oscEventSetPredicting);
    
    analyze.setGuiPosition(275, 400);
    
    if (init)   setupGui();
}

//-------
void Learn::update(){

    // send & receive osc
    if (oscInActive || oscOutActive) {
        oscManager.update();
    }
    
    // recording procedure
    if (countingDown) {
        float elapsed = ofGetElapsedTimef() - startTime;
        if (elapsed > trainCountdown) {
            countingDown = false;
            recording = true;
            gui2->setColorBack(ofColor(255,0,0));
        }
    }
    else if (recording) {
        float elapsed = ofGetElapsedTimef() - trainCountdown - startTime;
        if (elapsed < trainDuration) {
            if (ofGetFrameNum() % max(1,framesPerInstance) == 0) {
                recordInstance();
            }
        }
        else {
            stopRecording();
        }
    }
    
    // training procedure
    else if (training) {
        if (threadedLearn.getFinished()) {
            training = false;
            predicting = true;
            guiStatusLabel->setLabel("");
        }
    }
    
    // mapping inputs
    else if (analyzing) {
        analyze.update();
    }
    
    // prediction procedure
    if (predicting) {
        for (int i=0; i<outputs.size(); i++) {
            outputs[i]->predict();
        }
    }
    
    /*
    // update gui values
    if (inputsVisible) {
        for (int i=0; i<inputs.size(); i++) {
            vector<LearnInputParameter*> params = inputs[i]->getInputs();
            for (int j=0; j<params.size(); j++) {
                params[j]->updateGui();
            }
        }
    }
    */
}

//-------
void Learn::draw(){
    for (int i=0; i<outputs.size(); i++) {
        if (outputs[i]->isVisible()) {
            outputs[i]->draw();
        }
    }
    if (countingDown) {
        float elapsed = ofGetElapsedTimef() - startTime;
        guiStatusLabel->setLabel(" => starting in "+ofToString(ceil(trainDuration - elapsed)));
    }
    else if (recording) {
        float elapsed = ofGetElapsedTimef() - startTime;
        guiStatusLabel->setLabel(" => record for "+ofToString(ceil(trainDuration + trainCountdown - elapsed)));
    }
    else if (training) {
        guiStatusLabel->setLabel(" => classifying "+ofToString(threadedLearn.getTrainingIndex()));
    }
    
    if (summaryVisible) {
        drawSummary();
    }
    
    if (analyzing) {
        analyze.draw();
    }
}

//-------
void Learn::drawSummary(){
    ofPushMatrix();
    ofPushStyle();
    ofTranslate(summaryX, summaryY);
    ofSetColor(155);
    ofRect(0, 0, 265, 26 + 15 * max(activeInputs.size(), outputs.size()));
    ofSetColor(255);
    ofDrawBitmapString("Learn summary", 4, 17);
    ofTranslate(0, 22);
    for (int i=0; i<activeInputs.size(); i++) {
        float rv = (activeInputs[i]->get() - activeInputs[i]->getMin()) / (activeInputs[i]->getMax() - activeInputs[i]->getMin());
        ofPushMatrix();
        ofTranslate(5, 15*i);
        ofSetColor(100);
        ofRect(0, 0, 120, 14);
        ofSetColor(50);
        ofRect(0, 0, 120 * rv, 14);
        ofSetColor(255);
        ofDrawBitmapString(activeInputs[i]->getName(), 0, 12);
        ofPopMatrix();
    }
    ofTranslate(125, 0);
    for (int i=0; i<outputs.size(); i++) {
        float rv = (outputs[i]->get() - outputs[i]->getMin()) / (outputs[i]->getMax() - outputs[i]->getMin());
        ofPushMatrix();
        ofTranslate(5, 15*i);
        ofSetColor(100);
        ofRect(0, 0, 120, 14);
        ofSetColor(50);
        ofRect(0, 0, 120 * rv, 14);
        outputs[i]->getRecording() ? ofSetColor(255, 0, 0) : ofSetColor(255);
        ofCircle(128, 7, 6);
        activeOutput == i ? ofSetColor(255, 0, 0) : ofSetColor(255);
        ofDrawBitmapString(outputs[i]->getName(), 0, 12);
        ofPopMatrix();
    }
    ofPopStyle();
    ofPopMatrix();
}

//-------
void Learn::summaryClickParameters(int x, int y) {
    int idx = (int) ((y - summaryY - 22.0) / 15.0);
    if (x > 5+summaryX && x <= 125+summaryX) {
        if (idx >= 0 && idx < activeInputs.size()) {
            float val = (x - summaryX - 5.0) / 120.0;
            activeInputs[idx]->set(ofLerp(activeInputs[idx]->getMin(), activeInputs[idx]->getMax(), val));
            return;
        }
    }
    else if (x > 130+summaryX && x <= 250+summaryX) {
        if (idx >= 0 && idx < outputs.size()) {
            if (draggedFrames > 1) {
                float val = (x - summaryX - 130.0) / 120.0;
                outputs[idx]->set(ofLerp(outputs[idx]->getMin(), outputs[idx]->getMax(), val));
                return;
            }
        }
    }
    else if (x > 250+summaryX && x <= 265+summaryX) {
        if (idx >= 0 && idx < outputs.size()) {
            outputs[idx]->toggleRecording();
            return;
        }
    }
}

//-------
void Learn::summaryActiveParameter(int x, int y) {
    int idx = (int) ((y - summaryY - 22.0) / 15.0);
    if (x > 130+summaryX && x <= 250+summaryX) {
        if (idx >= 0 && idx < outputs.size() && idx != activeOutput) {
            summaryActivateParameter(idx);
        }
        else {
            summaryActivateParameter(-1);
        }
    }
    else if (x > 250+summaryX && x <= 265+summaryX) {
        summaryActivateParameter(idx);
    }
}

//-------
void Learn::summaryActivateParameter(int idx) {
    activeOutput = idx;
    activeInputs.clear();
    for (int i=0; i<inputs.size(); i++) {
        vector<LearnInputParameter *> params = inputs[i]->getInputs();
        for (int j=0; j<params.size(); j++) {
            if (activeOutput == -1 || outputs[activeOutput]->getInputActive(inputs[i])) {
                activeInputs.push_back(params[j]);
            }
        }
    }
}

//-------
void Learn::mousePressed(ofMouseEventArgs &e) {
    ofRectangle rect(summaryX, summaryY, 265, 26 + 15 * max(inputs.size(), outputs.size()));
    if (rect.inside(e.x, e.y)) {
        summaryClickParameters(e.x, e.y);
        dragging = true;
        draggedFrames = 0;
    }
}

//-------
void Learn::mouseDragged(ofMouseEventArgs &e) {
    draggedFrames++;
    ofRectangle rect(summaryX, summaryY, 265, 26 + 15 * max(inputs.size(), outputs.size()));
    if (dragging && rect.inside(e.x, e.y)) {
        summaryClickParameters(e.x, e.y);
    }
}

//-------
void Learn::mouseReleased(ofMouseEventArgs &e) {
    if (dragging && draggedFrames == 0) {
        summaryActiveParameter(e.x, e.y);
    }
    dragging = false;
    draggedFrames = 0;
}



//===========================================
//  MANAGING PARAMETERS

//-------
LearnInputGroup * Learn::addInput(string name, float *value, float min, float max, bool rangeLocked) {
    LearnInputParameter *newInput = new LearnInputParameter(name, value, min, max);
    LearnInputGroup *newGroup = new LearnInputGroup(name);
    newGroup->addInputParameter(newInput);
    inputs.push_back(newGroup);
    initializeInput(newInput);
    return newGroup;
}

//-------
LearnInputGroup * Learn::addInputGroup(string name) {
    LearnInputGroup *newGroup = new LearnInputGroup(name);
    inputs.push_back(newGroup);
    return newGroup;
}

//-------
LearnInputParameter * Learn::addInputToGroup(string groupName, string parameterName, float *value, float min, float max, bool rangeLocked) {
    for (int i=0; i<inputs.size(); i++) {
        if (groupName == inputs[i]->getName()) {
            LearnInputParameter * newInput = inputs[i]->addInputParameter(parameterName, value, min, max, rangeLocked);
            initializeInput(newInput);
            resetInputs();
            return newInput;
        }
    }
    ofLog(OF_LOG_ERROR, "Error: no input group found named "+groupName+"!");
}

//-------
LearnInputParameter * Learn::addInputToGroup(string groupName, string parameterName, float min, float max, bool rangeLocked) {
    for (map<string, vector<InputFeature> >::iterator it=inputFeatures.begin(); it!=inputFeatures.end(); ++it){
        for (int i=0; i<it->second.size(); i++) {
            string inputFeatureName = it->second[i].name=="" ? it->first : it->second[i].name;
            if (parameterName ==  inputFeatureName) {
                return addInputToGroup(groupName, parameterName, it->second[i].value, min, max, rangeLocked);
            }
        }
    }
    return addInputToGroup(groupName, parameterName, new float(), min, max, rangeLocked);
}


//-------
LearnOutputParameter * Learn::addOutput(string name, float *value, float min, float max, bool rangeLocked) {
	LearnOutputParameter *newOutput = new LearnOutputParameter(name, value, min, max, rangeLocked);
	initializeOutput(newOutput);
    return newOutput;
}

//-------
void Learn::initializeInput(LearnInputParameter *input) {
    if (customFont) {
        input->setFont(fontPath);
        input->setFontSizes(fontSmall, fontMedium, fontLarge);
    }
    input->setGuiPosition(10, 75+55*getNumberOfInputParameters());
    input->setVisible(inputsVisible);
    input->addParameterChangedListener(this, &Learn::inputParameterChanged);
    input->addParameterDeletedListener(this, &Learn::inputParameterDeleted);
    input->addParameterSelectedListener(this, &Learn::parameterSelected);
    
    resetInputs();
    for (int i=0; i<outputs.size(); i++) {
        outputs[i]->setInputParameters(inputs);
    }
    oscManager.registerToOsc((ParameterBase *) input, false);
    resetGuiPositions();
}

//-------
void Learn::initializeOutput(LearnOutputParameter *output, bool sendOsc, bool receiveOsc) {
    if (customFont) {
        output->setFont(fontPath);
        output->setFontSizes(fontSmall, fontMedium, fontLarge);
    }
    output->setGuiPosition(420, 75+55*outputs.size());
    output->setGuiInputsPosition(215, 75);
    output->setInputParameters(inputs);
    output->setVisible(outputsVisible);
    output->addParameterChangedListener(this, &Learn::outputParameterChanged);
    output->addParameterDeletedListener(this, &Learn::outputParameterDeleted);
    output->addParameterSelectedListener(this, &Learn::parameterSelected);
    output->addParameterViewedListener(this, &Learn::outputParameterViewed);
    if (oscManager.getSending() && sendOsc) {
        oscManager.registerToOsc(output, true);
    }
    if (oscManager.getReceiving() && receiveOsc) {
        oscManager.registerToOsc(output, false);
    }
    outputs.push_back(output);
}

//-------
LearnInputGroup * Learn::addInput(string name, float min, float max) {
    for (map<string, vector<InputFeature> >::iterator it=inputFeatures.begin(); it!=inputFeatures.end(); ++it){
        for (int i=0; i<it->second.size(); i++) {
            if (it->second[i].name == name) {
                return addInput(name, it->second[i].value, min, max, false);
            }
        }
    }
    return addInput(name, new float(), min, max, false);
}

//-------
LearnOutputParameter * Learn::addOutput(string name, float min, float max) {
    return addOutput(name, new float(), min, max, false);
}

//-------
void Learn::addParametersAsInputGroup(string name, vector<LearnInputParameter*> &newInputs) {
    LearnInputGroup *newGroup = addInputGroup(name);
    newGroup->addInputParameters(newInputs);
    resetInputs();
}


//-------
void Learn::removeInputGroup(string name) {
    vector<LearnInputGroup *>::iterator it=inputs.begin();
    while (it != inputs.end()) {
        if ((*it)->getName() == name) {
            (*it)->clearParameters();
            for (int i=0; i<outputs.size(); i++) {
                outputs[i]->removeInput(*it);
            }
            delete *it;
            it = inputs.erase(it);
        }
        else {
            ++it;
        }
    }
    resetInputs();
}

//-------
void Learn::resetInputs() {
    for (int i=0; i<outputs.size(); i++) {
        outputs[i]->setInputParameters(inputs);
    }
    if (oscManager.getReceiving()) {
        oscManager.clearInputTrackers();
        oscManager.registerToOscReceiver((vector<ParameterBase *> &) inputs);
        oscManager.registerToOscReceiver((vector<ParameterBase *> &) outputs);
    }
    resetGuiPositions();
    summaryActivateParameter(activeOutput);
}

//-------
void Learn::resetOutputs() {
    if (oscManager.getSending()) {
        oscManager.clearOutputTrackers();
        oscManager.registerToOscSender((vector<ParameterBase *> &) outputs);
    }
    if (oscManager.getReceiving()) {
        oscManager.clearInputTrackers();
        oscManager.registerToOscReceiver((vector<ParameterBase *> &) inputs);
        oscManager.registerToOscReceiver((vector<ParameterBase *> &) outputs);
    }
}

//-------
void Learn::clearInputs() {
    vector<LearnInputGroup *>::iterator it=inputs.begin();
    while (it != inputs.end()) {
        (*it)->clearParameters();
        delete *it;
        it = inputs.erase(it);
    }
    inputs.clear();
    resetInputs();
}

//-------
void Learn::clearOutputs() {
    vector<LearnOutputParameter *>::iterator it=outputs.begin();
    while (it != outputs.end()) {
        delete *it;
        it = outputs.erase(it);
    }
    outputs.clear();
    resetOutputs();
    resetInputs();
}

//-------
bool Learn::hasInput(string name) {
    for (int i=0; i<inputs.size(); i++) {
        if (inputs[i]->getName() == name) {
            return true;
        }
    }
    return false;
}

//-------
int Learn::getNumberOfInputParameters() {
    int n = 0;
    for (int i=0; i<inputs.size(); i++) {
        n += inputs[i]->getInputs().size();
    }
    return n;
}

//-----------
void Learn::addInputFeatureSet(string groupName) {
    if (inputFeatures.count(groupName) == 0) {
        ofLog(OF_LOG_ERROR, "Error: no feature set named "+groupName+" found");
        return;
    }
    else {
        addInputGroup(groupName);
        for (int f=0; f<inputFeatures[groupName].size(); f++) {
            InputFeature feature = inputFeatures[groupName][f];
            string name = feature.name == "" ? groupName : feature.name;
            addInputToGroup(groupName, name, feature.value, feature.min, feature.max);
            individualFeatures[name] = feature;
        }
    }
}

//-----------
void Learn::addSingleInputFeature(string name, float * value, float min, float max) {
    vector<InputFeature> features;
    InputFeature feature(value, min, max);
    features.push_back(feature);
    inputFeatures[name] = features;
    individualFeatures[name] = feature;
}




//===========================================
//  OSC

//-------
void Learn::setupOscSender(string host, int port) {
    if (host != "") {
        this->oscOutputHost = host;
        ((ofxUITextInput *) gui3->getWidget("oscHost"))->setTextString(oscOutputHost);
    }
    if (port > 0) {
        this->oscOutputPort = port;
        ((ofxUITextInput *) gui3->getWidget("oscPortOut"))->setTextString(ofToString(oscOutputPort));
    }
    enableOscOutputs(true);
}

//-------
void Learn::setupOscReceiver(int port) {
    if (port > 0) {
        this->oscInputPort = port;
        ((ofxUITextInput *) gui3->getWidget("oscPortIn"))->setTextString(ofToString(oscInputPort));
    }
    enableOscInputs(true);
}

//-------
void Learn::enableOscInputs(bool enable) {
    if (enable) {
        if (!oscInActive) {
            oscInActive = oscManager.setupReceiver(oscInputPort);
        }
        if (oscInActive) {
            oscManager.clearInputTrackers();
            oscManager.registerToOscReceiver((vector<ParameterBase *> &) inputs);
            oscManager.registerToOscReceiver((vector<ParameterBase *> &) outputs);
        }
    }
    else {
        oscManager.clearInputTrackers();
        oscInActive = false;
    }
}

//-------
void Learn::enableOscOutputs(bool enable) {
    if (enable) {
        if (!oscOutActive) {
            oscOutActive = oscManager.setupSender(oscOutputHost, oscOutputPort);
        }
        if (oscOutActive) {
            oscManager.clearOutputTrackers();
            oscManager.registerToOscSender((vector<ParameterBase *> &) outputs);
        }
    }
    else {
        oscManager.clearOutputTrackers();
        oscOutActive = false;
    }
}

//-------
void Learn::oscEventSetRecording(bool &b) {
    if (b)  startRecording();
    else    stopRecording();
}

//-------
void Learn::oscEventSetTrainFast(bool &b) {
    if (b) {
        if (recording)  stopRecording();
        trainClassifiers("fast");
    }
}

//-------
void Learn::oscEventSetTrainAccurate(bool &b) {
    if (b) {
        if (recording)  stopRecording();
        trainClassifiers("accurate");
    }
}

//-------
void Learn::oscEventSetPredicting(bool &b) {
    predicting = b;
}




//===========================================
//  GUI INTERACTION

//-------
void Learn::setupGui() {
    guiStatusLabel = new ofxUILabel(120.0f, "", OFX_UI_FONT_SMALL, 16.0f);
    ofxUILabelButton *bTouchOscInOut = new ofxUILabelButton("T", false, 12, 22, 0, 0, OFX_UI_FONT_SMALL);
    bTouchOscInOut->setName("touchOscInOut");
    
    vector<string> presets;
    guiSelector = new ofxUIDropDownList("Load Preset", presets, 130, 0, 0, OFX_UI_FONT_SMALL);
    guiSelector->setAllowMultiple(false);
    guiSelector->setAutoClose(true);
    
    gui1->setColorOutline(ofColor(255,200));
    gui1->setDrawOutline(true);
    gui1->clearWidgets();
    gui1->setPosition(5, 5);
    gui1->setWidth(120);
    gui1->setHeight(60);
    gui1->addLabelButton("add input", false, 90, 22);
    gui1->addWidgetSouthOf(new ofxUILabelButton("add output", false,  109, 22, 0, 0, OFX_UI_FONT_SMALL), "add input")->setPadding(2);
    gui1->addWidgetEastOf(bTouchOscInOut, "add input")->setPadding(1);
    
    gui2->setColorOutline(ofColor(255,200));
    gui2->setDrawOutline(true);
    gui2->clearWidgets();
    gui2->setPosition(130, 5);
    gui2->setWidth(450);
    gui2->setHeight(60);
    gui2->addLabelToggle("analyzer", &analyzing, 100, 22);
    gui2->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui2->addLabelButton("train fast", false, 100, 22);
    gui2->addWidgetSouthOf(new ofxUILabelToggle("record", &inRecording,  100, 22, 0, 0, OFX_UI_FONT_SMALL), "analyzer")->setPadding(2);
    gui2->addWidgetSouthOf(new ofxUILabelButton("train accurate", false,  100, 22, 0, 0, OFX_UI_FONT_SMALL), "train fast")->setPadding(2);
    gui2->addWidgetEastOf(new ofxUISlider("countdown", 0.0, 5.0, &trainCountdown, 100.0f, 9.0f), "train fast")->setPadding(2);
    gui2->addWidgetSouthOf(new ofxUISlider("duration", 0.0, 5.0, &trainDuration, 100.0f, 9.0f), "countdown")->setPadding(2);
    gui2->addWidgetEastOf(new ofxUIIntSlider("instanceRate", 1, 30, &instanceRate, 100.0f, 9.0f), "countdown")->setPadding(2);
    gui2->addWidgetSouthOf(guiStatusLabel, "instanceRate")->setPadding(4);
    
    gui3->setColorOutline(ofColor(255,200));
    gui3->setDrawOutline(true);
    gui3->clearWidgets();
    gui3->setPosition(585, 5);
    gui3->setWidth(438);
    gui3->setHeight(60);
    gui3->addLabelToggle("predict", &predicting, 100, 22);
    gui3->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui3->addLabelButton("save", false, 100, 50);
    gui3->addLabel("oscLabel", "osc");
    gui3->addTextInput("oscPortIn", ofToString(oscInputPort), 46.0f)->setAutoClear(false);
    gui3->addTextInput("oscHost", oscOutputHost, 78.0f)->setAutoClear(false);
    gui3->addTextInput("oscPortOut", ofToString(oscOutputPort), 46.0f)->setAutoClear(false);
    gui3->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    gui3->addWidgetSouthOf(new ofxUISpacer(0, 5, "gui3Spacer"), "oscLabel");
    gui3->addWidgetSouthOf(new ofxUILabelToggle("I", &inputsVisible), "gui3Spacer")->setPadding(2);
    gui3->addWidgetEastOf(new ofxUILabelToggle("O", &outputsVisible), "I")->setPadding(2);
    gui3->addWidgetEastOf(new ofxUILabelToggle("S", &summaryVisible), "O")->setPadding(2);
    gui3->addWidgetEastOf(guiSelector, "S");
    gui3->addWidgetSouthOf(new ofxUILabelToggle("osc <", &oscInActive, 46, 22, 0, 0, OFX_UI_FONT_SMALL), "predict");
    gui3->addWidgetEastOf(new ofxUILabelToggle("osc >", &oscOutActive, 46, 22, 0, 0, OFX_UI_FONT_SMALL), "osc <");
    
    ofAddListener(gui1->newGUIEvent, this, &Learn::gui1Event);
    ofAddListener(gui2->newGUIEvent, this, &Learn::gui2Event);
    ofAddListener(gui3->newGUIEvent, this, &Learn::gui3Event);
    
    resetPresets();
    summaryActivateParameter(-1);
}

//-------
void Learn::gui1Event(ofxUIEventArgs &e) {
    if (e.getName() == "add input") {
        if (e.getButton()->getValue() == 1) return;
        addInput("newInput"+ofToString(newInputCounter++), 0, 1);
    }
    else if (e.getName() == "add output") {
        if (e.getButton()->getValue() == 1) return;
        addOutput("newOutput"+ofToString(newOutputCounter++), 0, 1);
    }
    else if (e.getName() == "touchOscIn") {
        if (e.getButton()->getValue() == 1) return;
        saveInputsToTouchOsc();
    }
    else if (e.getName() == "touchOscOut") {
        if (e.getButton()->getValue() == 1) return;
        saveOutputsToTouchOsc();
    }
    else if (e.getName() == "touchOscInOut") {
        if (e.getButton()->getValue() == 1) return;
        saveInputsAndOutputsToTouchOsc();
    }
}

//-------
void Learn::gui2Event(ofxUIEventArgs &e) {
    if (e.getName() == "record") {
        e.getButton()->getValue() ? startRecording() : stopRecording();
    }
    else if (e.getName() == "train fast") {
        if (e.getButton()->getValue() == 1) return;
        if (recording)  stopRecording();
        trainClassifiers("fast");
    }
    else if (e.getName() == "train accurate") {
        if (e.getButton()->getValue() == 1) return;
        if (recording)  stopRecording();
        trainClassifiers("accurate");
    }
    else if (e.getName() == "analyzer") {
        e.getButton()->getValue() ? startAnalyzer() : stopAnalyzer();
    }
}

//-------
void Learn::gui3Event(ofxUIEventArgs &e) {
    if (e.getName() == "save") {
        if (e.getButton()->getValue() == 1) return;
        bool saved = savePreset();
        if (saved) {
            resetPresets();
        }
    }
    else if (e.getParentName() == "Load Preset") {
        string filepath = loadPresetDialog(e.getName());
        loadPreset(filepath);
    }
    else if (e.getName() == "oscHost" || e.getName() == "oscPortOut") {
        string newHost = ((ofxUITextInput *) gui3->getWidget("oscHost"))->getTextString();
        int newPort = ofToInt(((ofxUITextInput *) gui3->getWidget("oscPortOut"))->getTextString());
        if ((newHost != oscManager.getHost() || newPort != oscManager.getSenderPort())
            && (newHost != "" && newPort > 0)) {
            enableOscOutputs(false);
            setupOscSender(newHost, newPort);
            enableOscOutputs(oscOutActive);
        }
    }
    else if (e.getName() == "oscPortIn") {
        int newPort = ofToInt(((ofxUITextInput *) gui3->getWidget("oscPortIn"))->getTextString());
        if (newPort != oscManager.getReceiverPort() && newPort > 0) {
            enableOscInputs(false);
            setupOscReceiver(newPort);
            enableOscInputs(oscInActive);
        }
    }
    else if (e.getName() == "osc <") {
        enableOscInputs(e.getToggle()->getValue());
    }
    else if (e.getName() == "osc >") {
        enableOscOutputs(e.getToggle()->getValue());
    }
    else if (e.getName() == "I") {
        setGuiInputsVisible(inputsVisible);
        if (inputsVisible) {
            setGuiSummaryView(false);
        }
    }
    else if (e.getName() == "O") {
        setGuiOutputsVisible(outputsVisible);
    }
    else if (e.getName() == "S") {
        setGuiSummaryView(summaryVisible);
    }
}

//-------
void Learn::resetGuiPositions() {
    int idx = 0;
    for (int i=0; i<inputs.size(); i++) {
        vector<LearnInputParameter*> params = inputs[i]->getInputs();
        for (int j=0; j<params.size(); j++) {
            params[j]->setGuiPosition(10, 75+55*idx);
            idx++;
        }
    }
    for (int i=0; i<outputs.size(); i++) {
        outputs[i]->setGuiPosition(420, 75+55*i);
        outputs[i]->setGuiInputsPosition(215, 75);
    }
}




//===========================================
//  VISIBILITY

//-------
void Learn::setVisible(bool visible) {
    this->visible = visible;
    for (int i=0; i<inputs.size(); i++) {
        vector<LearnInputParameter*> params = inputs[i]->getInputs();
        for (int j=0; j<params.size(); j++) {
            params[j]->setVisible(visible && inputsVisible);
        }
    }
    for (int i=0; i<outputs.size(); i++) {
        outputs[i]->setVisible(visible && outputsVisible);
    }
    gui1->setVisible(visible);
    gui2->setVisible(visible);
    gui3->setVisible(visible);
}

//-------
void Learn::setGuiInputsVisible(bool visible) {
    this->inputsVisible = visible;
    for (int i=0; i<inputs.size(); i++) {
        vector<LearnInputParameter*> params = inputs[i]->getInputs();
        for (int j=0; j<params.size(); j++) {
            params[j]->setVisible(inputsVisible);
        }
    }
}

//-------
void Learn::setGuiOutputsVisible(bool visible) {
    this->outputsVisible = visible;
    for (int i=0; i<outputs.size(); i++) {
        outputs[i]->setVisible(outputsVisible);
    }
}

//-------
void Learn::setGuiSummaryView(bool summaryVisible) {
    this->summaryVisible = summaryVisible;
    if (summaryVisible) {
        setGuiInputsVisible(false);
        ofAddListener(ofEvents().mousePressed, this, &Learn::mousePressed);
        ofAddListener(ofEvents().mouseDragged, this, &Learn::mouseDragged);
        ofAddListener(ofEvents().mouseReleased, this, &Learn::mouseReleased);
    }
    else {
        ofRemoveListener(ofEvents().mousePressed, this, &Learn::mousePressed);
        ofRemoveListener(ofEvents().mouseDragged, this, &Learn::mouseDragged);
        ofRemoveListener(ofEvents().mouseReleased, this, &Learn::mouseReleased);
    }
}




//===========================================
//  GUI

//-------
void Learn::setGuiInputPosition(int x, int y, int h) {
    guiInputX = x;
    guiInputY = y;
    guiInputH = h;
}

//-------
void Learn::setGuiOutputPosition(int x, int y, int h) {
    guiOutputX = x;
    guiOutputY = y;
    guiOutputH = h;
}

//-------
void Learn::setGuiInputSelectorPosition(int x, int y) {
    guiOutputSelectorX = x;
    guiOutputSelectorY = y;
}




//===========================================
//  PARAMETER EVENTS

//-------
void Learn::inputParameterChanged(LearnParameter & input) {
    resetInputs();
}

//-------
void Learn::outputParameterChanged(LearnParameter & output) {
    resetOutputs();
}

//-------
void Learn::inputParameterDeleted(LearnParameter & parameter) {
    vector<LearnInputGroup *>::iterator it=inputs.begin();
    while (it != inputs.end()) {
        vector<LearnInputParameter *> params = (*it)->getInputs();
        vector<LearnInputParameter *>::iterator itp=params.begin();
        while (itp != params.end()) {
            if (*itp == &parameter) {
                vector<string> dependentOutputs;
                for (int i=0; i<outputs.size(); i++) {
                    if (outputs[i]->getInputActive(*it) && (outputs[i]->getNumInstances()>0 || outputs[i]->getTrained())) {
                        dependentOutputs.push_back(outputs[i]->getName());
                    }
                }
                if (dependentOutputs.size() > 0) {
                    string msg = "The following output parameters are trained with this input: ";
                    msg += ofJoinString(dependentOutputs, ", ");
                    msg += ".\nAll of their examples and classifiers will be erased. Confirm?";
                    bool confirm = ofSystemChoiceDialog(msg);
                    if (!confirm)   return;
                }
                (*it)->removeParameter(*itp);
                itp = params.erase(itp);
            }
            else ++itp;
        }
        if ((*it)->getInputs().size() == 0) {
            inputGroupDeleted((LearnInputGroup &) *it);
            it = inputs.erase(it);
        }
        else {
            ++it;
        }
    }
    resetInputs();
}

//-------
void Learn::inputGroupDeleted(LearnInputGroup & input) {
    vector<LearnInputGroup *>::iterator it=inputs.begin();
    while (it != inputs.end()) {
        if (*it == &input) {
            (*it)->clearParameters();
            delete *it;
            it = inputs.erase(it);
        }
        else {
            ++it;
        }
    }
    resetInputs();
}

//-------
void Learn::outputParameterDeleted(LearnParameter & output) {
    vector<LearnOutputParameter *>::iterator it=outputs.begin();
    while (it != outputs.end()) {
        if (*it == &output) {
            (*it)->setVisible(false);
            delete (*it);
            it = outputs.erase(it);
        }
        else ++it;
    }
    oscManager.registerToOscReceiver((vector<ParameterBase *> &) outputs);
    resetOutputs();
    resetGuiPositions();
}

//-------
void Learn::outputParameterViewed(LearnOutputParameter & output) {
    for (int i=0; i<outputs.size(); i++) {
        if (outputs[i] == &output)   continue;
        outputs[i]->setInputsVisible(false);
        outputs[i]->setExamplesVisible(false);
    }
}

//-------
void Learn::parameterSelected(LearnParameter & parameter) {
    for (int i=0; i<inputs.size(); i++) {
        vector<LearnInputParameter *> params = inputs[i]->getInputs();
        for (int j=0; j<params.size(); j++) {
            if (params[j] == &parameter)   continue;
            params[j]->deselect();
        }
    }
    for (int i=0; i<outputs.size(); i++) {
        if (outputs[i] == &parameter)   continue;
        outputs[i]->deselect();
    }
}




//===========================================
//  INPUT MAPPING PROCEDURE

//-------
void Learn::startAnalyzer() {
    analyzing = true;
    analyze.setVisible(true);
    analyze.setup();
    analyze.setInputs(&inputs);
    analyze.setOutputs(&outputs);
}

//-------
void Learn::stopAnalyzer() {
    analyzing = false;
    for (int i=0; i<inputs.size(); i++) {
        vector<LearnInputParameter *> params = inputs[i]->getInputs();
        for (int j=0; j<params.size(); j++) {
            params[j]->deselect();
        }
        
    }
    analyze.setVisible(false);
}




//===========================================
//  RECORD + LEARN

//-------
void Learn::startRecording() {
    bool atLeastOneOutputRecording = false;
    for (int i=0; i<outputs.size(); i++) {
        if (outputs[i]->getRecording()) {
            atLeastOneOutputRecording = true;
        }
    }
    if (atLeastOneOutputRecording) {
        startTime = ofGetElapsedTimef();
        framesPerInstance = ofGetFrameRate() / (float) instanceRate;
        countingDown = true;
        gui2->setColorBack(ofColor(0, 0, 255));
    }
    else {
        ofSystemAlertDialog("You need to have at least one output selected for recording.");
        ((ofxUILabelToggle *) gui2->getWidget("record"))->setValue(false);
    }
}

//-------
void Learn::stopRecording() {
    countingDown = false;
    recording = false;
    inRecording = false;
    guiStatusLabel->setLabel("  => "+ofToString(currentNewInstances)+" added");
    currentNewInstances = 0;
    gui2->setColorBack(ofColor(0,100));
}

//-------
void Learn::recordInstance() {
    for (int i=0; i<outputs.size(); i++) {
        if (!outputs[i]->getRecording())    continue;
        if (currentNewInstances==0 && outputs[i]->getNumInstances()>0) {
            outputs[i]->addDataPage();
        }
        outputs[i]->addInstance();
    }
    currentNewInstances++;
}

//-------
void Learn::trainClassifiers(string trainStrategy) {
    bool emptyParameters = false;
    for (int i=0; i<outputs.size(); i++) {
        if (outputs[i]->getNumInstances()==0) {
            emptyParameters = true;
            break;
        }
    }
    if (emptyParameters) {
        bool train = ofSystemChoiceDialog("Warning: some parameters have no examples given and will be skipped in training. Proceed?");
        if (!train) return;
    }
    threadedLearn.train(&outputs, trainStrategy);
    training = true;
}




//===========================================
//  TOUCH OSC

//-------
void Learn::saveInputsToTouchOsc() {
    oscManager.saveTouchOscLayout("touchOsc_inputs", (vector<ParameterBase *> &) inputs);
}

//-------
void Learn::saveOutputsToTouchOsc() {
    oscManager.saveTouchOscLayout("touchOsc_outputs", (vector<ParameterBase *> &) outputs);
}

//-------
void Learn::saveInputsAndOutputsToTouchOsc() {
    ofxTouchOsc touchOsc;
    touchOsc.setScale(320, 540);
    
    // control page
    bool b;
    Parameter<bool> *recordButton = new Parameter<bool>("record", b);
    Parameter<bool> *trainFButton = new Parameter<bool>("train fast", b);
    Parameter<bool> *trainAButton = new Parameter<bool>("train accurate", b);
    Parameter<bool> *predictButton = new Parameter<bool>("predict", b);
    recordButton->setOscAddress("/toggleRecord");
    trainFButton->setOscAddress("/toggleTrainFast");
    trainAButton->setOscAddress("/toggleTrainAccurate");
    predictButton->setOscAddress("/togglePredict");
    vector<ParameterBase *> controlButtons;
    controlButtons.push_back(recordButton);
    controlButtons.push_back(trainFButton);
    controlButtons.push_back(trainAButton);
    controlButtons.push_back(predictButton);
    
    // setup touch osc layout
    ofxTouchOscPage * pageControl = oscManager.makeTouchOscPage("control", controlButtons);
    ofxTouchOscPage * pageInputs = oscManager.makeTouchOscPage("inputs", (vector<ParameterBase *> &) inputs);
    ofxTouchOscPage * pageOutputs = oscManager.makeTouchOscPage("outputs", (vector<ParameterBase *> &) outputs);
    touchOsc.addPage(pageControl);
    touchOsc.addPage(pageInputs);
    touchOsc.addPage(pageOutputs);
    touchOsc.save("Learn_Parameters");
}




//===========================================
//  PRESETS

//-------
bool Learn::savePreset(string filename) {
    if (filename=="") {
        filename = ofSystemTextBoxDialog("Choose a filename");
    }
    if (filename=="")   return false;
    ofXml xml;
    xml.addChild("LearnPreset");
    xml.setTo("LearnPreset");
    savePresetProcedure(filename, xml);
    string path = ofToDataPath("presets/"+filename+".xml");
    xml.save(path);
    return true;
}

//-------
void Learn::savePresetProcedure(string filename, ofXml &xml) {
    saveInputs(filename, xml);
    saveOutputs(filename, xml);
}

//-------
void Learn::saveInputs(string filename, ofXml &xml) {
    Presets presets;
    
    /*
    xml.addChild("CustomInputs");
    xml.setTo("CustomInputs");
    for (map<string, CustomFeatureSet>::iterator it=customFeatureSets.begin(); it!=customFeatureSets.end(); ++it){
        string name = it->first;
        CustomFeatureSet cfs = it->second;
        ofXml xmlm;
        xmlm.addChild("CustomInput");
        xmlm.setTo("CustomInput");
        xmlm.addValue("Name", name);
        for (int i=0; i<cfs.features.size(); i++) {
            ofXml xmlf;
            xmlf.addChild("Parameter");
            xmlf.setTo("Parameter");
            xmlf.addValue("Name", cfs.features[i].name);
            xmlm.addXml(xmlf);
        }
        xml.addXml(xmlm);
    }
    xml.setToParent();
     */
    
    xml.addChild("Inputs");
    xml.setTo("Inputs");
    for (int i=0; i<inputs.size(); i++) {
        ofXml xmlg;
        xmlg.addChild("InputGroup");
        xmlg.setTo("InputGroup");
        xmlg.addValue("Name", inputs[i]->getName());
        
        vector<LearnInputParameter*> params = inputs[i]->getInputs();
        for (int j=0; j<params.size(); j++) {
            ofXml xmlp = presets.getXml(params[j]);
            xmlg.addXml(xmlp);
        }
        xml.addXml(xmlg);
    }
    xml.setToParent();
}

//-------
void Learn::saveOutputs(string filename, ofXml &xml) {
    Presets presets;
    xml.addChild("Outputs");
    xml.setTo("Outputs");
    for (int i=0; i<outputs.size(); i++) {

        // add inputs
        ofXml xmlp = presets.getXml(outputs[i]);
        xmlp.addChild("Inputs");
        xmlp.setTo("Inputs");

        vector<vector<vector<float> > > instances = outputs[i]->getInstances();
        vector<LearnInputGroup*> activeInputs = outputs[i]->getActiveInputs();

        for (int i=0; i<activeInputs.size(); i++) {
            xmlp.addValue("Input", activeInputs[i]->getName());
        }
        xmlp.setToParent();

        // add pages of examples
        xmlp.addChild("Examples");
        xmlp.setTo("Examples");
        for (int p=0; p<instances.size(); p++) {
            xmlp.addChild("Page");
            xmlp.setTo("Page["+ofToString(p)+"]");
            for (int j=0; j<instances[p].size(); j++) {
                string instanceString = ofToString(instances[p][j][0]);
                for (int k=1; k<instances[p][j].size(); k++) {
                    instanceString += ","+ofToString(instances[p][j][k]);
                }
                xmlp.addValue("Example", instanceString);
            }
            xmlp.setToParent();
        }
        xmlp.setToParent();

        // add classifier info
        if (outputs[i]->getTrained()) {
            xmlp.addChild("Classifier");
            xmlp.setTo("Classifier");
            if (outputs[i]->getLearnModel() == LearnOutputParameter::SVM) {
                ofDirectory dir;
                string classifierPath = dir.getAbsolutePath();
                classifierPath += "presets/classifiers/"+filename+"_"+outputs[i]->getName()+".dat";
                outputs[i]->saveClassifier(classifierPath);
                xmlp.addValue("Type", "SVM");
                xmlp.addValue("Path", classifierPath);
            }
            else if (outputs[i]->getLearnModel() == LearnOutputParameter::MLP) {
                xmlp.addValue("Type", "MLP");
                xmlp.addValue("NumHiddenLayers", ofToString(outputs[i]->getMlpNumHiddenLayers()));
                xmlp.addValue("MaxNumSamples", ofToString(outputs[i]->getMlpMaxSamples()));
                xmlp.addValue("TargetRmse", ofToString(outputs[i]->getMlpTargetRmse()));
                string w1s = ofToString(outputs[i]->getMlpCoefficients1());
                string w3s = ofToString(outputs[i]->getMlpCoefficients2());
                xmlp.addValue("W1", w1s.substr(1, w1s.size()-2));
                xmlp.addValue("W3", w3s.substr(1, w3s.size()-2));
            }
            xmlp.setToParent();
        }
        xml.addXml(xmlp);
    }
    xml.setToParent();
}

//-------
string Learn::loadPresetDialog(string filename) {
    if (filename == "...load from file...") {
        ofFileDialogResult file = ofSystemLoadDialog();
        if (file.bSuccess) {
            return file.filePath;
        }
    }
    else {
        return ofToDataPath("presets/"+filename);
    }
}

//-------
void Learn::loadPreset(string filename) {
    Presets presets;
    ofXml xml;
    if (filename=="") return;
    bool xmlLoaded = xml.load(filename);
    if (!xmlLoaded) {
        ofLog(OF_LOG_ERROR, "failed to load preset "+ofToString(filename));
        return;
    }
    xml.setTo("LearnPreset");
    loadPresetProcedure(xml);
}

//-------
void Learn::loadPresetProcedure(ofXml &xml) {
    
   // customInputs.clear();
    
    
    clearOutputs();
    clearInputs();
    loadInputs(xml);
    loadOutputs(xml);
    
    
    
    
    
    
    
    
    /*
    
    
    
    customInputs.clear();
    for (int i=0; i<inputs.size(); i++) {
        string name = inputs[i]->getName();
        if (inputFeatures.count(name) == 0) { // false??
            customInputs.push_back(name);
//            guiInputs->addLabelToggle(name, true, 140.0f);
            //            guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
            //            guiInputs->addLabelToggle("weighted centroid velocity", &vWCentroid, 50.0f)->setLabelText("velocity");
            //            guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
            
        }
    }
    
    */
    
    

    predicting = true;
}

//-------
void Learn::loadInputs(ofXml &xml) {
    customInputFeatures.clear();
    
    
    
    // Custom Inputs
    /*
    if (xml.exists("CustomInputs")) {
        xml.setTo("CustomInputs");
        if (xml.exists("CustomInput")) {
            xml.setTo("CustomInput[0]");
            do {
                string name = xml.getValue<string>("Name");
                CustomFeatureSet cfs;
                cfs.name = name;
                if (xml.exists("Parameter")) {
                    xml.setTo("Parameter[0]");
                    do {
                        string name = xml.getValue<string>("Name");
                        if (individualFeatures.count(name)==0) {
                            ofLog(OF_LOG_ERROR, "Error: feature "+name+" not found");
                        }
                        else {
                            cfs.features.push_back(individualFeatures[name]);
                        }
                    }
                    while (xml.setToSibling());
                    xml.setToParent();
                }
                customFeatureSets[cfs.name] = cfs;
                if (cfs.features.size() > 0) {
                    inputFeatures[cfs.name] = cfs.features;
                }
            }
            while (xml.setToSibling());
            xml.setToParent();
        }
        xml.setToParent();
    }
     */
    
    // Inputs
    xml.setTo("Inputs");
    if (xml.exists("InputGroup")) {
        xml.setTo("InputGroup[0]");
        do {
            string groupName = xml.getValue<string>("Name");
            if (xml.exists("Parameter")) {
                LearnInputGroup *newGroup = addInputGroup(groupName);
                xml.setTo("Parameter[0]");
                do {
                    string name = xml.getValue<string>("Name");
                    string oscAddress = xml.getValue<string>("OscAddress");
                    string type = xml.getValue<string>("Type");
                    float value = xml.getValue<float>("Value");
                    float min = xml.getValue<float>("Min");
                    float max = xml.getValue<float>("Max");
                    float warp = xml.getValue<float>("Warp");
                                            
                    // input to load settings into
                    LearnInputParameter * newInput = addInputToGroup(groupName, name, min, max);
                    newInput->setOscAddress(oscAddress);
                    newInput->setMin(min);
                    newInput->setMax(max);
                    newInput->setWarp(warp);
                    newInput->set(value);
                }
                while (xml.setToSibling());
                xml.setToParent();
                
                
                
                
                
                if (inputFeatures.count(groupName) == 0) {
                    customInputFeatures.push_back(groupName);
                }
                
                
                
            }
        }
        while (xml.setToSibling());
        xml.setToParent();
    }
    xml.setToParent();
}

//-------
void Learn::loadOutputs(ofXml &xml, bool loadExamples, bool loadClassifier) {
    xml.setTo("Outputs");
    if (xml.exists("Parameter")) {
        xml.setTo("Parameter[0]");
        do {
            string name = xml.getValue<string>("Name");
            string oscAddress = xml.getValue<string>("OscAddress");
            string type = xml.getValue<string>("Type");
            float value = xml.getValue<float>("Value");
            float min = xml.getValue<float>("Min");
            float max = xml.getValue<float>("Max");
            float warp = xml.getValue<float>("Warp");

            LearnOutputParameter * newOutput = addOutput(name, min, max);
            newOutput->setOscAddress(oscAddress);
            newOutput->setMin(min);
            newOutput->setMax(max);
            newOutput->setWarp(warp);
            newOutput->set(value);
            newOutput->setInputParameters(inputs);

            // plug inputs into output
            bool allInputsFound = true;
            xml.setTo("Inputs");
            if (xml.exists("Input[0]")) {
                xml.setTo("Input[0]");
                do {
                    string inputName = xml.getValue();
                    bool foundInput = false;
                    for (int i=0; i<inputs.size(); i++) {
                        if (inputs[i]->getName() == inputName) {
                            newOutput->addInput(inputs[i]);
                            foundInput = true;
                            break;
                        }
                    }
                    if (!foundInput) allInputsFound = false;
                }
                while (xml.setToSibling());
                xml.setToParent();
            }
            xml.setToParent();

            // add saved examples to output
            if (allInputsFound) {
                if (loadExamples) {
                    xml.setTo("Examples");
                    if (xml.exists("Page[0]")) {
                        xml.setTo("Page[0]");
                        do {
                            if (newOutput->getNumInstances() > 0) {
                                newOutput->addDataPage();
                            }
                            if (xml.exists("Example[0]")) {
                                xml.setTo("Example[0]");
                                do {
                                    vector<string> instanceStr = ofSplitString(xml.getValue(), ",");
                                    vector<float> instance;
                                    for (int i=0; i<instanceStr.size(); i++) {
                                        instance.push_back(ofToFloat(instanceStr[i]));
                                    }
                                    newOutput->addInstance(instance);
                                }
                                while (xml.setToSibling());
                                xml.setToParent();
                            }
                        }
                        while (xml.setToSibling());
                        xml.setToParent();
                    }
                    xml.setToParent();
                }

                if (loadClassifier) {
                    if (xml.exists("Classifier")) {
                        xml.setTo("Classifier");
                        string type = xml.getValue<string>("Type");
                        if (type == "SVM") {
                            string classifierPath = xml.getValue<string>("Path");
                            newOutput->loadClassifierSvm(classifierPath);
                        }
                        else if (type == "MLP") {
                            newOutput->setMlpNumHiddenLayers(xml.getValue<int>("NumHiddenLayers"));
                            newOutput->setMlpMaxSamples(xml.getValue<int>("MaxNumSamples"));
                            newOutput->setMlpTargetRmse(xml.getValue<float>("TargetRmse"));
                            vector<string> w1s = ofSplitString(xml.getValue<string>("W1"), ",");
                            vector<string> w3s = ofSplitString(xml.getValue<string>("W3"), ",");
                            vector<double> w1, w3;
                            for (int i=0; i<w1s.size(); i++)  w1.push_back(ofToDouble(w1s[i]));
                            for (int i=0; i<w3s.size(); i++)  w3.push_back(ofToDouble(w3s[i]));
                            newOutput->loadClassifierMlp(w1, w3);
                        }
                        xml.setToParent();
                    }
                }
            }
            else {
                ofLog(OF_LOG_ERROR, "Error for output "+newOutput->getName()+" : not all inputs found, so skip loading examples.");
            }
        }
        while (xml.setToSibling());
        xml.setToParent();
    }
    xml.setToParent();
}

//-------
void Learn::resetPresets() {
    vector<string> presets;
    ofDirectory dir(ofToDataPath("presets/"));
    dir.allowExt("xml");
    dir.listDir();
    for(int i = 0; i < dir.numFiles(); i++) {
        presets.push_back(dir.getName(i));
    }
    presets.push_back("...load from file...");
    guiSelector->clearToggles();
    guiSelector->addToggles(presets);
}




//===========================================
//  STYLE

//-------
void Learn::setFont(string path) {
    customFont = true;
    this->fontPath = path;
    for (int i=0; i<inputs.size(); i++) {
        vector<LearnInputParameter *> params = inputs[i]->getInputs();
        for (int j=0; j<params.size(); j++) {
            params[j]->setFont(fontPath);
        }
    }
    for (int i=0; i<outputs.size(); i++) {
        outputs[i]->setFont(fontPath);
    }
}

//-------
void Learn::setFontSizes(int small, int medium, int large) {
    this->fontSmall = small;
    this->fontMedium = medium;
    this->fontLarge = large;
    
    gui1->setFontSize(OFX_UI_FONT_SMALL, small);
    gui1->setFontSize(OFX_UI_FONT_MEDIUM, medium);
    gui1->setFontSize(OFX_UI_FONT_LARGE, large);
    
    gui2->setFontSize(OFX_UI_FONT_SMALL, small);
    gui2->setFontSize(OFX_UI_FONT_MEDIUM, medium);
    gui2->setFontSize(OFX_UI_FONT_LARGE, large);
    
    gui3->setFontSize(OFX_UI_FONT_SMALL, small);
    gui3->setFontSize(OFX_UI_FONT_MEDIUM, medium);
    gui3->setFontSize(OFX_UI_FONT_LARGE, large);
    
    for (int i=0; i<inputs.size(); i++) {
        vector<LearnInputParameter *> params = inputs[i]->getInputs();
        for (int j=0; j<params.size(); j++) {
            params[j]->setFontSizes(fontSmall, fontMedium, fontLarge);
        }
    }
    for (int i=0; i<outputs.size(); i++) {
        outputs[i]->setFontSizes(fontSmall, fontMedium, fontLarge);
    }
}


//-------
Learn::~Learn() {
    setGuiSummaryView(false);
    for (int i=0; i<inputs.size(); i++) {
        delete inputs[i];
    }
    for (int i=0; i<outputs.size(); i++) {
        delete outputs[i];
    }
    inputs.clear();
    outputs.clear();
}