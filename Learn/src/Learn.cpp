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
        oscManager.clearOutputTrackers();
        setupOscOutputs();
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
    gui3->addLabelToggle("predict", &predicting, 100, 50);
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
        
        
        bool saved = savePreset();
        
        if (saved) {
            
        }
        
        
        
        
        
        /*
         
         SAVE PRESET 
         
         */
    }
    else if (e.getName() == "load") {
        if (e.getButton()->getValue() == 1) return;
        cout << "load"<<endl;
        
        loadPreset("path");
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
    resetParameterGuiPositions();
}

//-------
void Learn::resetParameterGuiPositions() {
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
    vector<LearnInputParameter *>::iterator it=inputs.begin();
    while (it != inputs.end()) {
        if (*it == &input) {
            /* check if input tied into any outputs and ask to confirm */
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
            /* remove deleted input from all the outputs and delete itself */
            for (int i=0; i<outputs.size(); i++) {
                outputs[i]->removeInput(*it);
            }
            delete (*it);
            it = inputs.erase(it);
        }
        else ++it;
    }
    resetParameterGuiPositions();
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
    resetInputs();
    setupOscOutputs();
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
bool Learn::savePreset(string filename) {
    Presets presets;

    filename = "g";
    if (filename=="") {
        filename = ofSystemTextBoxDialog("Choose a filename");
    }
    if (filename=="") {
        return false;
    }
    string path = ofToDataPath("presets/"+filename+".xml");

    path = "/Users/Gene/Desktop/test.xml";
    
    ofXml xml;
    
    xml.addChild("LearnPreset");
    xml.setTo("LearnPreset");
    
    /* save inputs */
    
    xml.addChild("Inputs");
    xml.setTo("Inputs");
    for (int i=0; i<inputs.size(); i++) {
        ofXml xmlp = presets.getXml(inputs[i]);
        xml.addXml(xmlp);
    }
    xml.setToParent();
    
    /* save outputs */
    
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
        xmlp.addChild("Classifier");
        xmlp.setTo("Classifier");
        xmlp.setToParent();
        
        xml.addXml(xmlp);
    }
    xml.setToParent();
    
    xml.save(path);
    ofSystem("open "+path);
    
    return true;
}

//-------
void Learn::loadPreset(string filename) {
    Presets presets;
    ofXml xml;

    string path = ofToDataPath("presets/"+filename);
    path = "/Users/Gene/Desktop/test.xml";
    bool xmlLoaded = xml.load(path);
    if (!xmlLoaded) {
        cout << "failed to load preset " << "test.xml" << endl;
        return;
    }

    // store existing parameters to delete non-overwritten ones after loading done
    map<string, bool> inputsToDelete, outputsToDelete;
    for (int i=0; i<inputs.size(); i++) {
        inputsToDelete[inputs[i]->getName()] = true;
    }
    for (int i=0; i<outputs.size(); i++) {
        outputsToDelete[outputs[i]->getName()] = true;
    }
    
    xml.setTo("LearnPreset");

    /* Load Inputs */
    
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
            inputsToDelete[name] = false;
            
            input->setOscAddress(oscAddress);
            input->setMin(min);
            input->setMax(max);
            input->set(value);
        }
        while (xml.setToSibling());
        xml.setToParent();
    }
    xml.setToParent();

    // delete non-overwritten inputs from before loading
    for (int i=0; i<inputs.size(); i++) {
        if (inputsToDelete[inputs[i]->getName()]) {
            inputParameterDeleted((LearnParameter &) inputs[i]);
        }
    }
    inputsToDelete.clear();
    
    /* Load Outputs */
    
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
            }
            else {
                cout << "Error for Output "<<output->getName()<< " : not all inputs found, so skip loading examples."<<endl;
            }
        }
        while (xml.setToSibling());
        xml.setToParent();
    }
    xml.setToParent();
    
    // delete non-overwritten outputs from before loading
    for (int i=0; i<outputs.size(); i++) {
        if (inputsToDelete[outputs[i]->getName()]) {
            outputParameterDeleted((LearnParameter &) outputs[i]);
        }
    }
    outputsToDelete.clear();
}


