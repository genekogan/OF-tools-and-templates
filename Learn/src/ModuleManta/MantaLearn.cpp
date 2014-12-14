#include "MantaLearn.h"


//-----------
MantaLearn::MantaLearn() : Learn() {
    inputsVisible = true;
    mantaVisible = true;

    guiRow = -1;
    guiCol = -1;
    for (int r=0; r<6; r++) rows.push_back(ofToString(r));
    for (int c=0; c<8; c++) cols.push_back(ofToString(c));
    for (int r=0; r<6; r++) {
        for (int c=0; c<8; c++) {
            customPads.push_back("pad ("+ofToString(r)+","+ofToString(c)+")");
        }
    }

    guiInputs = new ofxUICanvas("Manta Inputs");
    guiInputs->setColorOutline(ofColor(255, 200));
    guiInputs->setDrawOutline(true);
    guiInputs->setVisible(inputsVisible);
    
    //guiInputs->setPosition(5, 87);
    guiInputs->setPosition(420, 200);
    
    guiInputs->setColorBack(ofColor(60));
    ofAddListener(guiInputs->newGUIEvent, this, &MantaLearn::guiInputEvent);
    setupGuiInputs();
    
    gui = new ofxUICanvas("MantaLearn");
    gui->setPosition(5, 5);
    ofAddListener(gui->newGUIEvent, this, &MantaLearn::guiEvent);
    setupGuiOutputs(true);

    // setup manta
    manta.setup();
    setMantaVisible(mantaVisible);
    setGuiInputsVisible(false);
    gui1->setVisible(false);
    
    // add all features as saved feature inputs
    addAllPadsAsInput();
    addSlidersAsInput();
    addButtonsAsInput();
    addNumPadsAsInput();
    addPadSumAsInput();
    addPadAverageAsInput();
    addPerimeterAsInput();
    addPadWidthAsInput();
    addPadHeightAsInput();
    addPadWHRatioAsInput();
    addAverageInterFingerDistanceAsInput();
    addCentroidAsInput();
    addWeightedCentroidAsInput();
    addAllPadVelocitiesAsInput();
    addSliderVelocitiesAsInput();
    addButtonVelocitiesAsInput();
    addNumPadsVelocityAsInput();
    addPadSumVelocityAsInput();
    addPadAverageVelocityAsInput();
    addPerimeterVelocityAsInput();
    addPadWidthVelocityAsInput();
    addPadHeightVelocityAsInput();
    addPadWHRatioVelocityAsInput();
    addAverageInterFingerDistanceVelocityAsInput();
    addCentroidVelocityAsInput();
    addWeightedCentroidVelocityAsInput();
}

//-----------
void MantaLearn::loadPreset(string path) {
    Learn::loadPreset(path);
    setupGuiInputs();
    //resetMantaInputSelector();
}

//-----------
void MantaLearn::update() {
    manta.update();
    Learn::update();
}

//-----------
void MantaLearn::draw() {
    if (visible) {
        if (mantaVisible) {
            manta.draw(5, 85, 410);
            manta.drawStats(5, 420, 410);
        }
        Learn::draw();
    }
}

