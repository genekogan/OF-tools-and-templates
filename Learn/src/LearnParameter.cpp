#include "LearnParameter.h"


//===========================================
//  CONSTRUCTORS, DESTRUCTORS, SETTERS

//-----------
LearnParameter::LearnParameter(string name, float *value, float min, float max, bool rangeLocked) : Parameter<float>(name, *value, min, max) {
    *value = 0.5 * (min + max);
    gui = new ofxUICanvas(name);
    this->rangeLocked = rangeLocked;
}

//-----------
LearnInputParameter::LearnInputParameter(string name, float *value, float min, float max, bool rangeLocked) : LearnParameter(name, value, min, max, rangeLocked) {
    setupGui();
    ofAddListener(gui->newGUIEvent, this, &LearnInputParameter::guiEvent);
}

//-----------
LearnInputGroup::LearnInputGroup(string name) {
    this->name = name;
}


//-----------
LearnOutputParameter::LearnOutputParameter(string name, float *value, float min, float max, bool rangeLocked) : LearnParameter(name, value, min, max, rangeLocked) {
    vector<string> emptyInputLabels;
    guiInputSelect = new ofxUICanvas(name+"_inputs");
    guiSelector = guiInputSelect->addDropDownList("select inputs", emptyInputLabels, 190.0f);
    guiData = new ofxUICanvas(name+"_data");

    direct = false;
    record = false;
    trained = false;
    training = false;
    viewExamples = false;
    viewInputs = false;

    dataWidth = 600;
    dataHeight = 300;
    
    directLerp = 1.0;
    
    if (DEFAULT_LEARN_TYPE == "SVM") {
        setTrainingSvm();
    }
    else {
        setTrainingMlp(DEFAULT_MLP_HIDDEN_LAYERS,
                       DEFAULT_MLP_TARGET_RMSE,
                       DEFAULT_MLP_MAX_SAMPLES);
    }
    
    setupGui();
    addDataPage();

    ofAddListener(gui->newGUIEvent, this, &LearnOutputParameter::guiEvent);
    ofAddListener(guiInputSelect->newGUIEvent, this, &LearnOutputParameter::guiInputSelectEvent);
    ofAddListener(guiData->newGUIEvent, this, &LearnOutputParameter::guiDataEvent);
}

//-----------
LearnParameter::~LearnParameter() {
    gui->disable();
    gui->removeWidgets();
    pSelectedEvent.disable();
    pSelectedEvent.clear();
    pChangedEvent.disable();
    pChangedEvent.clear();
    pDeletedEvent.disable();
    pDeletedEvent.clear();
}

//-----------
LearnOutputParameter::~LearnOutputParameter() {
    //clearInstances();
    ofRemoveListener(gui->newGUIEvent, this, &LearnOutputParameter::guiEvent);
    ofRemoveListener(guiInputSelect->newGUIEvent, this, &LearnOutputParameter::guiInputSelectEvent);
    guiInputSelect->disable();
    guiInputSelect->removeWidgets();
    gui->clearWidgets();
    gui->removeWidgets();
    gui->disable();
    pViewedEvent.disable();
    pViewedEvent.clear();
}

//-----------
void LearnParameter::set(float value){
    Parameter<float>::set(value);
    guiValue->setValue(value);
    guiValueText->setTextString(ofToString(get()));
}

//-----------
void LearnParameter::setName(string name){
    Parameter<float>::setName(name);
    guiName->setTextString(getName());
}

//-----------
void LearnParameter::setOscAddress(string oscAddress_){
    ofStringReplace(oscAddress_, " ", "_");
    Parameter<float>::setOscAddress(oscAddress_);
    guiOsc->setTextString(getOscAddress());
}

//-----------
void LearnParameter::setMin(float min){
    Parameter<float>::setMin(min);
    guiMin->setTextString(ofToString(getMin()));
}

