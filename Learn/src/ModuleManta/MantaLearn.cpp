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
    guiInputs->setPosition(5, 87);
    guiInputs->setColorBack(ofColor(60));
    ofAddListener(guiInputs->newGUIEvent, this, &MantaLearn::guiInputEvent);
    setupGuiInputs();
    
    gui = new ofxUICanvas("MantaLearn");
    gui->setPosition(5, 5);
    gui->setColorOutline(ofColor(255,200));
    gui->setDrawOutline(true);
    gui->clearWidgets();
    gui->addToggle("input selector", &inputsVisible);
    gui->addSlider("velocity lerp", 0.0, 1.0, &manta.getVelocityLerpRate())->getRect()->setWidth(100.0f);
    gui->addLabelButton("add output", false, 90.0f);
    gui->autoSizeToFitWidgets();
    gui->getRect()->setWidth(120.0f);
    ofAddListener(gui->newGUIEvent, this, &MantaLearn::guiEvent);

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
    resetMantaInputSelector();
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
    addSingleInputFeature("width/height ratio", &manta.getWidthHeightRatio(), 0, 10);
}

//-----------
void MantaLearn::addAverageInterFingerDistanceAsInput() {
    addSingleInputFeature("avg finger distance", &manta.getAverageInterFingerDistance(), 0, 2);
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
	features.push_back(InputFeature(&manta.getWeightedCentroidX(), 0, 1, "weighted centroid x"));
	features.push_back(InputFeature(&manta.getWeightedCentroidY(), 0, 1, "weighted centroid y"));
	inputFeatures["weighted centroid"] = features;
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
    addSingleInputFeature("width/height ratio velocity", &manta.getWidthHeightRatioVelocity(), -0.2, 0.2);
}

//-----------
void MantaLearn::addAverageInterFingerDistanceVelocityAsInput() {
    addSingleInputFeature("avg finger distance velocity", &manta.getAverageInterFingerDistanceVelocity(), -0.1, 0.1);
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
	features.push_back(InputFeature(&manta.getWeightedCentroidVelocityX(), -0.1, 0.1, "weighted centroid x velocity"));
	features.push_back(InputFeature(&manta.getWeightedCentroidVelocityY(), -0.1, 0.1, "weighted centroid y velocity"));
	inputFeatures["weighted centroid velocity"] = features;
}

