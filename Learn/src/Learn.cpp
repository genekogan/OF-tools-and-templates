#include "Learn.h"


//-------
void Learn::setup(){
    
    trainDuration = 2;
    trainCountdown = 2;
    inputsVisible = true;

    guiI = new ofxUIScrollableCanvas(5, 144, 600, 400);
    guiI->setDrawOutlineHighLight(true);
    guiI->setScrollableDirections(false, true);
    guiI->setFont("AndaleMono.ttf");
    guiI->setVisible(inputsVisible);
//    ofAddListener(gui->newGUIEvent, this, &Inputs::guiEvent);

    guiO = new ofxUIScrollableCanvas(5, 144, 600, 400);
    guiO->setDrawOutlineHighLight(true);
    guiO->setScrollableDirections(false, true);
    guiO->setFont("AndaleMono.ttf");
    guiO->setVisible(!inputsVisible);
//    ofAddListener(gui->newGUIEvent, this, &Outputs::guiEvent);

    buildGuiMenu();
}

void Learn::addInput(ofxParameter<float> *parameter) {
    InputParameter *newInput = new InputParameter(parameter);
    inputs.push_back(newInput);
    buildGuiInputs();
}
void Learn::addInput(string name, float min, float max) {
    InputParameter *newInput = new InputParameter(name, min, max);
    inputs.push_back(newInput);
    buildGuiInputs();
}

void Learn::addOutput(ofxParameter<float> *parameter) {
    OutputParameter *newOutput = new OutputParameter(parameter);
    outputs.push_back(newOutput);
    buildGuiOutputs();
}
void Learn::addOutput(string name, float min, float max) {
    OutputParameter *newOutput = new OutputParameter(name, min, max);
    outputs.push_back(newOutput);
    buildGuiOutputs();
}



//-------
void Learn::update(){
}

//-------
void Learn::draw(){
}

//-------
void Learn::buildGuiInputs() {
    guiI->clearWidgets();
    guiI->addLabel("Inputs");
    guiI->addSpacer(650, 2)->setPadding(12);
    
    for (int i=0; i<inputs.size(); i++) {
        guiI->addSlider(inputs[i]->getName(),
                        inputs[i]->getMin(),
                        inputs[i]->getMax(),
                        &inputs[i]->getValueRef(),
                        150.0f, 10.0f)->setLabelVisible(true);
        guiI->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
        guiI->addLabel("min");
        guiI->addTextInput("min", ofToString(inputs[i]->getMin()), 45.0f, 20.0f)->setAutoClear(false);
        guiI->addLabel("max");
        guiI->addTextInput("max", ofToString(inputs[i]->getMax()), 45.0f, 20.0f)->setAutoClear(false);

        if (inputs[i]->getType() == PARAMETER) {
            // nothing
        }
        else if (inputs[i]->getType() == OSC) {
            guiI->addLabel("osc");
            guiI->addTextInput("oscAddress", inputs[i]->getAddress(), 110.0f, 20.0f)->setAutoClear(false);
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
        guiI->addSpacer(420, 1)->setPadding(8);
    }
    guiI->autoSizeToFitWidgets();
}

//-------
void Learn::buildGuiOutputs() {
    guiO->clearWidgets();
    guiO->addLabel("Outputs");
    guiO->addSpacer(650, 2)->setPadding(20);
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
            inputLabels.push_back(inputs[i]->getAddress());
        }
        guiO->addDropDownList("inputs", inputLabels, 150.0f)->setAllowMultiple(true);
        guiO->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
        
        guiO->addTextInput("numExamples"+ofToString(i), ofToString(outputs[i]->getNumExamples())+" examples", 100.0f);
        guiO->addButton("clear", false);
        guiO->addToggle("record", &outputs[i]->getRecordingRef());
        
        guiO->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
        guiO->addSpacer(600, 1)->setPadding(8);
    }
    guiO->autoSizeToFitWidgets();
}

//-------
void Learn::buildGuiMenu() {
    guiTrain = new ofxUICanvas(5, 5, 360, 130);
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
    guiTrain->addWidgetEastOf(new ofxUISlider("countdown", 0.0, 5.0, &trainCountdown, 120.0f, 16.0f), "accurate");
    guiTrain->addWidgetSouthOf(new ofxUISlider("duration", 0.0, 5.0, &trainDuration, 120.0f, 16.0f), "countdown");
    
    guiPresets = new ofxUICanvas(370, 5, 340, 130);
    guiPresets->setDrawOutlineHighLight(true);
    guiPresets->setFont("AndaleMono.ttf");
    guiPresets->setGlobalButtonDimension(56);
    guiPresets->setPadding(6);
    guiPresets->addLabel("Menu")->setColorFill(ofxUIColor(50, 200, 50));
    guiPresets->addSpacer();
    guiPresets->addToggle("view", false)->setLabelPosition(OFX_UI_WIDGET_POSITION_DOWN);
    guiPresets->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiPresets->addToggle("predict", false)->setLabelPosition(OFX_UI_WIDGET_POSITION_DOWN);
    guiPresets->addButton("touchOsc", false)->setLabelPosition(OFX_UI_WIDGET_POSITION_DOWN);
    guiPresets->addButton("save", false)->setLabelPosition(OFX_UI_WIDGET_POSITION_DOWN);
    guiPresets->addButton("load", false)->setLabelPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    //    ofAddListener(guiParameters->newGUIEvent, this, &ofApp::guiParametersEvent);
    ofAddListener(guiTrain->newGUIEvent, this, &Learn::guiTrainEvent);
    ofAddListener(guiPresets->newGUIEvent, this, &Learn::guiPresetsEvent);
}

//-------
void Learn::guiTrainEvent(ofxUIEventArgs &e) {
    if (e.getName() == "record") {
        cout << " rec "<< endl;
        //outputs.setRecording(e.getButton()->getValue() == 1);
    }
    else if (e.getName() == "fast") {
        if (e.getButton()->getValue() == 1) return;
        //outputs.trainClassifiers(FAST);
        //outputs.setPredicting(true);
        //((ofxUIToggle *) guiPresets->getWidget("predict"))->setValue(true);
    }
    else if (e.getName() == "accurate") {
        if (e.getButton()->getValue() == 1) return;
        //outputs.trainClassifiers(ACCURATE);
        //outputs.setPredicting(true);
        //((ofxUIToggle *) guiPresets->getWidget("predict"))->setValue(true);
    }
}

//-------
void Learn::guiPresetsEvent(ofxUIEventArgs &e) {
    if (e.getName() == "view") {
        inputsVisible = !inputsVisible;
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
        //outputs.setPredicting(!outputs.getPredicting());
    }
    else if (e.getName() == "load") {
        if (e.getButton()->getValue() == 1) return;
        ofFileDialogResult loadFile = ofSystemLoadDialog("load presets file");
        if (loadFile.bSuccess){
            //presets.load(loadFile.filePath);
        }
    }
}