//-----------
void LearnParameter::setMax(float max) {
    Parameter<float>::setMax(max);
    guiMax->setTextString(ofToString(getMax()));
}

//-----------
void LearnParameter::setWarp(float warp) {
    ParameterBase::setWarp(warp);
    guiWarp->setTextString(ofToString(getWarp()));
}

//-----------
void LearnParameter::setReference(float *value) {
    Parameter<float>::setReference(value);
    guiValue->setReference(value);
}

//-----------
void LearnParameter::setVisible(bool visible){
    this->visible = visible;
    gui->setVisible(visible);
}




//===========================================
//  MANAGING INPUT GROUP PARAMETERS

//-----------
void LearnInputGroup::addInputParameter(LearnInputParameter * newInput) {
    inputs.push_back(newInput);
}

//-----------
void LearnInputGroup::addInputParameters(vector<LearnInputParameter *> newInputs) {
    for (int i=0; i<newInputs.size(); i++) {
        inputs.push_back(newInputs[i]);
    }
}

//-----------
LearnInputParameter * LearnInputGroup::addInputParameter(string name, float *value, float min, float max, bool rangeLocked) {
    LearnInputParameter *newInput = new LearnInputParameter(name, value, min, max, rangeLocked);
    inputs.push_back(newInput);
    return newInput;
}

//-----------
void LearnInputGroup::addInputParameter(string name, float min, float max, bool rangeLocked) {
    addInputParameter(name, new float(), min, max, rangeLocked);
}

//-----------
void LearnInputGroup::removeParameter(LearnInputParameter * parameter) {
    vector<LearnInputParameter *>::iterator it=inputs.begin();
    while (it != inputs.end()) {
        if (*it == parameter) {
            delete (*it);
            it = inputs.erase(it);
        }
        else ++it;
    }
}

//-----------
void LearnInputGroup::clearParameters() {
    for (int i=0; i<inputs.size(); i++) {
        delete inputs[i];
    }
    inputs.clear();
}




//===========================================
//  LEARNING

void LearnOutputParameter::addInstance(vector<float> instance) {
    data[page]->addEntry(instance);
}

//-----------
void LearnOutputParameter::addInstance() {
    addInstance(grabFeatureVector<float>(true));
}

//-----------
void LearnOutputParameter::clearInstances() {
    for (int i=0; i<data.size(); i++) {
        data[i]->removeSpreadsheetChangedListener(this, &LearnOutputParameter::dataChangedEvent);
        data[i]->clear();
        delete data[i];
    }
    data.clear();
    addDataPage();
    setPage(0);
    setTrained(false);
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
void LearnOutputParameter::setInputParameters(vector<LearnInputGroup *> &allInputs) {
    this->allInputs = allInputs;
    setupGuiInputSelector();
}

//-----------
void LearnOutputParameter::addDataPage() {
    ofxSpreadsheet *newData = new ofxSpreadsheet();
    newData->setup(dataWidth, dataHeight);
    newData->addSpreadsheetChangedListener(this, &LearnOutputParameter::dataChangedEvent);
    data.push_back(newData);
    setupHeaders(data.size()-1);
    setPage(data.size()-1);
}

//-----------
void LearnOutputParameter::setupHeaders(int p) {
    vector<string> headers;
    headers.push_back(name);
    for (int i=0; i<activeInputs.size(); i++) {
        vector<LearnInputParameter*> params = activeInputs[i]->getInputs();
        for (int j=0; j<params.size(); j++) {
            headers.push_back(params[j]->getName());
        }
    }
    if (p==-1) {
        for (int i=0; i<data.size(); i++) {
            data[i]->setHeaders(headers);
        }
    }
    else {
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
            int idxEntry = 1;
            for (int j=0; j<activeInputs.size(); j++) {
                vector<LearnInputParameter *> params = activeInputs[j]->getInputs();
                for (int k=0; k<params.size(); k++) {
                    float val = ((double) entries[i][idxEntry] - params[k]->getMin()) / (params[k]->getMax() - params[k]->getMin());
                    instance.push_back(val);
                    idxEntry++;
                }
            }
            learn.addTrainingInstance(getName(), instance, normalizedLabel);
        }
    }
}

