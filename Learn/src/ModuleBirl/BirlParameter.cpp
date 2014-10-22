#include "BirlParameter.h"


//===========================================
//  CONSTRUCTOR, DESTRUCTOR, DRAW


//-----------
BirlOutputParameter::BirlOutputParameter(string name, float *val, float min, float max) : LearnOutputParameter(name, val, min, max) {
    guiPreview = new ofxUICanvas();
    guiPerform = new ofxUICanvas();
    setupGuiPreview();
    ofAddListener(gui->newGUIEvent, this, &BirlOutputParameter::guiEvent);
    ofAddListener(guiPreview->newGUIEvent, this, &BirlOutputParameter::guiPreviewEvent);
    ofAddListener(guiPerform->newGUIEvent, this, &BirlOutputParameter::guiPreviewEvent);
    setDataSize(795, 300);
    guiData->setPosition(220, 430);
    viewInputs = false;
    inputKeys = false;
    inputKeysDiscrete = false;
    inputPressure = false;
    inputEmbouchure = false;
}

//-----------
BirlOutputParameter::~BirlOutputParameter() {
    ofRemoveListener(gui->newGUIEvent, this, &BirlOutputParameter::guiEvent);
    ofRemoveListener(guiPreview->newGUIEvent, this, &BirlOutputParameter::guiPreviewEvent);
    ofRemoveListener(guiPerform->newGUIEvent, this, &BirlOutputParameter::guiPreviewEvent);
    guiPreview->clearWidgets();
    guiPreview->removeWidgets();
    guiPreview->disable();
    guiPreview->setVisible(false);
    guiPerform->clearWidgets();
    guiPerform->removeWidgets();
    guiPerform->disable();
    guiPerform->setVisible(false);
    gui->clearWidgets();
    gui->removeWidgets();
    gui->disable();
    gui->setVisible(false);
}

//-----------
void BirlOutputParameter::draw() {
    if (viewExamples && guiPreview->isVisible()) {
        data[page]->draw(220, 460);
    }
    if (guiPerform->isVisible()) {
        guiPerformValueText->setTextString(ofToString(guiValue->getValue()));
    }
    if (training) {
        if (ofGetFrameNum() % 5 == 0) {
            ofColor newColor = ofColor(ofRandom(255),ofRandom(255),ofRandom(255));
            gui->setColorBack(newColor);
            guiPreview->setColorBack(newColor);
            guiPerform->setColorBack(newColor);
        }
    }

}




//===========================================
//  GUI

//-----------
void BirlOutputParameter::setupGui() {
    gui->setColorOutline(ofColor(255,200));
    gui->setDrawOutline(true);
    gui->clearWidgets();
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addLabelButton("X", false, 15.0f);
    guiName = gui->addTextInput("name", getName(), 155.0f);
    guiName->setAutoClear(false);
    gui->addLabel("osc: ");
    guiOsc = gui->addTextInput("osc", getOscAddress(), 128.0f);
    guiOsc->setAutoClear(false);
    gui->addLabel("min:");
    guiMin = gui->addTextInput("min", ofToString(getMin()), 50.0f);
    gui->addLabel("max:");
    guiMax = gui->addTextInput("max", ofToString(getMax()), 50.0f);
    guiMin->setAutoClear(false);
    guiMax->setAutoClear(false);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    gui->addLabel("inputs:");
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiInputKeys = gui->addLabelToggle("keys", &inputKeys, 100.0f);
    guiInputKeysDiscrete = gui->addLabelToggle("discrete", &inputKeysDiscrete, 100.0f);
    guiInputPressure = gui->addLabelToggle("pressure", &inputPressure, 100.0f);
    guiInputEmbouchure = gui->addLabelToggle("embouchure", &inputEmbouchure, 100.0f);
    gui->autoSizeToFitWidgets();
}

