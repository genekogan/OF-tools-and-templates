#include "Learn.h"


//-------
Learn::Learn() {
    trainDuration = 2;
    trainCountdown = 2;
    instanceRate = 15;
    startTime = -(trainDuration + trainCountdown);
    oscActive = false;
}

//-------
void Learn::update(){

    /* send & receive osc */
    if (oscActive)
        oscManager.update();
    
    /* recording procedure */
    if (countingDown) {
        float elapsed = ofGetElapsedTimef() - startTime;
        if (elapsed > trainCountdown) {
            countingDown = false;
            recording = true;
            gui2->setColorBack(ofColor(255, 0, 0));
        }
    }
    else if (recording) {
        float elapsed = ofGetElapsedTimef() - trainCountdown - startTime;
        if (elapsed < trainDuration) {
            if (ofGetFrameNum() % framesPerInstance == 0) {
                recordInstance();
            }
        }
        else {
            stopRecording();
        }
    }
    
    /* prediction procedure */
    else if (predicting) {
        for (int i=0; i<outputs.size(); i++) {
            if (outputs[i]->getTrained())
                outputs[i]->predict();
        }
    }
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
        ofSetColor(0, 0, 255);
        ofRect(900, 5, 100, 60);
        ofSetColor(255);
        ofDrawBitmapString(ofToString(ceil(trainDuration - elapsed)), 945, 28);
    }
    else if (recording) {
        float elapsed = ofGetElapsedTimef() - startTime;
        ofSetColor(255, 0, 0);
        ofRect(900, 5, 100, 60);
        ofSetColor(255);
        ofDrawBitmapString(ofToString(ceil(trainDuration + trainCountdown - elapsed)), 945, 28);
    }
}

//-------
void Learn::addInput(string name, float *value, float min, float max) {
    LearnInputParameter *newInput = new LearnInputParameter(name, value, min, max);
    newInput->setGuiPosition(10, 80+55*inputs.size());
    newInput->setVisible(true);
    newInput->addParameterChangedListener(this, &Learn::inputParameterChanged);
    newInput->addParameterDeletedListener(this, &Learn::inputParameterDeleted);
    newInput->addParameterSelectedListener(this, &Learn::parameterSelected);
    inputs.push_back(newInput);
    resetInputs();
}

//-------
void Learn::addOutput(string name, float *value, float min, float max) {
    LearnOutputParameter *newOutput = new LearnOutputParameter(name, value, min, max);
    newOutput->setGuiPosition(420, 80+55*outputs.size());
    newOutput->setInputParameters(inputs);
    newOutput->setVisible(true);
    newOutput->addParameterChangedListener(this, &Learn::outputParameterChanged);
    newOutput->addParameterDeletedListener(this, &Learn::outputParameterDeleted);
    newOutput->addParameterSelectedListener(this, &Learn::parameterSelected);
    newOutput->addParameterViewedListener(this, &Learn::outputParameterViewed);
    outputs.push_back(newOutput);
    if (oscManager.getSending()) {
        oscManager.clearOutputTrackers();
        setupOscOutputs();
    }
}

//-------
void Learn::addInput(string name, float min, float max) {
    addInput(name, new float(), min, max);
}

//-------
void Learn::addOutput(string name, float min, float max) {
    addOutput(name, new float(), min, max);
}

//-------
void Learn::setupTouchOsc() {
    vector<ParameterBase *> parameters;
    for (int i=0; i<inputs.size(); i++) {
        parameters.push_back(inputs[i]);
    }
    oscManager.saveTouchOscLayout("learn", parameters);
}

//-------
void Learn::setupOscInputs(int port) {
    if (port != -1)
        this->oscInputPort = port;
    vector<ParameterBase *> parameters;
    for (int i=0; i<inputs.size(); i++) {
        parameters.push_back(inputs[i]);
    }
    if (!oscManager.getReceiving())
        oscManager.setupReceiver(oscInputPort);
    oscManager.clearInputTrackers();
    oscManager.registerToOscReceiver(parameters);
    oscActive = true;
}

//-------
void Learn::setupOscOutputs(string host, int port) {
    if (host != "")
        this->oscOutputHost = host;
    if (port != -1)
        this->oscOutputPort = port;
    vector<ParameterBase *> parameters;
    for (int i=0; i<outputs.size(); i++) {
        parameters.push_back(outputs[i]);
    }
    if (!oscManager.getSending())
        oscManager.setupSender(oscOutputHost, oscOutputPort);
    oscManager.clearOutputTrackers();
    oscManager.registerToOscSender(parameters);
    oscActive = true;
}

//-------
void Learn::setVisible(bool visible) {
    this->visible = visible;
    for (int i=0; i<inputs.size(); i++) {
        inputs[i]->setVisible(visible);
    }
    for (int i=0; i<outputs.size(); i++) {
        outputs[i]->setVisible(visible);
    }
    gui1->setVisible(visible);
    gui2->setVisible(visible);
    gui3->setVisible(visible);
}

//-------
void Learn::setGuiInputsVisible(bool visible) {
    for (int i=0; i<inputs.size(); i++) {
        inputs[i]->setVisible(visible);
    }
}

//-------
void Learn::setGuiOutputsVisible(bool visible) {
    for (int i=0; i<outputs.size(); i++) {
        outputs[i]->setVisible(visible);
    }
}

//-------
void Learn::startRecording() {
    startTime = ofGetElapsedTimef();
    framesPerInstance = ofGetFrameRate() / (float) instanceRate;
    countingDown = true;
    gui2->setColorBack(ofColor(0, 0, 255));
}