//-----------
void LearnOutputParameter::setDataSize(int width, int height) {
    this->dataWidth = width;
    this->dataHeight = height;
    for (int i=0; i<data.size(); i++) {
        data[i]->setup(dataWidth, dataHeight);
    }
}

//-----------
void LearnOutputParameter::setTrainingMlp(int mlpNumHiddenLayers, float mlpTargetRmse, int mlpMaxSamples) {
    this->learnModel = MLP;
    this->mlpNumHiddenLayers = mlpNumHiddenLayers;
    this->mlpTargetRmse = mlpTargetRmse;
    this->mlpMaxSamples = mlpMaxSamples;
}

//-----------
void LearnOutputParameter::setTrainingSvm() {
    this->learnModel = SVM;
}

//-----------
void LearnOutputParameter::trainClassifier(TrainMode trainMode) {
    if (getNumInstances() > 0) {
        training = true;
        learn.clearTrainingInstances();
        addSpreadsheetDataToLearn();
        if (learnModel == MLP) {
            learn.setMlpNumHiddenLayers(mlpNumHiddenLayers);
            learn.setMlpTargetRmse(mlpTargetRmse);
            learn.setMlpMaxSamples(mlpMaxSamples);
            learn.trainRegression(REGRESSION_MLP, trainMode);
            setupMlpCoefficients();
        }
        else if (learnModel == SVM) {
            learn.trainRegression(REGRESSION_SVM, trainMode);
        }
        setRecording(false);
        setTrained(true);
        training = false;
    }
}

//---------
void LearnOutputParameter::setupMlpCoefficients() {
    dlib::matrix<double> w1m = learn.getRegressionMlp()->get_w1();
    dlib::matrix<double> w3m = learn.getRegressionMlp()->get_w3();
    int numLayers = learn.getMlpNumHiddenLayers();
    int numFeatures = getNumParameters(); //getNumInputs();
    mlpCoefficientsW1.clear();
    mlpCoefficientsW3.clear();
    for (int i=0; i<numLayers+1; i++) {
        for (int j=0; j<numFeatures+1; j++) {
            mlpCoefficientsW1.push_back(w1m(i, j));
        }
    }
    for (int i=0; i<numLayers+1; i++) {
        mlpCoefficientsW3.push_back(w3m(i));
    }
}

//-----------
void LearnOutputParameter::loadClassifierSvm(string path) {
    learnModel = SVM;
    learn.loadModel(REGRESSION_SVM, path);
    setTrained(true);
}

//-----------
void LearnOutputParameter::loadClassifierMlp(vector<double> w1, vector<double> w3) {
    learnModel = MLP;
    mlpCoefficientsW1.clear();
    mlpCoefficientsW3.clear();
    this->mlpCoefficientsW1 = w1;
    this->mlpCoefficientsW3 = w3;
    setTrained(true);
}

//-----------
void LearnOutputParameter::saveClassifier(string path) {
    if (trained) {
        learn.saveModel(path);
    }
}

//-----------
void LearnOutputParameter::predict() {
    if (direct) {
        setDirect();
    }
    else if (trained) {
        predict(grabFeatureVector<double>(false, true));
    }
}

//-----------
void LearnOutputParameter::predict(vector<double> instance) {
    double normalizedPrediction;
    if (learnModel == SVM) {
        normalizedPrediction = learn.predict(instance);
    }
    else if (learnModel == MLP) {
        normalizedPrediction = predictMlp(instance);
    }
    set((float) ofMap(normalizedPrediction, 0.0, 1.0, getMin(), getMax()));
    guiValueText->setTextString(ofToString(guiValue->getValue()));
}

