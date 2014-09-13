#include "Learn.h"


//-------
void Learn::setup(){
    trainDuration = 2;
    trainCountdown = 2;
    instanceRate = 15;
    inputsVisible = true;

    guiI = new ofxUIScrollableCanvas(5, 144, 500, 400);
    guiI->setDrawOutlineHighLight(true);
    guiI->setScrollableDirections(false, true);
    guiI->setFont("AndaleMono.ttf");
    guiI->setVisible(inputsVisible);
    ofAddListener(guiI->newGUIEvent, this, &Learn::guiInputsEvent);

    guiO = new ofxUIScrollableCanvas(5, 144, 500, 400);
    guiO->setDrawOutlineHighLight(true);
    guiO->setScrollableDirections(false, true);
    guiO->setFont("AndaleMono.ttf");
    guiO->setVisible(!inputsVisible);
    ofAddListener(guiO->newGUIEvent, this, &Learn::guiOutputsEvent);

    /* preview guis */
    guiIP = new ofxUIScrollableCanvas(5, 144, 305, 400);
    guiIP->setDrawOutlineHighLight(true);
    guiIP->setScrollableDirections(false, true);
    guiIP->setFont("AndaleMono.ttf");
    guiIP->setVisible(false);
    guiOP = new ofxUIScrollableCanvas(340, 144, 305, 400);
    guiOP->setDrawOutlineHighLight(true);
    guiOP->setScrollableDirections(false, true);
    guiOP->setFont("AndaleMono.ttf");
    guiOP->setVisible(false);

    buildGuiMenu();
    startTime = -(trainDuration + trainCountdown);
}

//-------
void Learn::addInput(ofxParameter<float> *parameter) {
    InputParameter *newInput = new InputParameter(parameter);
    inputs.push_back(newInput);
    buildGuiInputs();
}

//-------
void Learn::addInput(string name, float min, float max) {
    InputParameter *newInput = new InputParameter(name, min, max);
    inputs.push_back(newInput);
    buildGuiInputs();
}

//-------
void Learn::addOutput(ofxParameter<float> *parameter) {
    OutputParameter *newOutput = new OutputParameter(parameter);
    outputs.push_back(newOutput);
    buildGuiOutputs();
}

//-------
void Learn::addOutput(string name, float min, float max) {
    OutputParameter *newOutput = new OutputParameter(name, min, max);
    outputs.push_back(newOutput);
    buildGuiOutputs();
}

//-------
void Learn::update(){
    float elapsed = ofGetElapsedTimef() - startTime;
    if (elapsed < trainCountdown) {
    }
    else if (elapsed < trainCountdown + trainDuration) {
        if (ofGetFrameNum() % framesPerInstance == 0)
            recordInstance();
    }
    else {
        ((ofxUIToggle *) guiTrain->getWidget("record"))->setValue(false);
    }
    
    if (predicting) {
        for (int i=0; i<outputs.size(); i++) {
            if (outputs[i]->getTrained())
                outputs[i]->predict();
        }
    }
}

//-------
void Learn::draw(){
}

//-------
void Learn::buildGuiInputs() {
    guiI->clearWidgets();
    guiI->addLabel("Inputs");
    guiI->addSpacer(480, 2)->setPadding(12);
    for (int i=0; i<inputs.size(); i++) {
        guiI->addSlider(inputs[i]->getName(),
                        inputs[i]->getMin(),
                        inputs[i]->getMax(),
                        &inputs[i]->getValueRef(),
                        150.0f, 10.0f)->setLabelVisible(true);
        guiI->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
        guiI->addLabel("min");
        guiI->addTextInput("min_"+ofToString(i), ofToString(inputs[i]->getMin()), 45.0f, 20.0f)->setAutoClear(false);
        guiI->addLabel("max");
        guiI->addTextInput("max_"+ofToString(i), ofToString(inputs[i]->getMax()), 45.0f, 20.0f)->setAutoClear(false);

        if (inputs[i]->getType() == PARAMETER) {
            // nothing
        }
        else if (inputs[i]->getType() == OSC) {
            guiI->addLabel("osc");
            guiI->addTextInput("oscAddress_"+ofToString(i), inputs[i]->getAddress(), 110.0f, 20.0f)->setAutoClear(false);
        }
        else if (inputs[i]->getType() == MIDI) {
            // channel
        }
        else if (inputs[i]->getType() == MANTA) {
            // pad #
        }
        else if (inputs[i]->getType() == KINECT) {
            // joint selection
        }
        
        guiI->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
        guiI->addSpacer(480, 1)->setPadding(8);
    }
    guiI->autoSizeToFitWidgets();

    buildGuiPreview();
}

