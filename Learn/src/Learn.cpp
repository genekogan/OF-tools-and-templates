#include "Learn.h"


//-------
void Learn::setup(){

    guiI = new ofxUIScrollableCanvas(5, 145, 400, 500);
    guiI->setDrawOutlineHighLight(true);
    guiI->setScrollableDirections(false, true);
    guiI->setFont("AndaleMono.ttf");
//    ofAddListener(gui->newGUIEvent, this, &Inputs::guiEvent);

    guiO = new ofxUIScrollableCanvas(320, 145, 720, 500);
    guiO->setDrawOutlineHighLight(true);
    guiO->setScrollableDirections(false, true);
    guiO->setFont("AndaleMono.ttf");
    guiO->setVisible(false);
//    ofAddListener(gui->newGUIEvent, this, &Outputs::guiEvent);

    
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
    guiI->addSpacer(280, 2)->setPadding(12);
    
    for (int i=0; i<inputs.size(); i++) {
        guiI->addSlider(inputs[i]->getName(),
                        inputs[i]->getMin(),
                        inputs[i]->getMax(),
                        &inputs[i]->getValueRef(),
                        169.0f, 10.0f)->setLabelVisible(true);
        guiI->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
        guiI->addLabel("min");
        guiI->addTextInput("min", ofToString(inputs[i]->getMin()), 50.0f, 20.0f)->setAutoClear(false);
        guiI->addLabel("max");
        guiI->addTextInput("max", ofToString(inputs[i]->getMax()), 50.0f, 20.0f)->setAutoClear(false);

        if (inputs[i]->getType() == PARAMETER) {
        }
        else if (inputs[i]->getType() == OSC) {
            guiI->addLabel("osc");
            guiI->addTextInput("oscAddress", inputs[i]->getAddress(), 100.0f, 20.0f)->setAutoClear(false);
        }
        else if (inputs[i]->getType() == MIDI) {
        }
        else if (inputs[i]->getType() == MANTA) {
        }
        else if (inputs[i]->getType() == KINECT) {
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
        guiO->addLabel("value");
        guiO->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
        guiO->addTextInput("valueText"+ofToString(i), ofToString(outputs[i]->getValue()), 70.0f)->setAutoClear(false);;
        guiO->addSlider("value"+ofToString(i),
                        outputs[i]->getMin(),
                        outputs[i]->getMax(),
                        &outputs[i]->getValueRef(),
                        220.0f, 17.0f)->setLabelVisible(false);
        guiO->addLabel("min");
        guiO->addTextInput("min"+ofToString(i), ofToString(outputs[i]->getMin()), 70.0f)->setAutoClear(false);
        guiO->addLabel("max");
        guiO->addTextInput("max"+ofToString(i), ofToString(outputs[i]->getMax()), 70.0f)->setAutoClear(false);
        guiO->addButton("del"+ofToString(i), false);
        
        guiO->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
        
        guiO->addLabel("osc");
        guiO->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
        guiO->addTextInput("osc"+ofToString(i), outputs[i]->getAddress(), 220.0f)->setAutoClear(false);
        
        
        vector<string> inputLabels;
        for (int i=0; i<inputs.size(); i++) {
            inputLabels.push_back(inputs[i]->getAddress());
        }
        guiO->addDropDownList("inputs", inputLabels, 150.0f)->setAllowMultiple(true);
        
        
        guiO->addTextInput("numExamples"+ofToString(i), ofToString(outputs[i]->getNumExamples())+" examples", 100.0f);
        guiO->addButton("clear"+ofToString(i), false);
        guiO->addToggle("record", &outputs[i]->getRecordingRef());
        guiO->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
        guiO->addSpacer(650, 1)->setPadding(8);
    }
//    guiO->addSpacer(1, 16*inputs->getParameters().size())->setColorFill(ofxUIColor(0, 0));
    guiO->autoSizeToFitWidgets();


}