//-----------
double LearnOutputParameter::predictMlp(vector<double> example) {
    int numLayers = learn.getMlpNumHiddenLayers();
    int numFeatures = getNumParameters();
    
    vector<double> z;
    for (int i=0; i<example.size(); i++) {
        z.push_back(example[i]);
    }
    z.push_back(-1.0);
    
    vector<double> tmp1;
    for (int i=0; i<numLayers+1; i++) {
        float tmp0 = 0.0;
        for (int j=0; j<numFeatures+1; j++) {
            tmp0 += ( mlpCoefficientsW1[ i*(numFeatures+1) + j ] * z[j] );
        }
        tmp1.push_back( sigmoid(tmp0) );
    }
    tmp1[numLayers] = -1.0;    // bias term overrides last element
    
    float tmp2 = 0.0;
    for (int j=0; j<numLayers+1; j++) {
        tmp2 += (mlpCoefficientsW3[j] * tmp1[j]);
    }
    return sigmoid(tmp2);
}

//-----------
void LearnOutputParameter::setDirect() {
    float newValue = 0.0;
    int numActiveParameters = 0;
    for (int i=0; i<activeInputs.size(); i++) {
        vector<LearnInputParameter*> params = activeInputs[i]->getInputs();
        for (int j=0; j<params.size(); j++) {
            newValue += (((Parameter<float> *) params[j])->get() - params[j]->getMin()) / (params[j]->getMax() - params[j]->getMin());
            numActiveParameters++;
        }
    }
    newValue = newValue / (float) numActiveParameters;
    set(ofLerp(get(), ofMap(newValue, 0.0, 1.0, getMin(), getMax()), directLerp));
    guiValueText->setTextString(ofToString(guiValue->getValue()));
}

//-----------
void LearnOutputParameter::draw() {
    if (viewExamples) {
        data[page]->draw(420, 460);
    }
    if (training) {
        if (ofGetFrameNum() % 8 == 0) {
            gui->setColorBack(ofColor(ofRandom(255),ofRandom(255),ofRandom(255)));
        }
    }
}

//-----------
vector<vector<vector<float> > > LearnOutputParameter::getInstances() {
    vector<vector<vector<float> > > instances;
    for (int i=0; i<data.size(); i++) {
        instances.push_back(data[i]->getEntries());
    }
    return instances;
}

//-----------
void LearnOutputParameter::exportData(string filename) {
    string path;
    if (filename=="") {
        ofFileDialogResult filesave = ofSystemSaveDialog("data_"+getName()+".csv", "Choose a location to save to");
        if (filesave.bSuccess) {
            path = filesave.getPath();
        }
        else return;
    }
    else {
        path = ofToDataPath(filename);
    }
    ofFile dataFile(path, ofFile::WriteOnly);
    string header = "Page,"+getName();
    for (int i=0; i<activeInputs.size(); i++) {
        header += ","+activeInputs[i]->getName();
    }
    dataFile << header << "\n";
    vector<vector<vector<float> > > examples = getInstances();
    for (int p=0; p<examples.size(); p++) {
        for (int i=0; i<examples[p].size(); i++) {
            string dataRow = ofToString(p);
            for (int j=0; j<examples[p][i].size(); j++) {
                dataRow += ","+ofToString(examples[p][i][j]);
            }
            dataFile << dataRow << "\n";
        }
    }
    dataFile.close();
    ofSystem("open "+path);
}


//===========================================
//  GUI

