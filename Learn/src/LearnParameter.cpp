#include "LearnParameter.h"


//===========================================
//  CONSTRUCTORS AND DESTRUCTORS

LearnParameter::LearnParameter(string name, float *value, float min, float max) : Parameter<float>(name, *value, min, max) {
    *value = 0.5 * (min + max);
    gui = new ofxUICanvas(name);
}

//-----------
LearnInputParameter::LearnInputParameter(string name, float *value, float min, float max) : LearnParameter(name, value, min, max) {
    setupGui();
    ofAddListener(gui->newGUIEvent, this, &LearnInputParameter::guiEvent);
}

//-----------
LearnOutputParameter::LearnOutputParameter(string name, float *value, float min, float max) : LearnParameter(name, value, min, max) {
    data.setup(600, 300);
    setupHeaders();
    guiInputSelect = new ofxUICanvas(name+"_inputs");
    setupGui();
    ofAddListener(gui->newGUIEvent, this, &LearnOutputParameter::guiEvent);
    ofAddListener(guiInputSelect->newGUIEvent, this, &LearnOutputParameter::guiInputSelectEvent);
    record = false;
    trained = false;
    viewExamples = false;
    viewInputs = false;
}

//-----------
LearnParameter::~LearnParameter() {
    gui->clearWidgets();
    gui->removeWidgets();
    gui->disable();
}

//-----------
LearnOutputParameter::~LearnOutputParameter() {
    ofRemoveListener(gui->newGUIEvent, this, &LearnOutputParameter::guiEvent);
    ofRemoveListener(guiInputSelect->newGUIEvent, this, &LearnOutputParameter::guiInputSelectEvent);
    guiInputSelect->clearWidgets();
    guiInputSelect->removeWidgets();
    guiInputSelect->disable();
    gui->clearWidgets();
    gui->removeWidgets();
    gui->disable();
}




//===========================================
//  LEARNING

void LearnOutputParameter::addInstance() {
    data.addEntry(grabFeatureVector<float>(true));
    guiExamples->setLabelText(ofToString(data.getNumberOfEntries())+" examples");
}

//-----------
void LearnOutputParameter::clearInstances() {
    data.clear();
    guiExamples->setLabelText(ofToString(data.getNumberOfEntries())+" examples");
}

//-----------
void LearnOutputParameter::setInputParameters(vector<LearnInputParameter *> &allInputs) {
    this->allInputs = allInputs;
    setupGuiInputSelector();
}

//-----------
void LearnOutputParameter::setupHeaders() {
    vector<string> headers;
    headers.push_back(name);
    for (int i=0; i<activeInputs.size(); i++) {
        headers.push_back(activeInputs[i]->getName());
    }
    data.setHeaders(headers);
}

//-----------
void LearnOutputParameter::addSpreadsheetDataToLearn() {
    learn.clearTrainingInstances();
    vector<vector<float> > entries = data.getEntries();
    for (int i=0; i<entries.size(); i++) {
        double normalizedLabel = (double) ofMap(entries[i][0], getMin(), getMax(), 0.0f, 1.0f);
        vector<double> instance;
        for (int j=1; j<entries[i].size(); j++) {
            instance.push_back((double) entries[i][j]);
        }
        learn.addTrainingInstance(instance, normalizedLabel);
    }
}

//-----------
void LearnOutputParameter::trainClassifierFast() {
    if (data.getNumberOfEntries() > 0) {
        addSpreadsheetDataToLearn();
        learn.trainRegression(FAST, REGRESSION_SVM);
        trained = true;
    }
}

//-----------
void LearnOutputParameter::trainClassifierAccurate() {
    if (data.getNumberOfEntries() > 0) {
        addSpreadsheetDataToLearn();
        learn.trainRegression(ACCURATE, REGRESSION_SVM);
        trained = true;
    }
}

//-----------
void LearnOutputParameter::predict() {
    double normalizedPrediction = learn.predict(grabFeatureVector<double>(false));
    set((float) ofMap(normalizedPrediction, 0.0, 1.0, getMin(), getMax()));
    guiValueText->setTextString(ofToString(guiValue->getValue()));
}

