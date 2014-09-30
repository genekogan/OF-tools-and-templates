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
    guiInputSelect = new ofxUICanvas(name+"_inputs");
    guiData = new ofxUICanvas(name+"_data");

    record = false;
    trained = false;
    viewExamples = false;
    viewInputs = false;

    setupGui();
    addDataPage();

    ofAddListener(gui->newGUIEvent, this, &LearnOutputParameter::guiEvent);
    ofAddListener(guiInputSelect->newGUIEvent, this, &LearnOutputParameter::guiInputSelectEvent);
    ofAddListener(guiData->newGUIEvent, this, &LearnOutputParameter::guiDataEvent);
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
    data[page]->addEntry(grabFeatureVector<float>(true));
    guiExamples->setLabelText(ofToString(getNumInstances())+" examples");
    guiDataStatus->setLabelText(ofToString(getName())+" examples ("+ofToString(getNumInstances())+")");
}

//-----------
void LearnOutputParameter::clearInstances() {
    for (int i=0; i<data.size(); i++) {
        data[i]->clear();
        delete data[i];
    }
    data.clear();
    addDataPage();
    setPage(0);
    guiExamples->setLabelText(ofToString(getNumInstances())+" examples");
    guiDataStatus->setLabelText(ofToString(getName())+" examples ("+ofToString(getNumInstances())+")");
}

//-----------
int LearnOutputParameter::getNumInstances() {
    int numInstances = 0;
    for (int i=0; i<data.size(); i++) {
        numInstances += data[i]->getNumberOfEntries();
    }
    return numInstances;
}

//-----------
void LearnOutputParameter::setInputParameters(vector<LearnInputParameter *> &allInputs) {
    this->allInputs = allInputs;
    setupGuiInputSelector();
}

//-----------
void LearnOutputParameter::addDataPage() {
    ofxSpreadsheet *newData = new ofxSpreadsheet();
    newData->setup(600, 300);
    data.push_back(newData);
    setupHeaders(data.size()-1);
    setPage(data.size()-1);
}

//-----------
void LearnOutputParameter::setupHeaders(int p) {
    vector<string> headers;
    headers.push_back(name);
    for (int i=0; i<activeInputs.size(); i++) {
        headers.push_back(activeInputs[i]->getName());
    }
    if (p==-1) {
        for (int i=0; i<data.size(); i++) {
            data[i]->setHeaders(headers);
        }
    } else {
        data[p]->setHeaders(headers);
    }
}

//-----------
void LearnOutputParameter::addSpreadsheetDataToLearn() {
    learn.clearTrainingInstances();
    for (int p=0; p<data.size(); p++) {
        vector<vector<float> > entries = data[p]->getEntries();
        for (int i=0; i<entries.size(); i++) {
            double normalizedLabel = (double) ofMap(entries[i][0], getMin(), getMax(), 0.0f, 1.0f);
            vector<double> instance;
            for (int j=1; j<entries[i].size(); j++) {
                instance.push_back((double) entries[i][j]);
            }
            learn.addTrainingInstance(instance, normalizedLabel);
        }
    }
}

//-----------
void LearnOutputParameter::trainClassifierFast() {
    if (getNumInstances() > 0) {
        addSpreadsheetDataToLearn();
        learn.trainRegression(FAST, REGRESSION_SVM);
        trained = true;
    }
}

//-----------
void LearnOutputParameter::trainClassifierAccurate() {
    if (getNumInstances() > 0) {
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
        data[page]->draw(420, 460);
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
    guiExamples->setLabelText(ofToString(getNumInstances())+" examples");
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
    guiData->setVisible(viewInputs);
    guiData->setPosition(420, 432);
    guiData->clearWidgets();
    guiDataStatus = guiData->addLabelButton(ofToString(getName())+" examples ("+ofToString(getNumInstances())+")", false, true);//, 200.0f);
    guiDataStatus->setColorBack(ofColor(0,0));
    guiDataStatus->getRect()->setWidth(200.0f);
    guiData->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiData->addSpacer(167, 0);
    guiData->addLabelButton("<", false, 15.0f);
    guiDataPage = guiData->addLabelButton("page 1/1", false, 60.0f);
    guiDataPage->setColorBack(ofColor(0,0));
    guiData->addLabelButton(">", false, 15.0f);
    guiData->addSpacer(20, 0);
    guiData->addLabelButton("clear all", false, 64.0f)->setColorBack(ofColor(255,0,0,100));
    guiData->autoSizeToFitWidgets();
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
        if (guiName->isFocused())
            ofNotifyEvent(pSelectedEvent, *this, this);
        guiSetName();
    }
    else if (e.getName() == "osc") {
        if (guiOsc->isFocused())
            ofNotifyEvent(pSelectedEvent, *this, this);
        guiSetOscAddress();
    }
    else if (e.getName() == "min") {
        if (guiMin->isFocused())
            ofNotifyEvent(pSelectedEvent, *this, this);
        guiSetMin();
    }
    else if (e.getName() == "max") {
        if (guiMax->isFocused())
            ofNotifyEvent(pSelectedEvent, *this, this);
        guiSetMax();
    }
    else if (e.getName() == "value") {
        guiValueText->setTextString(ofToString(guiValue->getValue()));
    }
    else if (e.getName() == "valueText") {
        if (guiValueText->isFocused())
            ofNotifyEvent(pSelectedEvent, *this, this);
        guiSetValueText();
    }
}