//-----------
void MantaLearn::createCustomFeatureSetFromMantaSelection() {
    vector<int> padSelection = manta.getPadSelection();
    vector<int> padVelocitySelection = manta.getPadVelocitySelection();
    vector<int> sliderSelection = manta.getSliderSelection();
    vector<int> sliderVelocitySelection = manta.getSliderVelocitySelection();
    vector<int> buttonSelection = manta.getButtonSelection();
    vector<int> buttonVelocitySelection = manta.getButtonVelocitySelection();
    
    vector<InputFeature> features;
    for (int i=0; i<padSelection.size(); i++) {
        int row = floor(padSelection[i] / 8);
        int col = padSelection[i] % 8;
        features.push_back(InputFeature(manta.getPadRef(row, col), 0, 196, "pad "+ofToString(row)+" "+ofToString(col)));
    }
    for (int i=0; i<padVelocitySelection.size(); i++) {
        int row = floor(padVelocitySelection[i] / 8);
        int col = padVelocitySelection[i] % 8;
        features.push_back(InputFeature(manta.getPadVelocityRef(row, col), 0, 196, "pad velocity "+ofToString(row)+" "+ofToString(col)));
    }
    for (int i=0; i<sliderSelection.size(); i++) {
        features.push_back(InputFeature(manta.getSliderRef(sliderSelection[i]), 0, 1, "slider "+ofToString(sliderSelection[i])));
    }
    for (int i=0; i<sliderVelocitySelection.size(); i++) {
        features.push_back(InputFeature(manta.getSliderVelocityRef(sliderVelocitySelection[i]), 0, 1, "slider velocity "+ofToString(sliderVelocitySelection[i])));
    }
    for (int i=0; i<buttonSelection.size(); i++) {
        features.push_back(InputFeature(manta.getButtonRef(buttonSelection[i]), 0, 1, "button "+ofToString(buttonSelection[i])));
    }
    for (int i=0; i<buttonVelocitySelection.size(); i++) {
        features.push_back(InputFeature(manta.getButtonVelocityRef(buttonVelocitySelection[i]), 0, 1, "button velocity "+ofToString(buttonVelocitySelection[i])));
    }
    
    string newInputName;
    if (features.size() == 1) {
        newInputName = features[0].name;
        addInputFeatureSet(newInputName);
    }
    else {
        newInputName = ofSystemTextBoxDialog("Name of custom feature set:");
        if (newInputName=="")   newInputName = "custom "+ofToString(numCustomFeatureSets++);
        if (features.size() > 0) {
            inputFeatures[newInputName] = features;
            addInputFeatureSet(newInputName);
        }
    }
    manta.clearSelection();
    
    customInputFeatures.push_back(newInputName);

    guiInputs->addLabelToggle(newInputName, hasInput(newInputName), 110.0f);
    guiInputs->autoSizeToFitWidgets();
    guiInputs->setWidth(120);
    guiInputs->setPosition(420, 365-22*customInputFeatures.size());
}

//-----------
void MantaLearn::addAllPadsAsInput() {
    vector<InputFeature> features;
    for (int row=0; row<6; row++) {
        for (int col=0; col<8; col++) {
            addSingleInputFeature("pad "+ofToString(row)+" "+ofToString(col), manta.getPadRef(row, col), 0, 196);
            features.push_back(InputFeature(manta.getPadRef(row, col), 0, 196, "pad "+ofToString(row)+" "+ofToString(col)));
        }
    }
    inputFeatures["all pads"] = features;
}

//-----------
void MantaLearn::addSlidersAsInput() {
	vector<InputFeature> features;
	features.push_back(InputFeature(manta.getSliderRef(0), 0, 1, "slider 0"));
	features.push_back(InputFeature(manta.getSliderRef(1), 0, 1, "slider 1"));
    inputFeatures["all sliders"] = features;
    addSingleInputFeature("slider 0", manta.getSliderRef(0), 0, 1);
    addSingleInputFeature("slider 1", manta.getSliderRef(1), 0, 1);
}

//-----------
void MantaLearn::addButtonsAsInput() {
	vector<InputFeature> features;
	features.push_back(InputFeature(manta.getButtonRef(0), 0, 1, "button 0"));
	features.push_back(InputFeature(manta.getButtonRef(1), 0, 1, "button 1"));
	features.push_back(InputFeature(manta.getButtonRef(2), 0, 1, "button 2"));
	features.push_back(InputFeature(manta.getButtonRef(3), 0, 1, "button 3"));
	inputFeatures["all buttons"] = features;
    addSingleInputFeature("button 0", manta.getButtonRef(0), 0, 1);
    addSingleInputFeature("button 1", manta.getButtonRef(1), 0, 1);
    addSingleInputFeature("button 2", manta.getButtonRef(2), 0, 1);
    addSingleInputFeature("button 3", manta.getButtonRef(3), 0, 1);
}

//-----------
void MantaLearn::addNumPadsAsInput() {
    addSingleInputFeature("number pads", &manta.getNumPads(), 0, 48);
}