//-----------
void MantaLearn::setupGuiInputs() {
    guiInputs->clearWidgets();
    guiInputs->addLabel("Manta features");
    
    guiInputs->addLabelToggle("all pads", &allPads, 140.0f);
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiInputs->addLabelToggle("all pad velocities", &vAllPads, 50.0f)->setLabelText("velocity");
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);

    guiInputs->addLabelToggle("all sliders", &allSliders, 140.0f);
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiInputs->addLabelToggle("all slider velocities", &vAllSliders, 50.0f)->setLabelText("velocity");
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    guiInputs->addLabelToggle("all buttons", &allButtons, 140.0f);
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiInputs->addLabelToggle("all button velocities", &vAllButtons, 50.0f)->setLabelText("velocity");
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    guiInputs->addLabelToggle("number pads", &numPads, 140.0f);
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiInputs->addLabelToggle("number pads velocity", &vNumPads, 50.0f)->setLabelText("velocity");
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);

    guiInputs->addLabelToggle("pad sum", &padSum, 140.0f);
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiInputs->addLabelToggle("pad sum velocity", &vPadSum, 50.0f)->setLabelText("velocity");
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    guiInputs->addLabelToggle("pad average", &padAvg, 140.0f);
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiInputs->addLabelToggle("pad average velocity", &vPadAvg, 50.0f)->setLabelText("velocity");
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    guiInputs->addLabelToggle("perimeter", &perimeter, 140.0f);
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiInputs->addLabelToggle("perimeter velocity", &vPerimeter, 50.0f)->setLabelText("velocity");
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    guiInputs->addLabelToggle("width", &width, 140.0f);
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiInputs->addLabelToggle("width velocity", &vWidth, 50.0f)->setLabelText("velocity");
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    guiInputs->addLabelToggle("height", &height, 140.0f);
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiInputs->addLabelToggle("height velocity", &vHeight, 50.0f)->setLabelText("velocity");
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);

    guiInputs->addLabelToggle("width/height ratio", &whRatio, 140.0f);
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiInputs->addLabelToggle("width/height ratio velocity", &vWhRatio, 50.0f)->setLabelText("velocity");
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    guiInputs->addLabelToggle("avg finger distance", &avgInterDist, 140.0f);
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiInputs->addLabelToggle("avg finger distance velocity", &vAvgInterDist, 50.0f)->setLabelText("velocity");
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    guiInputs->addLabelToggle("centroid", &centroid, 140.0f);
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiInputs->addLabelToggle("centroid velocity", &vCentroid, 50.0f)->setLabelText("velocity");
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    guiInputs->addLabelToggle("weighted centroid", &wCentroid, 140.0f);
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiInputs->addLabelToggle("weighted centroid velocity", &vWCentroid, 50.0f)->setLabelText("velocity");
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    guiInputs->addSpacer();
    guiInputs->addLabelToggle("slider 0", &sliderVal[0], 140.0f);
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiInputs->addLabelToggle("slider velocity 0", &sliderVel[0], 50.0f)->setLabelText("velocity");
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    guiInputs->addLabelToggle("slider 1", &sliderVal[1], 140.0f);
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiInputs->addLabelToggle("slider velocity 1", &sliderVel[1], 50.0f)->setLabelText("velocity");
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    guiInputs->addSpacer();
    guiInputs->addLabelToggle("button 0", &buttonVal[0], 140.0f);
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiInputs->addLabelToggle("button velocity 0", &buttonVel[0], 50.0f)->setLabelText("velocity");
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    guiInputs->addLabelToggle("button 1", &buttonVal[1], 140.0f);
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiInputs->addLabelToggle("button velocity 1", &buttonVel[1], 50.0f)->setLabelText("velocity");
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    guiInputs->addLabelToggle("button 2", &buttonVal[2], 140.0f);
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiInputs->addLabelToggle("button velocity 2", &buttonVel[2], 50.0f)->setLabelText("velocity");
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    guiInputs->addLabelToggle("button 3", &buttonVal[3], 140.0f);
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiInputs->addLabelToggle("button velocity 3", &buttonVel[3], 50.0f)->setLabelText("velocity");
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    guiInputs->addSpacer();
    
    guiInputs->addLabel("Select pad: ")->getRect()->setWidth(64.0f);
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiInputs->addDropDownList("Row", rows, 42.0f);
    guiInputs->addDropDownList("Col", cols, 42.0f);
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    if (guiRow >= 0 && guiCol >= 0) {
        guiInputs->addLabel("Pad ("+ofToString(guiRow)+","+ofToString(guiCol)+")")->getRect()->setWidth(64.0f);
        guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
        guiInputs->addLabelToggle("pad individual value", &padVal[guiRow][guiCol], 48.0f)->setLabelText("value");
        guiInputs->addLabelToggle("pad individual velocity", &padVel[guiRow][guiCol], 56.0f)->setLabelText("velocity");
        guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    }
    guiInputs->addSpacer();

    guiInputs->autoSizeToFitWidgets();
    guiInputs->setWidth(210);
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
    if      (e.getParentName() == "Row") {
        guiRow = ofToInt(e.getName());
        setupGuiInputs();
    }
    else if (e.getParentName() == "Col") {
        guiCol = ofToInt(e.getName());
        setupGuiInputs();
    }
    else if (e.getName()=="pad individual value") {
        if (guiRow >= -1 && guiCol >= -1) {
            string name = "pad "+ofToString(guiRow)+" "+ofToString(guiCol);
            e.getButton()->getValue() ? addInputFeatureSet(name) : Learn::removeInputGroup(name);
        }
    }
    else if (e.getName()=="pad individual velocity") {
        if (guiRow >= -1 && guiCol >= -1) {
            string name = "pad velocity "+ofToString(guiRow)+" "+ofToString(guiCol);
            e.getButton()->getValue() ? addInputFeatureSet(name) : Learn::removeInputGroup(name);
        }
    }
    else if (inputFeatures.count(e.getName()) != 0) {
        e.getButton()->getValue() ? addInputFeatureSet(e.getName()) : Learn::removeInputGroup(e.getName());
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
        else if (inputs[i]->getName() == "width/height ratio")  whRatio = true;
        else if (inputs[i]->getName() == "width/height ratio velocity")  vWhRatio = true;
        else if (inputs[i]->getName() == "avg finger distance")  avgInterDist = true;
        else if (inputs[i]->getName() == "avg finger distance velocity")  vAvgInterDist = true;
        else if (inputs[i]->getName() == "centroid")  centroid = true;
        else if (inputs[i]->getName() == "centroid velocity")  vCentroid = true;
        else if (inputs[i]->getName() == "weighted centroid")  wCentroid = true;
        else if (inputs[i]->getName() == "weighted centroid velocity")  vWCentroid = true;
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