//-----------
void LearnOutputParameter::draw() {
    if (viewExamples) {
        data.draw(420, 460);
    }
}




//===========================================
//  GUI

void LearnInputParameter::setupGui() {
    gui->clearWidgets();
    gui->addLabelButton("X", false, 15.0f);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiName = gui->addTextInput("name", ofToString(getName()), 155.0f);
    guiName->setAutoClear(false);
    gui->addLabel("osc:");
    guiOsc = gui->addTextInput("osc", ofToString(getOscAddress()), 151.0f);
    guiOsc->setAutoClear(false);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    guiValue = gui->addSlider("value", getMin(), getMax(), getReference(), 120.0f, 18.0f);
    guiValue->setLabelVisible(false);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiValueText = gui->addTextInput("valueText", ofToString(get()), 50.0f);
    guiValueText->setAutoClear(false);
    gui->addLabel("min:");
    guiMin = gui->addTextInput("min", ofToString(getMin()), 52.0f);
    gui->addLabel("max:");
    guiMax = gui->addTextInput("max", ofToString(getMax()), 52.0f);
    guiMin->setAutoClear(false);
    guiMax->setAutoClear(false);
    gui->autoSizeToFitWidgets();
}

//-----------
void LearnOutputParameter::setupGui() {
    gui->clearWidgets();
    gui->setWidth(800);
    gui->addLabelButton("X", false, 15.0f);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiName = gui->addTextInput("name", ofToString(getName()), 155.0f);
    guiName->setAutoClear(false);
    gui->addLabel("osc:");
    guiOsc = gui->addTextInput("osc", ofToString(getOscAddress()), 128.0f);
    guiOsc->setAutoClear(false);
    guiInputs = gui->addLabelToggle("inputs", &viewInputs, 60.0f);
    guiExamples = gui->addLabelToggle("examples", &viewExamples, 105.0f);
    guiExamples->setLabelText(ofToString(data.getNumberOfEntries())+" examples");
    gui->addLabelToggle("record", &record, 56.0f);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    guiValue = gui->addSlider("value", getMin(), getMax(), getReference(), 250.0f, 18.0f);
    guiValue->setLabelVisible(false);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiValueText = gui->addTextInput("valueText", ofToString(get()), 80.0f);
    guiValueText->setAutoClear(false);
    gui->addLabel("min:");
    guiMin = gui->addTextInput("min", ofToString(getMin()), 80.0f);
    gui->addLabel("max:");
    guiMax = gui->addTextInput("max", ofToString(getMax()), 80.0f);
    guiMin->setAutoClear(false);
    guiMax->setAutoClear(false);
    gui->autoSizeToFitWidgets();
}

//-----------
void LearnOutputParameter::setupGuiInputSelector() {
    guiInputSelect->clearWidgets();
    vector<string> inputLabels;
    for (int i=0; i<allInputs.size(); i++) {
        inputLabels.push_back(allInputs[i]->getName());
    }
    selector = guiInputSelect->addDropDownList("select inputs", inputLabels, 200.0f);
    selector->setAutoClose(false);
    selector->setAllowMultiple(true);
    selector->open();
    vector<ofxUILabelToggle *> toggles = selector->getToggles();
    for (int i=0; i<toggles.size(); i++) {
        string inputName = toggles[i]->getName();
        for (int j=0; j<activeInputs.size(); j++) {
            toggles[i]->setValue(inputName == activeInputs[j]->getName());
        }
    }
    guiInputSelect->autoSizeToFitWidgets();
    guiInputSelect->setVisible(viewInputs);
    setupHeaders();
}