//-----------
void MantaLearn::addPadSumAsInput() {
    addSingleInputFeature("pad sum", &manta.getPadSum(), 0, 3000);
}

//-----------
void MantaLearn::addPadAverageAsInput() {
    addSingleInputFeature("pad average", &manta.getPadAverage(), 0, 196);
}

//-----------
void MantaLearn::addPerimeterAsInput() {
    addSingleInputFeature("perimeter", &manta.getPerimeter(), 0, 3);
}

//-----------
void MantaLearn::addPadWidthAsInput() {
    addSingleInputFeature("width", &manta.getWidth(), 0, 1);
}

//-----------
void MantaLearn::addPadHeightAsInput() {
    addSingleInputFeature("height", &manta.getHeight(), 0, 1);
}

//-----------
void MantaLearn::addPadWHRatioAsInput() {
    addSingleInputFeature("w/h ratio", &manta.getWidthHeightRatio(), 0, 10);
}

//-----------
void MantaLearn::addAverageInterFingerDistanceAsInput() {
    addSingleInputFeature("finger dist", &manta.getAverageInterFingerDistance(), 0, 2);
}

//-----------
void MantaLearn::addCentroidAsInput() {
	vector<InputFeature> features;
	features.push_back(InputFeature(&manta.getCentroidX(), 0, 1, "centroid x"));
	features.push_back(InputFeature(&manta.getCentroidY(), 0, 1, "centroid y"));
	inputFeatures["centroid"] = features;
}

//-----------
void MantaLearn::addWeightedCentroidAsInput() {
	vector<InputFeature> features;
	features.push_back(InputFeature(&manta.getWeightedCentroidX(), 0, 1, "wtd centroid x"));
	features.push_back(InputFeature(&manta.getWeightedCentroidY(), 0, 1, "wtd centroid y"));
	inputFeatures["wtd centroid"] = features;
}

//-----------
void MantaLearn::addAllPadVelocitiesAsInput() {
	vector<InputFeature> features;
    for (int row=0; row<6; row++) {
        for (int col=0; col<8; col++) {
            addSingleInputFeature("pad velocity "+ofToString(row)+" "+ofToString(col), manta.getPadVelocityRef(row, col), -20, 20);
			features.push_back(InputFeature(manta.getPadVelocityRef(row, col), -20, 20, "pad velocity "+ofToString(row)+" "+ofToString(col)));
        }
    }
	inputFeatures["all pad velocities"] = features;
}

//-----------
void MantaLearn::addSliderVelocitiesAsInput() {
	vector<InputFeature> features;
	features.push_back(InputFeature(manta.getSliderVelocityRef(0), -0.1, 0.1, "slider velocity 0"));
	features.push_back(InputFeature(manta.getSliderVelocityRef(1), -0.1, 0.1, "slider velocity 1"));
	inputFeatures["all slider velocities"] = features;
    addSingleInputFeature("slider velocity 0", manta.getSliderVelocityRef(0), -0.1, 0.1);
    addSingleInputFeature("slider velocity 1", manta.getSliderVelocityRef(1), -0.1, 0.1);

}

//-----------
void MantaLearn::addButtonVelocitiesAsInput() {
	vector<InputFeature> features;
	features.push_back(InputFeature(manta.getButtonVelocityRef(0), -0.1, 0.1, "button velocity 0"));
	features.push_back(InputFeature(manta.getButtonVelocityRef(1), -0.1, 0.1, "button velocity 1"));
	features.push_back(InputFeature(manta.getButtonVelocityRef(2), -0.1, 0.1, "button velocity 2"));
	features.push_back(InputFeature(manta.getButtonVelocityRef(3), -0.1, 0.1, "button velocity 3"));
	inputFeatures["all button velocities"] = features;
    addSingleInputFeature("button velocity 0", manta.getButtonVelocityRef(0), -0.1, 0.1);
    addSingleInputFeature("button velocity 1", manta.getButtonVelocityRef(1), -0.1, 0.1);
    addSingleInputFeature("button velocity 2", manta.getButtonVelocityRef(2), -0.1, 0.1);
    addSingleInputFeature("button velocity 3", manta.getButtonVelocityRef(3), -0.1, 0.1);

}