//-------
void Learn::stopRecording() {
    countingDown = false;
    recording = false;
    inRecording = false;
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
    for (int i=0; i<outputs.size(); i++) {
        if (outputs[i]->getNumInstances() == 0)  continue;
        if (trainStrategy == "fast") {
            outputs[i]->trainClassifierFast();
        }
        else if (trainStrategy == "accurate") {
            outputs[i]->trainClassifierAccurate();
        }
    }
    countingDown = false;
    recording = false;
    predicting = true;
}

//-------
void Learn::setupGui() {
    gui1 = new ofxUICanvas("setup");
    gui1->clearWidgets();
    gui1->setPosition(5, 5);
    gui1->setWidth(110);
    gui1->setHeight(60);
    gui1->addLabelButton("add input", false, 100, 22);
    gui1->addWidgetSouthOf(new ofxUILabelButton("add output", false,  100, 22, 0, 0, OFX_UI_FONT_SMALL), "add input")->setPadding(2);
    
    gui2 = new ofxUICanvas("train");
    gui2->clearWidgets();
    gui2->setPosition(130, 5);
    gui2->setWidth(450);
    gui2->setHeight(60);
    gui2->addLabelToggle("record", &inRecording, 100, 50);
    gui2->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui2->addLabelButton("train fast", false, 100, 22);
    gui2->addWidgetSouthOf(new ofxUILabelButton("train accurate", false,  100, 22, 0, 0, OFX_UI_FONT_SMALL), "train fast")->setPadding(2);
    gui2->addWidgetEastOf(new ofxUISlider("countdown", 0.0, 5.0, &trainCountdown, 100.0f, 9.0f), "train fast")->setPadding(2);
    gui2->addWidgetSouthOf(new ofxUISlider("duration", 0.0, 5.0, &trainDuration, 100.0f, 9.0f), "countdown")->setPadding(2);
    gui2->addWidgetEastOf(new ofxUIIntSlider("instanceRate", 1, 30, &instanceRate, 100.0f, 9.0f), "countdown")->setPadding(2);
    
    gui3 = new ofxUICanvas("perform");
    gui3->clearWidgets();
    gui3->setPosition(595, 5);
    gui3->setWidth(220);
    gui3->setHeight(60);
    gui3->addLabelToggle("predict", false, 100, 50);
    gui3->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui3->addLabelButton("save", false, 100, 22);
    gui3->addWidgetSouthOf(new ofxUILabelButton("load", false,  100, 22, 0, 0, OFX_UI_FONT_SMALL), "save")->setPadding(2);
    
    ofAddListener(gui1->newGUIEvent, this, &Learn::gui1Event);
    ofAddListener(gui2->newGUIEvent, this, &Learn::gui2Event);
    ofAddListener(gui3->newGUIEvent, this, &Learn::gui3Event);
}

//-------
void Learn::gui1Event(ofxUIEventArgs &e) {
    if (e.getName() == "add input") {
        if (e.getButton()->getValue() == 1) return;
        addInput("newInput"+ofToString(newInputCounter++), 0, 1);
    }
    else if (e.getName() == "add output") {
        if (e.getButton()->getValue() == 1) return;
        addOutput("newOutput"+ofToString(newOutputCounter++), new float(), 0, 1);
    }
}

//-------
void Learn::gui2Event(ofxUIEventArgs &e) {
    if (e.getName() == "record") {
        if (e.getButton()->getValue() == 1) {
            startRecording();
        }
        else {
            stopRecording();
        }
    }
    else if (e.getName() == "train fast") {
        if (e.getButton()->getValue() == 1) return;
        trainClassifiers("fast");
    }
    else if (e.getName() == "train accurate") {
        if (e.getButton()->getValue() == 1) return;
        trainClassifiers("accurate");
    }
}

//-------
void Learn::gui3Event(ofxUIEventArgs &e) {
    if (e.getName() == "predict") {
        predicting = e.getToggle()->getValue();
    }
    else if (e.getName() == "save") {
        if (e.getButton()->getValue() == 1) return;
        cout << "save"<<endl;
        /*
         
         SAVE PRESET 
         
         */
    }
    else if (e.getName() == "load") {
        if (e.getButton()->getValue() == 1) return;
        cout << "load"<<endl;
        /*
         
         LOAD PRESET
         
         */
    }
}

//-------
void Learn::resetInputs() {
    for (int i=0; i<outputs.size(); i++) {
        outputs[i]->setInputParameters(inputs);
    }
    setupOscInputs();
    for (int i=0; i<inputs.size(); i++) {
        inputs[i]->setGuiPosition(10, 80+55*i);
    }
    for (int i=0; i<outputs.size(); i++) {
        outputs[i]->setGuiPosition(420, 80+55*i);
    }
}

//-------
void Learn::inputParameterChanged(LearnParameter & input) {
    resetInputs();
}

//-------
void Learn::outputParameterChanged(LearnParameter & input) {
    setupOscOutputs();
}

//-------
void Learn::inputParameterDeleted(LearnParameter & input) {
    for (int i=0; i<inputs.size(); i++) {
        if (&input == inputs[i]) {
            delete inputs[i];
            inputs.erase(inputs.begin() + i);
            resetInputs();
            return;
        }
    }
}

//-------
void Learn::outputParameterDeleted(LearnParameter & output) {
    for (int i=0; i<outputs.size(); i++) {
        if (&output == outputs[i]) {
            delete outputs[i];
            outputs.erase(outputs.begin() + i);
            resetInputs();
            setupOscOutputs();
            return;
        }
    }
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
        if (inputs[i] == &parameter)   continue;
        inputs[i]->deselect();
    }
    for (int i=0; i<outputs.size(); i++) {
        if (outputs[i] == &parameter)   continue;
        outputs[i]->deselect();
    }
}