//-----------
void BirlOutputParameter::setupGuiPreview() {
    guiPreview->setColorOutline(ofColor(255,200));
    guiPreview->setDrawOutline(true);
    guiPreview->clearWidgets();
    guiPreview->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiPreview->addLabel("osc:");
    guiPreviewOsc = guiPreview->addTextInput("osc", ofToString(getOscAddress()), 120.0f);
    guiPreviewOsc->setAutoClear(false);
    guiValue = guiPreview->addSlider("value", getMin(), getMax(), getReference(), 200.0f, 18.0f);
    guiValue->setLabelVisible(false);
    guiValueText = guiPreview->addTextInput("valueText", ofToString(get()), 70.0f);
    guiValueText->setAutoClear(false);
    guiExamples = guiPreview->addLabelToggle("examples", &viewExamples, 105.0f);
    guiExamples->setLabelText(ofToString(getNumInstances())+" examples");
    guiPreview->addLabelToggle("record", &record, 56.0f);
    guiPreview->autoSizeToFitWidgets();
    guiPerform->setColorOutline(ofColor(255,200));
    guiPerform->setDrawOutline(true);
    guiPerform->clearWidgets();
    guiPerform->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiPerform->addLabel("osc:");
    guiPerformOsc = guiPerform->addTextInput("oscPerform", ofToString(getOscAddress()), 150.0f);
    guiPerformOsc->setAutoClear(false);
    guiPerformValue = guiPerform->addSlider("value", getMin(), getMax(), getReference(), 280.0f, 18.0f);
    guiPerformValue->setLabelVisible(false);
    guiPerformValueText = guiPerform->addTextInput("valueText", ofToString(get()), 100.0f);
    guiPerformValueText->setAutoClear(false);
    guiPerform->autoSizeToFitWidgets();
}

//-----------
void BirlOutputParameter::guiEvent(ofxUIEventArgs &e) {
    if (e.getName() == "name") {
        guiPreviewOsc->setTextString(getOscAddress());
        guiPerformOsc->setTextString(getOscAddress());
    }
    else if (e.getName() == "osc") {
        setOscAddress(guiOsc->getTextString());
        guiPreviewOsc->setTextString(getOscAddress());
        guiPerformOsc->setTextString(getOscAddress());
    }
    else if (e.getName() == "keys" ||
             e.getName() == "discrete" ||
             e.getName() == "pressure" ||
             e.getName() == "embouchure") {
        if (getNumInstances() > 0) {
            if (!ofSystemChoiceDialog("This will erase all your entries. Are you sure you want to do this?")) {
                if      (e.getName() == "keys")       inputKeys = !e.getButton()->getValue();
                else if (e.getName() == "discrete")   inputKeysDiscrete = !e.getButton()->getValue();
                else if (e.getName() == "pressure")   inputPressure = !e.getButton()->getValue();
                else if (e.getName() == "embouchure") inputEmbouchure = !e.getButton()->getValue();
                return;
            }
            else {
                clearInstances();
            }
        }
        if (e.getName() == "keys" && e.getButton()->getValue())
            inputKeysDiscrete = false;
        else if (e.getName() == "discrete" && e.getButton()->getValue())
            inputKeys = false;
        setInputs();
    }
    else if (e.getName() == "min") {
        guiPreviewSetMinMax();
    }
    else if (e.getName() == "max") {
        guiPreviewSetMinMax();
    }
}

//-----------
void BirlOutputParameter::guiPreviewSetMinMax() {
    float currentValue = get();
    guiValue->setMaxAndMin(getMax(), getMin());
    guiPerformValue->setMaxAndMin(getMax(), getMin());
    guiValue->setValue(currentValue);
    guiPerformValue->setValue(currentValue);
}

//-----------
void BirlOutputParameter::guiPreviewEvent(ofxUIEventArgs &e) {
    if (e.getName() == "value") {
        ofNotifyEvent(pSelectedEvent, *this, this);
        guiValueText->setTextString(ofToString(*getReference()));
        guiPerformValueText->setTextString(ofToString(*getReference()));
    }
    else if (e.getName() == "valueText") {
        if (guiValueText->isFocused())
            ofNotifyEvent(pSelectedEvent, *this, this);
        float newValue = ofToFloat(guiValueText->getTextString());
        guiValue->setValue(newValue);

        guiPerformValue->setValue(newValue);
        
    }
    else if (e.getName() == "osc") {
        if (guiPreviewOsc->isFocused())
            ofNotifyEvent(pSelectedEvent, *this, this);
        setOscAddress(guiPreviewOsc->getTextString());
        guiOsc->setTextString(guiPreviewOsc->getTextString());
        guiPerformOsc->setTextString(guiPreviewOsc->getTextString());
        ofNotifyEvent(pChangedEvent, (LearnParameter &) *this, this);
    }
    else if (e.getName() == "oscPerform") {
        if (guiPerformOsc->isFocused())
            ofNotifyEvent(pSelectedEvent, *this, this);
        setOscAddress(guiPerformOsc->getTextString());
        guiOsc->setTextString(guiPerformOsc->getTextString());
        guiPreviewOsc->setTextString(guiPreviewOsc->getTextString());
        ofNotifyEvent(pChangedEvent, (LearnParameter &) *this, this);
    }
    else if (e.getName() == "record") {
        setRecording(record);
    }
    else if (e.getName() == "examples") {
        setExamplesVisible(guiExamples->getValue());
        ofNotifyEvent(pViewedEvent, *this, this);
    }
}