void LearnInputParameter::setupGui() {
    gui->setColorOutline(ofColor(255,200));
    gui->setDrawOutline(true);
    gui->clearWidgets();
    gui->addLabelButton("X", false, 15.0f);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiName = gui->addTextInput("name", ofToString(getName()), 155.0f);
    guiName->setAutoClear(false);
    gui->addLabel("osc:");
    guiOsc = gui->addTextInput("osc", ofToString(getOscAddress()), 155.0f);
    guiOsc->setAutoClear(false);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    guiValue = gui->addSlider("value", getMin(), getMax(), getReference(), 100.0f, 18.0f);
    guiValue->setLabelVisible(false);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiValueText = gui->addTextInput("valueText", ofToString(get()), 40.0f);
    guiValueText->setAutoClear(false);
    gui->addLabel("min:");
    guiMin = gui->addTextInput("min", ofToString(getMin()), 40.0f);
    gui->addLabel("max:");
    guiMax = gui->addTextInput("max", ofToString(getMax()), 40.0f);
    gui->addLabel("w:");
    guiWarp = gui->addTextInput("warp", ofToString(getWarp()), 30.0f);
    guiMin->setAutoClear(false);
    guiMax->setAutoClear(false);
    guiWarp->setAutoClear(false);
    gui->autoSizeToFitWidgets();
    gui->setWidth(GUI_INPUT_WIDTH);
}

//-----------
void LearnOutputParameter::setupGui() {
    cout << "SETUP LOP GUI " << ofGetFrameNum() << endl;
    
    guiData->clearWidgets();
    gui->setColorOutline(ofColor(255,200));
    gui->setDrawOutline(true);
    gui->clearWidgets();
    gui->setWidth(800);
    gui->addLabelButton("X", false, 15.0f);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiName = gui->addTextInput("name", ofToString(getName()), 100.0f);
    guiName->setAutoClear(false);
    gui->addLabel("osc:");
    guiOsc = gui->addTextInput("osc", ofToString(getOscAddress()), 145.0f);
    guiOsc->setAutoClear(false);
    guiDirect = gui->addLabelToggle("D", &direct, 20.0f);
    guiDirect->setName("direct");
    guiInputs = gui->addLabelToggle("inputs", &viewInputs, 60.0f);
    if (direct) {
        gui->addLabel("lerp:");
        guiLerpRate = gui->addMinimalSlider("lerpRate", 0.0f, 1.0f, &directLerp, 126.0f, 16.0f);
        guiLerpRate->setLabelVisible(false);
    }
    else {
        guiExamples = gui->addLabelToggle("examples", &viewExamples, 104.0f);
        guiExamples->setLabelText(ofToString(getNumInstances())+" examples");
        guiRecord = gui->addLabelToggle("record", &record, 56.0f);
    }
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    guiValue = gui->addSlider("value", getMin(), getMax(), getReference(), 240.0f, 18.0f);
    guiValue->setLabelVisible(false);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiValueText = gui->addTextInput("valueText", ofToString(get()), 75.0f);
    guiValueText->setAutoClear(false);
    gui->addLabel("w:");
    guiWarp = gui->addTextInput("warp", ofToString(getWarp()), 30.0f);
    guiWarp->setAutoClear(false);
    gui->addLabel("min:");
    guiMin = gui->addTextInput("min", ofToString(getMin()), 52.0f);
    guiMin->setAutoClear(false);
    gui->addLabel("max:");
    guiMax = gui->addTextInput("max", ofToString(getMax()), 52.0f);
    guiMax->setAutoClear(false);
    gui->autoSizeToFitWidgets();
    guiData->setVisible(viewInputs);
    guiData->setPosition(420, 432);
    guiData->clearWidgets();
    guiDataStatus = guiData->addLabelButton(ofToString(getName())+" examples ("+ofToString(getNumInstances())+")", false, true);
    guiDataStatus->setColorBack(ofColor(0,0));
    guiDataStatus->getRect()->setWidth(200.0f);
    guiData->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiData->addSpacer(130, 0);
    guiData->addLabelButton("<", false, 15.0f);
    guiDataPage = guiData->addLabelButton("page 1/1", false, 60.0f);
    guiDataPage->setColorBack(ofColor(0,0));
    guiData->addLabelButton(">", false, 15.0f);
    guiData->addSpacer(21, 0);
    guiData->addLabelButton("csv", false, 30.0f);
    guiData->addLabelButton("clear all", false, 64.0f)->setColorBack(ofColor(255,0,0,100));
    guiData->autoSizeToFitWidgets();
    gui->setWidth(GUI_OUTPUT_WIDTH);
    if (direct) {
        guiData->setVisible(false);
    }
}

