#include "Learn.h"


//-------
Learn::Learn(bool init) {
    trainDuration = 2;
    trainCountdown = 2;
    instanceRate = 15;
    startTime = -(trainDuration + trainCountdown);
    oscInActive = false;
    
    oscOutputHost = "localhost";
    oscOutputPort = 8000;
    oscInputPort = 9000;
    
    gui1 = new ofxUICanvas("setup");
    gui2 = new ofxUICanvas("train");
    gui3 = new ofxUICanvas("perform");

    if (init)   setupGui();
}

//-------
void Learn::update(){
    
    // send & receive osc
    if (oscInActive || oscOutActive)
        oscManager.update();
    
    // recording procedure
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
    
    // prediction procedure
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
        guiStatusLabel->setLabel("  => starting in "+ofToString(ceil(trainDuration - elapsed)));
    }
    else if (recording) {
        float elapsed = ofGetElapsedTimef() - startTime;
        guiStatusLabel->setLabel("  => record for "+ofToString(ceil(trainDuration + trainCountdown - elapsed)));
    }
}

//-------
LearnInputParameter * Learn::addInput(string name, float *value, float min, float max) {
    LearnInputParameter *newInput = new LearnInputParameter(name, value, min, max);
    newInput->setGuiPosition(10, 80+55*inputs.size());
    newInput->setVisible(true);
    newInput->addParameterChangedListener(this, &Learn::inputParameterChanged);
    newInput->addParameterDeletedListener(this, &Learn::inputParameterDeleted);
    newInput->addParameterSelectedListener(this, &Learn::parameterSelected);
    inputs.push_back(newInput);
    resetInputs();
    return newInput;
}

//-------
LearnOutputParameter * Learn::addOutput(string name, float *value, float min, float max) {
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
        oscManager.registerToOsc(newOutput, true);
    }
    if (oscManager.getReceiving()) {
        oscManager.registerToOsc(newOutput, false);
    }
    return newOutput;
}

//-------
LearnInputParameter * Learn::addInput(string name, float min, float max) {
    return addInput(name, new float(), min, max);
}

//-------
LearnOutputParameter * Learn::addOutput(string name, float min, float max) {
    return addOutput(name, new float(), min, max);
}

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
    oscOutActive = oscManager.setupSender(oscOutputHost, oscOutputPort);
}