//-----------
void MantaLearn::addNumPadsVelocityAsInput() {
    addSingleInputFeature("number pads velocity", &manta.getNumPadsVelocity(), -1, 1);
}

//-----------
void MantaLearn::addPadSumVelocityAsInput() {
    addSingleInputFeature("pad sum velocity", &manta.getPadSumVelocity(), -200, 200);
}

//-----------
void MantaLearn::addPadAverageVelocityAsInput() {
    addSingleInputFeature("pad average velocity", &manta.getPadAverageVelocity(), -30, 30);
}

//-----------
void MantaLearn::addPerimeterVelocityAsInput() {
    addSingleInputFeature("perimeter velocity", &manta.getPerimeterVelocity(), -1, 1);
}

//-----------
void MantaLearn::addPadWidthVelocityAsInput() {
    addSingleInputFeature("width velocity", &manta.getWidthVelocity(), -0.1, 0.1);
}

//-----------
void MantaLearn::addPadHeightVelocityAsInput() {
    addSingleInputFeature("height velocity", &manta.getHeightVelocity(), -0.1, 0.1);
}

//-----------
void MantaLearn::addPadWHRatioVelocityAsInput() {
    addSingleInputFeature("w/h ratio velocity", &manta.getWidthHeightRatioVelocity(), -0.2, 0.2);
}

//-----------
void MantaLearn::addAverageInterFingerDistanceVelocityAsInput() {
    addSingleInputFeature("finger dist velocity", &manta.getAverageInterFingerDistanceVelocity(), -0.1, 0.1);
}

//-----------
void MantaLearn::addCentroidVelocityAsInput() {
	vector<InputFeature> features;
	features.push_back(InputFeature(&manta.getCentroidVelocityX(), -0.1, 0.1, "centroid x velocity"));
	features.push_back(InputFeature(&manta.getCentroidVelocityY(), -0.1, 0.1, "centroid y velocity"));
	inputFeatures["centroid velocity"] = features;
}

//-----------
void MantaLearn::addWeightedCentroidVelocityAsInput() {
	vector<InputFeature> features;
	features.push_back(InputFeature(&manta.getWeightedCentroidVelocityX(), -0.1, 0.1, "wtd centroid x velocity"));
	features.push_back(InputFeature(&manta.getWeightedCentroidVelocityY(), -0.1, 0.1, "wtd centroid y velocity"));
	inputFeatures["wtd centroid velocity"] = features;
}

//-----------
void MantaLearn::setupGuiOutputs(bool showAddOutputOption) {
    gui->setColorOutline(ofColor(255,200));
    gui->setDrawOutline(true);
    gui->clearWidgets();
    gui->addToggle("input selector", &inputsVisible);
    gui->addSlider("velocity lerp", 0.0, 1.0, &manta.getVelocityLerpRate())->getRect()->setWidth(100.0f);
    if (showAddOutputOption) {
        gui->addLabelButton("add output", false, 90.0f);
    }
    gui->autoSizeToFitWidgets();
    gui->getRect()->setWidth(120.0f);
}

//-----------
void MantaLearn::setupGuiInputs() {
    guiInputs->setPosition(420, 365-22*customInputFeatures.size());

    
    
    /* DONT CLEAR? */
    
    guiInputs->clearWidgets();
    
    
    
    
    
    guiInputs->addLabel("Manta");
    guiInputs->addSpacer(110, 1);
    guiInputs->addLabelButton("add custom", false, 110.0f)->setColorBack(ofColor(0,100,0));
    guiInputs->addSpacer(110, 1);

    guiEntryHelper("all pads", "all pad velocities");
    guiEntryHelper("all sliders", "all slider velocities");
    guiEntryHelper("all buttons", "all button velocities");
    guiInputs->addSpacer(110, 1);
    guiEntryHelper("number pads", "number pads velocity");
    guiEntryHelper("pad sum", "pad sum velocity");
    guiEntryHelper("pad average", "pad average velocity");
    guiEntryHelper("perimeter", "perimeter velocity");
    guiEntryHelper("width", "width velocity");
    guiEntryHelper("height", "height velocity");
    guiEntryHelper("w/h ratio", "w/h ratio velocity");
    guiEntryHelper("finger dist", "finger dist velocity");
    guiEntryHelper("centroid", "centroid velocity");
    guiEntryHelper("wtd centroid", "wtd centroid velocity");
    
    guiInputs->addSpacer(110,1);
    for (int i=0; i<customInputFeatures.size(); i++) {
        guiInputs->addLabelToggle(customInputFeatures[i], hasInput(customInputFeatures[i]), 110.0f);
    }
    guiInputs->autoSizeToFitWidgets();
    guiInputs->setWidth(120);
}