//-----------
void LearnParameter::guiSetName() {
    string newName = guiName->getTextString();
    if (newName == getName())   return;
    setName(newName);
    setOscAddress("/"+guiName->getTextString());
    guiOsc->setTextString(getOscAddress());
    ofNotifyEvent(pChangedEvent, *this, this);
}

//-----------
void LearnParameter::guiSetOscAddress() {
    string newOscAddress = guiOsc->getTextString();
    if (newOscAddress == getOscAddress())   return;
    setOscAddress(guiOsc->getTextString());
    ofNotifyEvent(pChangedEvent, *this, this);
}

//-----------
void LearnParameter::guiSetMin() {
    float currentValue = *getReference();
    float newValue = ofToFloat(guiMin->getTextString());
    setMin(newValue);
    guiValue->setMin(newValue);
    set(currentValue);
}

//-----------
void LearnParameter::guiSetMax() {
    float currentValue = *getReference();
    float newValue = ofToFloat(guiMax->getTextString());
    setMax(newValue);
    guiValue->setMax(ofToFloat(guiMax->getTextString()));
    set(currentValue);
}

//-----------
void LearnParameter::guiSetValueText() {
    guiValue->setValue(ofToFloat(guiValueText->getTextString()));
}

//-----------
void LearnParameter::deselect() {
    guiSetName();
    guiSetOscAddress();
    guiSetMin();
    guiSetMax();
    guiSetValueText();
    guiName->setFocus(false);
    guiOsc->setFocus(false);
    guiMin->setFocus(false);
    guiMax->setFocus(false);
    guiValueText->setFocus(false);
}

//-----------
void LearnInputParameter::guiEvent(ofxUIEventArgs &e) {
    LearnParameter::guiEvent(e);
    if (e.getName() == "X") {
        if (e.getButton()->getValue() == 1) return;
        string name = getName();
        ofNotifyEvent(pDeletedEvent, (LearnParameter &) *this, this);
    }
}

//-----------
void LearnOutputParameter::guiEvent(ofxUIEventArgs &e) {
    if (e.getName() == "X") {
        if (e.getButton()->getValue() == 1) return;
        if (ofSystemChoiceDialog("Really delete "+getName()+"?")) {
            ofNotifyEvent(pDeletedEvent, (LearnParameter &) *this, this);
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
        setExamplesVisible(guiExamples->getValue());
        ofNotifyEvent(pViewedEvent, *this, this);
    }
    else if (e.getName() == "inputs") {
        setInputsVisible(viewInputs);
        ofNotifyEvent(pViewedEvent, *this, this);
    }
    else if (e.getName() == "min" || e.getName() == "max") {
        if ((ofToFloat(guiMin->getTextString()) != getMin() ||
            ofToFloat(guiMax->getTextString()) != getMax()) &&
            getNumInstances() > 0) {
            if (!ofSystemChoiceDialog("Warning: this will overwrite all recorded examples and trained classifiers. Proceed?")) {
                guiMin->setTextString(ofToString(getMin()));
                guiMax->setTextString(ofToString(getMax()));
                return;
            }
            else {
                clearInstances();
            }
        }
    }
    LearnParameter::guiEvent(e);
}

//-----------
void LearnOutputParameter::setInputsVisible(bool b) {
    guiInputs->setValue(b);
    guiInputSelect->setVisible(viewInputs);
    viewExamples = viewInputs ? false : viewExamples;
}

//-----------
void LearnOutputParameter::setExamplesVisible(bool b) {
    guiExamples->setValue(b);
    viewExamples = b;
    guiData->setVisible(viewExamples);
    if (viewExamples) {
        viewInputs = false;
        guiInputSelect->setVisible(false);
    }
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
void LearnOutputParameter::setPage(int p) {
    page = ofClamp(p, 0, data.size()-1);
    guiDataPage->setLabelText("page "+ofToString(page+1)+"/"+ofToString(data.size()));
    for (int i=0; i<data.size(); i++) {
        data[i]->setInputsActive(false);
    }
    data[page]->setInputsActive(true);
}

//-----------
void LearnOutputParameter::guiDataEvent(ofxUIEventArgs &e) {
    if (e.getName() == "<") {
        if (e.getButton()->getValue() == 1) return;
        setPage(page-1);
    }
    else if (e.getName() == ">") {
        if (e.getButton()->getValue() == 1) return;
        setPage(page+1);
    }
    else if (e.getName() == "clear all") {
        if (e.getButton()->getValue() == 1) return;
        bool confirm = ofSystemChoiceDialog("Are you sure you want to delete all examples for "+getName()+"?");
        if (confirm) {
            clearInstances();
        }
    }
}

//-----------
void LearnOutputParameter::setVisible(bool visible){
    LearnParameter::setVisible(visible);
    viewExamples = visible ? viewExamples : false;
    viewInputs = visible ? viewInputs : false;
    guiData->setVisible(viewInputs);
    guiInputSelect->setVisible(viewInputs);
}

//-----------
void LearnOutputParameter::setGuiPosition(int x, int y) {
    gui->setPosition(x, y);
    guiInputSelect->setPosition(x, 460);
}