//-------
void Learn::buildGuiOutputs() {
    guiO->clearWidgets();
    guiO->addLabel("Outputs");
    guiO->addSpacer(500, 2)->setPadding(20);
    for (int i=0; i<outputs.size(); i++) {
        guiO->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
        guiO->addSlider(outputs[i]->getName(),
                        outputs[i]->getMin(),
                        outputs[i]->getMax(),
                        &outputs[i]->getValueRef(),
                        150.0f, 10.0f)->setLabelVisible(true);
        guiO->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
        guiO->addLabel("min");
        guiO->addTextInput("min"+ofToString(i), ofToString(outputs[i]->getMin()), 45.0f, 20.0f)->setAutoClear(false);
        guiO->addLabel("max");
        guiO->addTextInput("max"+ofToString(i), ofToString(outputs[i]->getMax()), 45.0f, 20.0f)->setAutoClear(false);

        if (outputs[i]->getType() == PARAMETER) {
        }
        else if (outputs[i]->getType() == OSC) {
            guiO->addLabel("osc");
            guiO->addTextInput("oscAddress", outputs[i]->getAddress(), 110.0f, 20.0f)->setAutoClear(false);
        }
        
        guiO->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
        vector<string> inputLabels;
        for (int i=0; i<inputs.size(); i++) {
            inputLabels.push_back(inputs[i]->getName());
        }
        guiO->addDropDownList("inputs_"+ofToString(i), inputLabels, 150.0f)->setAllowMultiple(true);;
        
        guiO->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
        guiO->addTextInput("numExamples"+ofToString(i), ofToString(outputs[i]->getNumExamples())+" examples", 100.0f);
        guiO->addButton("clear_"+ofToString(i), false);
        guiO->addToggle("record_"+ofToString(i), &outputs[i]->getRecordingRef());
        
        guiO->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
        guiO->addSpacer(500, 1)->setPadding(8);
    }
    guiO->addSpacer(0, inputs.size()*12)->setPadding(8);
    guiO->autoSizeToFitWidgets();
    
    buildGuiPreview();
}

//-------
void Learn::buildGuiPreview() {
    guiIP->clearWidgets();
    guiIP->addLabel("Inputs");
    guiIP->addSpacer(300, 2)->setPadding(12);
    for (int i=0; i<inputs.size(); i++) {
        guiIP->addSlider(inputs[i]->getName(),
                        inputs[i]->getMin(),
                        inputs[i]->getMax(),
                        &inputs[i]->getValueRef(),
                        300.0f, 12.0f)->setLabelVisible(true);
        guiIP->addSpacer(300, 1)->setPadding(8);
    }
    guiIP->autoSizeToFitWidgets();

    guiOP->clearWidgets();
    guiOP->addLabel("Outputs");
    guiOP->addSpacer(300, 2)->setPadding(12);
    for (int i=0; i<outputs.size(); i++) {
        guiOP->addSlider(outputs[i]->getName(),
                         outputs[i]->getMin(),
                         outputs[i]->getMax(),
                         &outputs[i]->getValueRef(),
                         300.0f, 12.0f)->setLabelVisible(true);
        guiOP->addSpacer(300, 1)->setPadding(8);
    }
    guiOP->autoSizeToFitWidgets();
}

//-------
void Learn::buildGuiMenu() {
    guiPresets = new ofxUICanvas(5, 5, 340, 135);
    guiPresets->setDrawOutlineHighLight(true);
    guiPresets->setFont("AndaleMono.ttf");
    guiPresets->setGlobalButtonDimension(56);
    guiPresets->setPadding(6);
    guiPresets->addLabel("Menu")->setColorFill(ofxUIColor(50, 200, 50));
    guiPresets->addSpacer();
    guiPresets->addToggle("view", &inputsVisible)->setLabelPosition(OFX_UI_WIDGET_POSITION_DOWN);
    guiPresets->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiPresets->addToggle("predict", &predicting)->setLabelPosition(OFX_UI_WIDGET_POSITION_DOWN);
    guiPresets->addButton("touchOsc", false)->setLabelPosition(OFX_UI_WIDGET_POSITION_DOWN);
    guiPresets->addButton("save", false)->setLabelPosition(OFX_UI_WIDGET_POSITION_DOWN);
    guiPresets->addButton("load", false)->setLabelPosition(OFX_UI_WIDGET_POSITION_DOWN);

    guiTrain = new ofxUICanvas(350, 5, 360, 135);
    guiTrain->setDrawOutlineHighLight(true);
    guiTrain->setFont("AndaleMono.ttf");
    guiTrain->setGlobalButtonDimension(56);
    guiTrain->setPadding(6);
    guiTrain->addLabel("Train")->setColorFill(ofxUIColor(50, 200, 50));
    guiTrain->addSpacer();
    guiTrain->addToggle("record", false)->setLabelPosition(OFX_UI_WIDGET_POSITION_DOWN);
    guiTrain->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiTrain->addButton("fast", false)->setLabelPosition(OFX_UI_WIDGET_POSITION_DOWN);
    guiTrain->addButton("accurate", false)->setLabelPosition(OFX_UI_WIDGET_POSITION_DOWN);
    guiTrain->addWidgetEastOf(new ofxUISlider("countdown", 0.0, 5.0, &trainCountdown, 120.0f, 8.0f), "accurate");
    guiTrain->addWidgetSouthOf(new ofxUISlider("duration", 0.0, 5.0, &trainDuration, 120.0f, 8.0f), "countdown");
    guiTrain->addWidgetSouthOf(new ofxUIIntSlider("instanceRate", 1, 30, &instanceRate, 120.0f, 8.0f), "duration");

    ofAddListener(guiPresets->newGUIEvent, this, &Learn::guiPresetsEvent);
    ofAddListener(guiTrain->newGUIEvent, this, &Learn::guiTrainEvent);
}