//===========================================
//  VISIBILITY

//-----------
void BirlOutputParameter::setVisible(bool visible) {
    LearnOutputParameter::setVisible(visible);
    guiPreview->setVisible(visible);
    guiPerform->setVisible(visible);
}

//-------
void BirlOutputParameter::setGuiPreviewPosition(int x, int y) {
    guiPreview->setPosition(x, y);
    guiPerform->setPosition(x, y);
}

//-------
void BirlOutputParameter::setPreviewMode(BirlMode mode) {
    gui->setVisible(mode == BIRL_INPUTS);
    guiPreview->setVisible(mode == BIRL_TRAIN);
    guiPerform->setVisible(mode == BIRL_PERFORM);
}

//-----------
void BirlOutputParameter::deselect() {
    LearnOutputParameter::deselect();
    guiPerformOsc->setFocus(false);
    guiPreviewOsc->setFocus(false);
}



//===========================================
//  LEARNING

//-----------
void BirlOutputParameter::setRecording(bool record) {
    LearnOutputParameter::setRecording(record);
    guiPreview->setColorBack(record ? ofColor(200, 0, 0, 200) : trained ? ofColor(0, 60, 0, 100) : ofColor(0, 100));
}

//-----------
void BirlOutputParameter::setInputs() {
    activeInputs.clear();
    for (int i=0; i<allInputs.size(); i++) {
        string inputType = ofSplitString(allInputs[i]->getName(), "_")[0];
        if (inputType == "key" && inputKeys)
            activeInputs.push_back(allInputs[i]);
        else if (inputType == "keyDiscrete" && inputKeysDiscrete)
            activeInputs.push_back(allInputs[i]);
        if (inputType == "pressure" && inputPressure)
            activeInputs.push_back(allInputs[i]);
        if (inputType == "embouchure" && inputEmbouchure)
            activeInputs.push_back(allInputs[i]);
    }
    setupHeaders();
}

//-----------
void BirlOutputParameter::setTrained(bool trained) {
    this->trained = trained;
    this->record = false;
    gui->setColorBack(trained ? ofColor(0,60,0,100) : ofColor(0,100));
    guiPreview->setColorBack(trained ? ofColor(0,60,0,100) : ofColor(0,100));
    guiPerform->setColorBack(trained ? ofColor(0,60,0,100) : ofColor(0,100));
}

//-----------
void BirlOutputParameter::predict() {
    vector<double> birlInstance;
    if (inputKeys || inputKeysDiscrete) {
        for (int i=0; i<KEYS_NUMBER; i++)
            birlInstance.push_back(allInputs[i]->get());
    }
    if (inputPressure) {
        for (int i=0; i<PRESSURE_NUMBER; i++)
            birlInstance.push_back(allInputs[2 * KEYS_NUMBER + i]->get());
    }
    if (inputEmbouchure) {
        for (int i=0; i<EMBOUCHURE_NUMBER; i++)
            birlInstance.push_back(allInputs[2 * KEYS_NUMBER + PRESSURE_NUMBER + i]->get());
    }
    LearnOutputParameter::predict(birlInstance);
}




//===========================================
//  STYLE

//-----------
void BirlOutputParameter::setFont(string path) {
    LearnOutputParameter::setFont(path);
    guiPreview->setFont(path);
    guiPerform->setFont(path);
}

//-----------
void BirlOutputParameter::setFontSizes(int small, int medium, int large) {
    LearnOutputParameter::setFontSizes(small, medium, large);
    guiPreview->setFontSize(OFX_UI_FONT_SMALL, small);
    guiPreview->setFontSize(OFX_UI_FONT_MEDIUM, medium);
    guiPreview->setFontSize(OFX_UI_FONT_LARGE, large);
    guiPerform->setFontSize(OFX_UI_FONT_SMALL, small);
    guiPerform->setFontSize(OFX_UI_FONT_MEDIUM, medium);
    guiPerform->setFontSize(OFX_UI_FONT_LARGE, large);
}