//-----------
void LearnParameter::guiEvent(ofxUIEventArgs &e) {
    if (e.getName() == "name") {
        string newName = guiName->getTextString();
        if (newName == getName())   return;
        setName(newName);
        setOscAddress("/"+guiName->getTextString());
        guiOsc->setTextString(getOscAddress());
        ofNotifyEvent(parameterChangeEvent, (LearnParameter &) *this, this);
    }
    else if (e.getName() == "osc") {
        string newOscAddress = guiOsc->getTextString();
        if (newOscAddress == getOscAddress())   return;
        setOscAddress(guiOsc->getTextString());
        ofNotifyEvent(parameterChangeEvent, (LearnParameter &) *this, this);
    }
    else if (e.getName() == "min") {
        float currentValue = *getReference();
        float newValue = ofToFloat(guiMin->getTextString());
        setMin(newValue);
        guiValue->setMin(newValue);
        set(currentValue);
    }
    else if (e.getName() == "max") {
        float currentValue = *getReference();
        float newValue = ofToFloat(guiMax->getTextString());
        setMax(newValue);
        guiValue->setMax(ofToFloat(guiMax->getTextString()));
        set(currentValue);
    }
    else if (e.getName() == "value") {
        guiValueText->setTextString(ofToString(guiValue->getValue()));
    }
    else if (e.getName() == "valueText") {
        guiValue->setValue(ofToFloat(guiValueText->getTextString()));
    }
}

//-----------
void LearnInputParameter::guiEvent(ofxUIEventArgs &e) {
    LearnParameter::guiEvent(e);
    if (e.getName() == "X") {
        if (e.getButton()->getValue() == 1) return;
        string name = getName();
        ofNotifyEvent(parameterDeletedEvent, (LearnParameter &) *this, this);
    }
}

//-----------
void LearnOutputParameter::guiEvent(ofxUIEventArgs &e) {
    if (e.getName() == "X") {
        if (e.getButton()->getValue() == 1) return;
        if (ofSystemChoiceDialog("Really delete "+getName()+"?")) {
            ofNotifyEvent(parameterDeletedEvent, (LearnParameter &) *this, this);
        }
    }
    else if (e.getName() == "record") {
        if (record && activeInputs.size()==0) {
            ofSystemAlertDialog("Can't record: no inputs selected.");
            record = false;
            return;
        }
        gui->setColorBack(record ? ofColor(200, 0, 0, 200) : ofColor(0, 100));
    }
    else if (e.getName() == "examples") {
        viewExamples = guiExamples->getValue();
        if (viewExamples) {
            viewInputs = false;
            guiInputSelect->setVisible(false);
        }
    }
    else if (e.getName() == "inputs") {
        guiInputSelect->setVisible(viewInputs);
        viewExamples = viewInputs ? false : viewExamples;
    }
    else if (e.getName() == "min" || e.getName() == "max") {
        if ((ofToFloat(guiMin->getTextString()) != getMin() ||
            ofToFloat(guiMax->getTextString()) != getMax()) &&
            data.getNumberOfEntries() > 0) {
            if (!ofSystemChoiceDialog("Warning: this will overwrite all recorded examples and trained classifiers. Proceed?")) {
                guiMin->setTextString(ofToString(getMin()));
                guiMax->setTextString(ofToString(getMax()));
                return;
            }
            else {
                data.clear();
            }
        }
    }
    LearnParameter::guiEvent(e);
}

//-----------
void LearnOutputParameter::guiInputSelectEvent(ofxUIEventArgs &e) {
    vector<ofxUILabelToggle *> toggles = selector->getToggles();
    activeInputs.clear();
    for (int i=0; i<toggles.size(); i++) {
        if (toggles[i]->getValue()) {
            activeInputs.push_back(allInputs[i]);
        }
    }
    setupHeaders();
}

//-----------
void LearnOutputParameter::setVisible(bool visible){
    LearnParameter::setVisible(visible);
    viewExamples = visible ? viewExamples : false;
    viewInputs = visible ? viewInputs : false;
    guiInputSelect->setVisible(viewInputs);
}

//-----------
void LearnOutputParameter::setGuiPosition(int x, int y) {
    gui->setPosition(x, y);
    guiInputSelect->setPosition(x, 460);
}