//-----------
void MantaLearn::guiEntryHelper(string b1, string b2) {
    /*
    guiInputs->addLabelToggle(b1, false, 90.0f);
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiInputs->addLabelToggle(b2, false, 15.0f)->setLabelText("V");
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
     */
    
    
    guiInputs->addLabelToggle(b1, hasInput(b1), 90.0f);
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiInputs->addLabelToggle(b2, hasInput(b2), 15.0f)->setLabelText("V");
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);

}

//-----------
void MantaLearn::guiEvent(ofxUIEventArgs &e) {
    if (e.getName() == "input selector") {
        guiInputs->setVisible(e.getButton()->getValue());
    }
    else if (e.getName() == "add output") {
        if (e.getButton()->getValue()==1)   return;
        addOutput("newOutput_"+ofToString(outputs.size()), 0, 1);
    }
}

//-----------
void MantaLearn::guiInputEvent(ofxUIEventArgs &e) {
    // add custom issue
    if (e.getName() == "add custom") {
        if (e.getButton()->getValue())  return;
        createCustomFeatureSetFromMantaSelection();
        resetManta();
    }
    
    // toggle global inputs
    else if (inputFeatures.count(e.getName()) != 0) {
        if (e.getButton()->getValue()) {
            addInputFeatureSet(e.getName());
        }
        else {
            Learn::removeInputGroup(e.getName());
        }
    }
    
    // toggle custom inputs
    else {
        for (int i=0; i<customInputFeatures.size(); i++) {
            if (customInputFeatures[i] == e.getName()) {
                Learn::removeInputGroup(e.getName());
                return;
            }
        }
    }

    

    // reset manta
    if (e.getName() == "all pads" || e.getName() == "all sliders" || e.getName() == "all buttons" ||
        e.getName() == "slider 0" || e.getName() == "slider 1" ||
        e.getName() == "button 0" || e.getName() == "button 1" || e.getName() == "button 2" || e.getName() == "button 3") {
        resetManta();
    }
    else {
        for (int r=0; r<6; r++) {
            for (int c=0; c<8; c++) {
                if (e.getName() == "pad "+ofToString(r)+" "+ofToString(c)) {
                    resetManta();
                    return;
                }
            }
        }
    }
}

//-----------
void MantaLearn::setMantaVisible(bool mantaVisible) {
    this->mantaVisible = mantaVisible;
    manta.setVisible(mantaVisible);
    gui->setVisible(mantaVisible);
    guiInputs->setVisible(mantaVisible);
}

//-----------
void MantaLearn::resetManta() {
    *manta.getSliderRef(0) = 0;
    *manta.getSliderRef(1) = 0;
    *manta.getButtonRef(0) = 0;
    *manta.getButtonRef(1) = 0;
    *manta.getButtonRef(2) = 0;
    *manta.getButtonRef(3) = 0;
    for (int row=0; row<6; row++) {
        for (int col=0; col<8; col++) {
            *manta.getPadRef(row, col) = 0;
        }
    }
}

//-----------
MantaLearn::~MantaLearn() {
    setMantaVisible(false);
    ofRemoveListener(guiInputs->newGUIEvent, this, &MantaLearn::guiInputEvent);
    guiInputs->disable();
    delete guiInputs;
    manta.close();
}