void Learn::setupOscReceiver(int port) {
    if (port > 0) {
        this->oscInputPort = port;
        ((ofxUITextInput *) gui3->getWidget("oscPortIn"))->setTextString(ofToString(oscInputPort));
    }
    oscInActive = oscManager.setupReceiver(oscInputPort);
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
    guiStatusLabel = new ofxUILabel(120.0f, "", OFX_UI_FONT_SMALL, 16.0f);
    ofxUILabelButton *bTouchOscIn = new ofxUILabelButton("T", false, 12, 22, 0, 0, OFX_UI_FONT_SMALL);
    ofxUILabelButton *bTouchOscOut = new ofxUILabelButton("T", false, 12, 22, 0, 0, OFX_UI_FONT_SMALL);
    bTouchOscIn->setName("touchOscIn");
    bTouchOscOut->setName("touchOscOut");

    vector<string> presets;
    guiSelector = new ofxUIDropDownList("Load Preset", presets, 216, 0, 0, OFX_UI_FONT_SMALL);
    guiSelector->setAllowMultiple(false);
    guiSelector->setAutoClose(true);

    gui1->setColorOutline(ofColor(255,200));
    gui1->setDrawOutline(true);
    gui1->clearWidgets();
    gui1->setPosition(5, 5);
    gui1->setWidth(120);
    gui1->setHeight(60);
    gui1->addLabelButton("add input", false, 90, 22);
    gui1->addWidgetSouthOf(new ofxUILabelButton("add output", false,  90, 22, 0, 0, OFX_UI_FONT_SMALL), "add input")->setPadding(2);
    gui1->addWidgetEastOf(bTouchOscIn, "add input")->setPadding(1);
    gui1->addWidgetEastOf(bTouchOscOut, "add output")->setPadding(1);
    
    gui2->setColorOutline(ofColor(255,200));
    gui2->setDrawOutline(true);
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
    gui3->addTextInput("oscPortIn", ofToString(oscInputPort), 40.0f)->setAutoClear(false);
    gui3->addTextInput("oscHost", oscOutputHost, 90.0f)->setAutoClear(false);
    gui3->addTextInput("oscPortOut", ofToString(oscOutputPort), 40.0f)->setAutoClear(false);
    gui3->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    gui3->addWidgetSouthOf(new ofxUISpacer(0, 5, "gui3Spacer"), "oscLabel");
    gui3->addWidgetSouthOf(guiSelector, "gui3Spacer");
    gui3->addWidgetSouthOf(new ofxUILabelToggle("osc <", &oscInActive, 46, 22, 0, 0, OFX_UI_FONT_SMALL), "predict");
    gui3->addWidgetEastOf(new ofxUILabelToggle("osc >", &oscOutActive, 46, 22, 0, 0, OFX_UI_FONT_SMALL), "osc <");
    
    ofAddListener(gui1->newGUIEvent, this, &Learn::gui1Event);
    ofAddListener(gui2->newGUIEvent, this, &Learn::gui2Event);
    ofAddListener(gui3->newGUIEvent, this, &Learn::gui3Event);

    resetPresets();
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
    else if (e.getName() == "touchOscIn") {
        if (e.getButton()->getValue() == 1) return;
        saveInputsToTouchOsc();
    }
    else if (e.getName() == "touchOscOut") {
        if (e.getButton()->getValue() == 1) return;
        saveOutputsToTouchOsc();
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
    if (e.getName() == "save") {
        if (e.getButton()->getValue() == 1) return;
        bool saved = savePreset();
        if (saved) {
            resetPresets();
        }
    }
    else if (e.getParentName() == "Load Preset") {
        loadPresetDialog(e.getName());
    }
    else if (e.getName() == "oscHost" || e.getName() == "oscPortOut") {
        cout << " LETS TRY TO SET UP NEW HOST " << endl;
        string newHost = ((ofxUITextInput *) gui3->getWidget("oscHost"))->getTextString();
        int newPort = ofToInt(((ofxUITextInput *) gui3->getWidget("oscPortOut"))->getTextString());
        cout << " ===> " << newHost << " " << newPort << endl;
        if ((newHost != oscManager.getHost() || newPort != oscManager.getSenderPort())
            && newHost != "" && newPort > 0) {
            cout << " ------>" << newHost << " " << newPort << endl;
            setupOscSender(newHost, newPort);
            enableOscOutputs(oscOutActive);
        }
    }
    else if (e.getName() == "oscPortIn") {
        int newPort = ofToInt(((ofxUITextInput *) gui3->getWidget("oscPortIn"))->getTextString());
        if (newPort != oscManager.getReceiverPort() && newPort > 0) {
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
void Learn::resetGuiPositions() {
    for (int i=0; i<inputs.size(); i++) {
        inputs[i]->setGuiPosition(10, 80+55*i);
    }
    for (int i=0; i<outputs.size(); i++) {
        outputs[i]->setGuiPosition(420, 80+55*i);
    }
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


//-------
void Learn::inputParameterChanged(LearnParameter & input) {
    resetInputs();
}

//-------
void Learn::outputParameterChanged(LearnParameter & output) {
    resetOutputs();
}

//-------
void Learn::inputParameterDeleted(LearnParameter & input) {
    vector<LearnInputParameter *>::iterator it=inputs.begin();
    while (it != inputs.end()) {
        if (*it == &input) {
            // check if input tied into any outputs and ask to confirm
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
            // remove deleted input from all the outputs and delete itself
            for (int i=0; i<outputs.size(); i++) {
                outputs[i]->removeInput(*it);
            }
            delete (*it);
            it = inputs.erase(it);
        }
        else ++it;
    }
    resetGuiPositions();
}

//-------
void Learn::outputParameterDeleted(LearnParameter & output) {
    vector<LearnOutputParameter *>::iterator it=outputs.begin();
    while (it != outputs.end()) {
        if (*it == &output) {
            delete (*it);
            it = outputs.erase(it);
        }
        else ++it;
    }
    resetGuiPositions();
    oscManager.registerToOscReceiver((vector<ParameterBase *> &) outputs);
    resetOutputs();
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

//-------
void Learn::saveInputsToTouchOsc() {
    oscManager.saveTouchOscLayout("touchOsc_inputs", (vector<ParameterBase *> &) inputs);
}

//-------
void Learn::saveOutputsToTouchOsc() {
    oscManager.saveTouchOscLayout("touchOsc_outputs", (vector<ParameterBase *> &) outputs);
}

//-------
bool Learn::savePreset(string filename) {
    if (filename=="") {
        filename = ofSystemTextBoxDialog("Choose a filename");
    }
    if (filename=="")   return false;
    string path = ofToDataPath("presets/"+filename+".xml");
    ofXml xml;
    xml.addChild("LearnPreset");
    xml.setTo("LearnPreset");
    saveInputs(filename, xml);
    saveOutputs(filename, xml);
    xml.save(path);
    return true;
}

//-------
void Learn::saveInputs(string filename, ofXml &xml) {
    Presets presets;
    xml.addChild("Inputs");
    xml.setTo("Inputs");
    for (int i=0; i<inputs.size(); i++) {
        ofXml xmlp = presets.getXml(inputs[i]);
        xml.addXml(xmlp);
    }
    xml.setToParent();
}

//-------
void Learn::saveOutputs(string filename, ofXml &xml) {
    Presets presets;
    xml.addChild("Outputs");
    xml.setTo("Outputs");
    for (int i=0; i<outputs.size(); i++) {
        vector<LearnInputParameter *> activeInputs = outputs[i]->getActiveInputs();
        vector<vector<vector<float> > > instances = outputs[i]->getInstances();
        
        // add inputs
        ofXml xmlp = presets.getXml(outputs[i]);
        xmlp.addChild("Inputs");
        xmlp.setTo("Inputs");
        for (int j=0; j<activeInputs.size(); j++) {
            xmlp.addValue("Input", activeInputs[j]->getName());
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
            ofDirectory dir;
            string classifierPath = dir.getAbsolutePath();
            classifierPath += "presets/classifiers/"+filename+"_"+outputs[i]->getName()+".dat";
            outputs[i]->saveClassifier(classifierPath);
            xmlp.addValue("Classifier", classifierPath);
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
    } else {
        return ofToDataPath("presets/"+filename);
    }
}

//-------
void Learn::loadPreset(string filename) {
    Presets presets;
    ofXml xml;
    bool xmlLoaded = xml.load(filename);
    if (!xmlLoaded) {
        cout << "failed to load preset " << filename << endl;
        return;
    }
    xml.setTo("LearnPreset");
    loadInputs(xml);
    loadOutputs(xml);
}

//-------
void Learn::loadInputs(ofXml &xml) {
    // store existing parameters to delete non-overwritten ones after loading done
    map<string, bool> inputsToDelete;
    for (int i=0; i<inputs.size(); i++) {
        inputsToDelete[inputs[i]->getName()] = true;
    }
    
    xml.setTo("Inputs");
    if (xml.exists("Parameter")) {
        xml.setTo("Parameter[0]");
        do {
            string name = xml.getValue<string>("Name");
            string oscAddress = xml.getValue<string>("OscAddress");
            string type = xml.getValue<string>("Type");
            float value = xml.getValue<float>("Value");
            float min = xml.getValue<float>("Min");
            float max = xml.getValue<float>("Max");
            
            // input to load settings into
            LearnInputParameter * input;
            
            // try to find existing input with same name...
            bool inputExists = false;
            for (int i=0; i<inputs.size(); i++) {
                if (inputs[i]->getName() == name) {
                    input = inputs[i];
                    inputExists = true;
                    break;
                }
            }
            // ...or make new one if none found
            if (!inputExists) {
                input = addInput(name, new float(), min, max);
            }
            input->setOscAddress(oscAddress);
            input->setMin(min);
            input->setMax(max);
            input->set(value);
            inputsToDelete[name] = false;
        }
        while (xml.setToSibling());
        xml.setToParent();
    }
    xml.setToParent();
    
    // delete non-overwritten inputs from before loading
    vector<LearnInputParameter *> parametersToDelete;
    for (int i=0; i<inputs.size(); i++) {
        if (inputsToDelete[inputs[i]->getName()]) {
            parametersToDelete.push_back(inputs[i]);
        }
    }
    for (int i=0; i<parametersToDelete.size(); i++) {
        inputParameterDeleted((LearnParameter &) *parametersToDelete[i]);
    }
    parametersToDelete.clear();
    inputsToDelete.clear();
}

//-------
void Learn::loadOutputs(ofXml &xml) {
    // store existing parameters to delete non-overwritten ones after loading done
    map<string, bool> outputsToDelete;
    for (int i=0; i<outputs.size(); i++) {
        outputsToDelete[outputs[i]->getName()] = true;
    }
    
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
            
            // output to load settings into
            LearnOutputParameter * output;
            
            // try to find existing output with same name...
            bool outputExists = false;
            for (int i=0; i<outputs.size(); i++) {
                if (outputs[i]->getName() == name) {
                    output = outputs[i];
                    outputExists = true;
                    break;
                }
            }
            // ...or make new one if none found
            if (!outputExists) {
                output = addOutput(name, new float(), min, max);
            }
            outputsToDelete[name] = false;
            
            output->setOscAddress(oscAddress);
            output->setMin(min);
            output->setMax(max);
            output->set(value);
            output->setInputParameters(inputs);
            
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
                            output->addInput(inputs[i]);
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
                xml.setTo("Examples");
                if (xml.exists("Page[0]")) {
                    xml.setTo("Page[0]");
                    do {
                        if (output->getNumInstances() > 0) {
                            output->addDataPage();
                        }
                        if (xml.exists("Example[0]")) {
                            xml.setTo("Example[0]");
                            do {
                                vector<string> instanceStr = ofSplitString(xml.getValue(), ",");
                                vector<float> instance;
                                for (int i=0; i<instanceStr.size(); i++) {
                                    instance.push_back(ofToFloat(instanceStr[i]));
                                }
                                output->addInstance(instance);
                            }
                            while (xml.setToSibling());
                            xml.setToParent();
                        }
                    }
                    while (xml.setToSibling());
                    xml.setToParent();
                }
                xml.setToParent();
                
                if (xml.exists("Classifier")) {
                    string classifierPath = xml.getValue<string>("Classifier");
                    output->loadClassifier(classifierPath);
                }
            }
            else {
                cout << "Error for Output "<<output->getName()<< " : not all inputs found, so skip loading examples."<<endl;
            }
        }
        while (xml.setToSibling());
        xml.setToParent();
    }
    xml.setToParent();
    
    // delete non-overwritten inputs from before loading
    vector<LearnOutputParameter *> parametersToDelete;
    for (int i=0; i<outputs.size(); i++) {
        if (outputsToDelete[outputs[i]->getName()]) {
            parametersToDelete.push_back(outputs[i]);
        }
    }
    for (int i=0; i<parametersToDelete.size(); i++) {
        outputParameterDeleted((LearnParameter &) *parametersToDelete[i]);
    }
    parametersToDelete.clear();
    outputsToDelete.clear();
}