//-------
void Learn::startRecording() {
    startTime = ofGetElapsedTimef();
    framesPerInstance = ofGetFrameRate() / (float) instanceRate;
}

//-------
void Learn::recordInstance() {
    for (int i=0; i<outputs.size(); i++) {
        if (!outputs[i]->getRecording())    continue;
        outputs[i]->addTrainingInstance();
        ((ofxUITextInput *) guiO->getWidget("numExamples"+ofToString(i)))->setTextString(ofToString(outputs[i]->getNumExamples())+" examples");
    }
}

//-------
void Learn::setPredictingMode(bool predicting) {
    this->predicting = predicting;
    guiI->setVisible(!predicting);
    guiO->setVisible(!predicting);
    guiIP->setVisible(predicting);
    guiOP->setVisible(predicting);
    if (!predicting) {
        guiI->setVisible(inputsVisible);
        guiO->setVisible(!inputsVisible);
    }
}

//-------
void Learn::guiInputsEvent(ofxUIEventArgs &e){
    for (int i=0; i<outputs.size(); i++) {
        if (e.getName() == "min_"+ofToString(i)) {
            float min = ofToFloat(((ofxUITextInput *) guiI->getWidget("min_"+ofToString(i)))->getTextString());
            inputs[i]->setMin(min);
            ((ofxUISlider *) guiI->getWidget(inputs[i]->getName()))->setMin(min);
        }
        else if (e.getName() == "max_"+ofToString(i)) {
            float max = ofToFloat(((ofxUITextInput *) guiI->getWidget("max_"+ofToString(i)))->getTextString());
            inputs[i]->setMax(max);
            ((ofxUISlider *) guiI->getWidget(inputs[i]->getName()))->setMax(max);

        }
        else if (e.getName() == "oscAddress_"+ofToString(i)) {
            inputs[i]->setAddress(((ofxUITextInput *) guiI->getWidget("oscAddress_"+ofToString(i)))->getTextString());
        }
    }
}

//-------
void Learn::guiOutputsEvent(ofxUIEventArgs &e){
    for (int i=0; i<outputs.size(); i++) {
        if (e.getName() == "clear_"+ofToString(i)) {
            outputs[i]->clearTrainingExamples();
        }
        else if (e.getName() == "record_"+ofToString(i)) {
            // done
        }
        else if (e.getParentName() == "inputs_"+ofToString(i)) {
            outputs[i]->clearInputs();
            ofxUIDropDownList *dropDown = (ofxUIDropDownList *) guiO->getWidget("inputs_"+ofToString(i));
            vector<ofxUILabelToggle *> toggles = dropDown->getToggles();
            for (int j=0; j<toggles.size(); j++) {
                if (toggles[j]->getValue()) {
                    outputs[i]->addInput(inputs[j]);
                }
            }
        }
    }
}

//-------
void Learn::guiTrainEvent(ofxUIEventArgs &e) {
    if (e.getName() == "record") {
        startRecording();
    }
    else if (e.getName() == "fast" || e.getName() == "recording") {
        if (e.getButton()->getValue() == 1) return;
        for (int i=0; i<outputs.size(); i++) {
            if (outputs[i]->getNumExamples() == 0)  continue;
            if (e.getName() == "fast")
                outputs[i]->trainClassifierFast();
            else if (e.getName() == "accurate")
                outputs[i]->trainClassifierAccurate();
        }
        ((ofxUIToggle *) guiPresets->getWidget("predict"))->setValue(true);
        recording = false;
        predicting = true;
    }
}

//-------
void Learn::guiPresetsEvent(ofxUIEventArgs &e) {
    if (e.getName() == "view") {
        if (predicting) return;
        guiI->setVisible(inputsVisible);
        guiO->setVisible(!inputsVisible);
    }
    else if (e.getName() == "save") {
        if (e.getButton()->getValue() == 1) return;
        cout << "save " << endl;
    }
    else if (e.getName() == "touchOsc") {
        if (e.getButton()->getValue() == 1) return;
        //outputs.sendControlOSCLayout();
    }
    else if (e.getName() == "predict") {
        setPredictingMode(e.getButton()->getValue());
    }
    else if (e.getName() == "load") {
        if (e.getButton()->getValue() == 1) return;
        ofFileDialogResult loadFile = ofSystemLoadDialog("load presets file");
        if (loadFile.bSuccess){
            //presets.load(loadFile.filePath);
        }
    }
}