/*
//-----------
void MantaLearn::resetMantaInputSelector() {
    
    allPads = false;
    allSliders = false;
    allButtons = false;
    numPads = false;
    padSum = false;
    padAvg = false;
    perimeter = false;
    width = false;
    height = false;
    whRatio = false;
    avgInterDist = false;
    centroid = false;
    wCentroid = false;
    vAllPads = false;
    vAllSliders = false;
    vAllButtons = false;
    vNumPads = false;
    vPadSum = false;
    vPadAvg = false;
    vPerimeter = false;
    vWidth = false;
    vHeight = false;
    vWhRatio = false;
    vAvgInterDist = false;
    vCentroid = false;
    vWCentroid = false;
    for (int r=0; r<6; r++) {
        for (int c=0; c<8; c++) {
            padVal[r][c] = false;
            padVel[r][c] = false;
        }
    }
    for (int s=0; s<2; s++) {
        sliderVal[s] = false;
        sliderVel[s] = false;
    }
    for (int b=0; b<4; b++) {
        buttonVal[b] = false;
        buttonVel[b] = false;
    }
    
    for (int i=0; i<inputs.size(); i++) {
        if      (inputs[i]->getName() == "all pads")  allPads = true;
        else if (inputs[i]->getName() == "all pad velocities")  vAllPads = true;
        else if (inputs[i]->getName() == "all sliders")  allSliders = true;
        else if (inputs[i]->getName() == "all slider velocities")  vAllSliders = true;
        else if (inputs[i]->getName() == "all buttons")  allButtons = true;
        else if (inputs[i]->getName() == "all button velocities")  vAllButtons = true;
        else if (inputs[i]->getName() == "number pads")  numPads = true;
        else if (inputs[i]->getName() == "number pads velocity")  vNumPads = true;
        else if (inputs[i]->getName() == "pad sum")  padSum = true;
        else if (inputs[i]->getName() == "pad sum velocity")  vPadSum = true;
        else if (inputs[i]->getName() == "pad average")  padAvg = true;
        else if (inputs[i]->getName() == "pad average velocity")  vPadAvg = true;
        else if (inputs[i]->getName() == "perimeter")  perimeter = true;
        else if (inputs[i]->getName() == "perimeter velocity")  vPerimeter = true;
        else if (inputs[i]->getName() == "width")  width = true;
        else if (inputs[i]->getName() == "width velocity")  vWidth = true;
        else if (inputs[i]->getName() == "height")  height = true;
        else if (inputs[i]->getName() == "height velocity")  vHeight = true;
        else if (inputs[i]->getName() == "w/h ratio")  whRatio = true;
        else if (inputs[i]->getName() == "w/h ratio velocity")  vWhRatio = true;
        else if (inputs[i]->getName() == "finger dist")  avgInterDist = true;
        else if (inputs[i]->getName() == "finger dist velocity")  vAvgInterDist = true;
        else if (inputs[i]->getName() == "centroid")  centroid = true;
        else if (inputs[i]->getName() == "centroid velocity")  vCentroid = true;
        else if (inputs[i]->getName() == "wtd centroid")  wCentroid = true;
        else if (inputs[i]->getName() == "wtd centroid velocity")  vWCentroid = true;
        else if (inputs[i]->getName() == "slider 0")  sliderVal[0] = true;
        else if (inputs[i]->getName() == "slider velocity 0")  sliderVel[0] = true;
        else if (inputs[i]->getName() == "slider 1")  sliderVal[1] = true;
        else if (inputs[i]->getName() == "slider velocity 1")  sliderVel[1] = true;
        else if (inputs[i]->getName() == "button 0")  buttonVal[0] = true;
        else if (inputs[i]->getName() == "button velocity 0")  buttonVel[0] = true;
        else if (inputs[i]->getName() == "button 1")  buttonVal[1] = true;
        else if (inputs[i]->getName() == "button velocity 1")  buttonVel[1] = true;
        else if (inputs[i]->getName() == "button 2")  buttonVal[2] = true;
        else if (inputs[i]->getName() == "button velocity 2")  buttonVel[2] = true;
        else if (inputs[i]->getName() == "button 3")  buttonVal[3] = true;
        else if (inputs[i]->getName() == "button velocity 3")  buttonVel[3] = true;
    }
}
*/