//-----------
void LearnOutputParameter::setupGuiInputSelector() {
    cout << "SETUP LOP GUI INSPECT " << ofGetFrameNum() << endl;
    
    guiInputSelect->clearWidgets();
    guiInputSelect->setColorOutline(ofColor(255,200));
    guiInputSelect->setDrawOutline(true);
    
    vector<string> inputLabels;
    for (int i=0; i<allInputs.size(); i++) {
        inputLabels.push_back(allInputs[i]->getName());
    }
    
    guiSelector = guiInputSelect->addDropDownList("select inputs", inputLabels, 190.0f);
    vector<ofxUILabelToggle *> toggles = guiSelector->getToggles();
    for (int i=0; i<toggles.size(); i++) {
        string inputName = toggles[i]->getName();
        for (int j=0; j<activeInputs.size(); j++) {
            if (inputName == activeInputs[j]->getName()) {
                toggles[i]->setValue(value);
            }
        }
    }
    guiSelector->setAutoClose(false);
    guiSelector->setAllowMultiple(true);
    guiSelector->open();
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
    else if (e.getName() == "warp") {
        if (guiWarp->isFocused())
            ofNotifyEvent(pSelectedEvent, *this, this);
        guiSetWarp();
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
void LearnParameter::guiSetWarp() {
    float currentValue = *getReference();
    guiValue->setWarp(ofToFloat(guiWarp->getTextString()));
    set(currentValue);
}

//-----------
void LearnParameter::guiSetValueText() {
    float newValue = ofToFloat(guiValueText->getTextString());
    guiValue->setValue(newValue);
}

//-----------
void LearnParameter::updateGui() {
    guiSetName();
    guiSetOscAddress();
    guiSetMin();
    guiSetMax();
    //guiSetValueText();
}

//-----------
void LearnParameter::deselect() {
    updateGui();
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
        setRecording(record);
    }
    else if (e.getName() == "examples") {
        setExamplesVisible(guiExamples->getValue());
        ofNotifyEvent(pViewedEvent, *this, this);
    }
    else if (e.getName() == "inputs") {
        setInputsVisible(viewInputs);
        ofNotifyEvent(pViewedEvent, *this, this);
    }
    else if (e.getName() == "min" || e.getName() == "max" || e.getName() == "warp") {
        if ((ofToFloat(guiMin->getTextString()) != getMin() ||
            ofToFloat(guiMax->getTextString()) != getMax() ||
            ofToFloat(guiWarp->getTextString()) != getWarp()) &&
            getNumInstances() > 0) {
            if (!ofSystemChoiceDialog("Warning: this will overwrite all recorded examples and trained classifiers. Proceed?")) {
                guiMin->setTextString(ofToString(getMin()));
                guiMax->setTextString(ofToString(getMax()));
                guiWarp->setTextString(ofToString(getWarp()));
                return;
            }
            else {
                clearInstances();
                setTrained(false);
            }
        }
    }
    else if (e.getName() == "direct") {
        setupGui();
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
    vector<ofxUILabelToggle *> toggles = guiSelector->getToggles();
    activeInputs.clear();
    for (int i=0; i<toggles.size(); i++) {
        if (toggles[i]->getValue()) {
            addInput(allInputs[i]);
        }
    }
    setupHeaders();
}

//-----------
void LearnOutputParameter::addInput(LearnInputGroup * input) {
    for (int i=0; i<activeInputs.size(); i++) {
        if (input == activeInputs[i]) return;   // prevent duplicates
    }
    activeInputs.push_back(input);
    vector<ofxUILabelToggle *> toggles = guiSelector->getToggles();
    for (int i=0; i<toggles.size(); i++) {
        if (toggles[i]->getName() == input->getName()) {
            toggles[i]->setValue(true);
        }
    }
    setupHeaders();
}

//-----------
bool LearnOutputParameter::removeInput(LearnInputGroup * input) {
    for (int i=0; i<allInputs.size(); i++) {
        if (input == allInputs[i]) {
            allInputs.erase(allInputs.begin() + i);
            break;
        }
    }
    for (int i=0; i<activeInputs.size(); i++) {
        if (input == activeInputs[i]) {
            activeInputs.erase(activeInputs.begin() + i);
            clearInstances();
            setTrained(false);
        }
    }
    guiSelector->removeToggle(input->getName());
}

//-----------
int LearnOutputParameter::getNumParameters() {
    int n = 0;
    for (int i=0; i<activeInputs.size(); i++) {
        n += activeInputs[i]->getInputs().size();
    }
    return n;
}

//-----------
bool LearnOutputParameter::getInputActive(LearnInputGroup * input) {
    for (int i=0; i<activeInputs.size(); i++) {
        if (input == activeInputs[i])   return true;
    }
    return false;
}

//-----------
void LearnOutputParameter::activateAllInputs() {
    activeInputs.clear();
    for (int i=0; i<allInputs.size(); i++) {
        addInput(allInputs[i]);
    }
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
void LearnOutputParameter::setTrained(bool trained) {
    this->trained = trained;
    if (trained)    record = false;
    gui->setColorBack(trained ? ofColor(0,60,0,100) : record ? ofColor(200,0,0,200) : ofColor(0,100));
}

//-----------
void LearnOutputParameter::setRecording(bool record) {
    this->record = record;
    if (record && activeInputs.size()==0) {
        ofSystemAlertDialog("Can't record: no inputs selected.");
        this->record = false;
        return;
    }
    gui->setColorBack(record ? ofColor(200,0,0,200) : trained ? ofColor(0,60,0,100) : ofColor(0,100));
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
            setTrained(false);
        }
    }
    else if (e.getName() == "csv") {
        if (e.getButton()->getValue() == 1) return;
        exportData();
    }
}

//-----------
void LearnOutputParameter::dataChangedEvent(bool &b){
    guiExamples->setLabelText(ofToString(getNumInstances())+" examples");
    guiDataStatus->setLabelText(ofToString(getName())+" examples ("+ofToString(getNumInstances())+")");
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
}

//-----------
void LearnOutputParameter::setGuiInputsPosition(int x, int y) {
    guiInputSelect->setPosition(x, y);
}

//-----------
inline double LearnOutputParameter::sigmoid(double x) {
    return 1.0/(1.0 + pow(2.71828182845904523536028747135266249775724709369995, -x));
}

//-------
void LearnParameter::setFont(string path) {
    gui->setFont(path);
}

//-------
void LearnOutputParameter::setFont(string path) {
    LearnParameter::setFont(path);
    guiInputSelect->setFont(path);
    guiData->setFont(path);
}

//-------
void LearnParameter::setFontSizes(int small, int medium, int large) {
    gui->setFontSize(OFX_UI_FONT_SMALL, small);
    gui->setFontSize(OFX_UI_FONT_MEDIUM, medium);
    gui->setFontSize(OFX_UI_FONT_LARGE, large);
}

//-------
void LearnOutputParameter::setFontSizes(int small, int medium, int large) {
    LearnParameter::setFontSizes(small, medium, large);
    guiInputSelect->setFontSize(OFX_UI_FONT_SMALL, small);
    guiInputSelect->setFontSize(OFX_UI_FONT_MEDIUM, medium);
    guiInputSelect->setFontSize(OFX_UI_FONT_LARGE, large);
    guiData->setFontSize(OFX_UI_FONT_SMALL, small);
    guiData->setFontSize(OFX_UI_FONT_MEDIUM, medium);
    guiData->setFontSize(OFX_UI_FONT_LARGE, large);
}
