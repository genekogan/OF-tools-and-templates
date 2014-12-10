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
    
    
    
    //addWeightedCentroidVelocityAsInput2();

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
            vector<InputFeature> subFeature;
            subFeature.push_back(InputFeature(manta.getPadRef(row, col), 0, 196));
            inputFeatures["pad "+ofToString(row)+" "+ofToString(col)] = subFeature;
            features.push_back(InputFeature(manta.getPadRef(row, col), 0, 196, "pad "+ofToString(row)+" "+ofToString(col)));
        }
    }
    inputFeatures["all pads"] = features;
}

//-----------
void MantaLearn::addSlidersAsInput() {
	vector<InputFeature> features, feature0, feature1;
	features.push_back(InputFeature(manta.getSliderRef(0), 0, 1, "slider 0"));
	features.push_back(InputFeature(manta.getSliderRef(1), 0, 1, "slider 1"));
    inputFeatures["all sliders"] = features;
	feature0.push_back(InputFeature(manta.getSliderRef(0), 0, 1));
	feature1.push_back(InputFeature(manta.getSliderRef(1), 0, 1));
	inputFeatures["slider 0"] = feature0;
    inputFeatures["slider 1"] = feature1;
}

//-----------
void MantaLearn::addButtonsAsInput() {
	vector<InputFeature> features, feature0, feature1, feature2, feature3;
	features.push_back(InputFeature(manta.getButtonRef(0), 0, 1, "button 0"));
	features.push_back(InputFeature(manta.getButtonRef(1), 0, 1, "button 1"));
	features.push_back(InputFeature(manta.getButtonRef(2), 0, 1, "button 2"));
	features.push_back(InputFeature(manta.getButtonRef(3), 0, 1, "button 3"));
	inputFeatures["all buttons"] = features;
	feature0.push_back(InputFeature(manta.getButtonRef(0), 0, 1));
	feature1.push_back(InputFeature(manta.getButtonRef(1), 0, 1));
	feature2.push_back(InputFeature(manta.getButtonRef(2), 0, 1));
	feature3.push_back(InputFeature(manta.getButtonRef(3), 0, 1));
	inputFeatures["button 0"] = feature0;
	inputFeatures["button 1"] = feature1;
	inputFeatures["button 2"] = feature2;
	inputFeatures["button 3"] = feature3;
}

//-----------
void MantaLearn::addNumPadsAsInput() {
	vector<InputFeature> features;
	features.push_back(InputFeature(&manta.getNumPads(), 0, 48));
	inputFeatures["number pads"] = features;
}

//-----------
void MantaLearn::addPadSumAsInput() {
	vector<InputFeature> features;
	features.push_back(InputFeature(&manta.getPadSum(), 0, 3000));
	inputFeatures["pad sum"] = features;
}

//-----------
void MantaLearn::addPadAverageAsInput() {
	vector<InputFeature> features;
	features.push_back(InputFeature(&manta.getPadAverage(), 0, 196));
	inputFeatures["pad average"] = features;
}

//-----------
void MantaLearn::addPerimeterAsInput() {
	vector<InputFeature> features;
	features.push_back(InputFeature(&manta.getPerimeter(), 0, 3));
	inputFeatures["perimeter"] = features;
}

//-----------
void MantaLearn::addPadWidthAsInput() {
	vector<InputFeature> features;
	features.push_back(InputFeature(&manta.getWidth(), 0, 1));
	inputFeatures["width"] = features;
}

//-----------
void MantaLearn::addPadHeightAsInput() {
	vector<InputFeature> features;
	features.push_back(InputFeature(&manta.getHeight(), 0, 1));
	inputFeatures["height"] = features;
}

//-----------
void MantaLearn::addPadWHRatioAsInput() {
	vector<InputFeature> features;
	features.push_back(InputFeature(&manta.getWidthHeightRatio(), 0, 10));
	inputFeatures["width/height ratio"] = features;
}

//-----------
void MantaLearn::addAverageInterFingerDistanceAsInput() {
	vector<InputFeature> features;
	features.push_back(InputFeature(&manta.getAverageInterFingerDistance(), 0, 2));
	inputFeatures["avg finger distance"] = features;
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
            vector<InputFeature> subFeature;
			subFeature.push_back(InputFeature(manta.getPadVelocityRef(row, col), -20, 20));
			features.push_back(InputFeature(manta.getPadVelocityRef(row, col), -20, 20, "pad velocity "+ofToString(row)+" "+ofToString(col)));
            inputFeatures["pad velocity "+ofToString(row)+" "+ofToString(col)] = subFeature;
        }
    }
	inputFeatures["all pad velocities"] = features;
}

//-----------
void MantaLearn::addSliderVelocitiesAsInput() {
	vector<InputFeature> features, feature0, feature1;
	features.push_back(InputFeature(manta.getSliderVelocityRef(0), -0.1, 0.1, "slider velocity 0"));
	features.push_back(InputFeature(manta.getSliderVelocityRef(1), -0.1, 0.1, "slider velocity 1"));
	inputFeatures["all slider velocities"] = features;
	feature0.push_back(InputFeature(manta.getSliderVelocityRef(0), -0.1, 0.1));
	feature1.push_back(InputFeature(manta.getSliderVelocityRef(1), -0.1, 0.1));
	inputFeatures["slider velocity 0"] = feature0;
	inputFeatures["slider velocity 1"] = feature1;
}

//-----------
void MantaLearn::addButtonVelocitiesAsInput() {
	vector<InputFeature> features, feature0, feature1, feature2, feature3;
	features.push_back(InputFeature(manta.getButtonVelocityRef(0), -0.1, 0.1, "button velocity 0"));
	features.push_back(InputFeature(manta.getButtonVelocityRef(1), -0.1, 0.1, "button velocity 1"));
	features.push_back(InputFeature(manta.getButtonVelocityRef(2), -0.1, 0.1, "button velocity 2"));
	features.push_back(InputFeature(manta.getButtonVelocityRef(3), -0.1, 0.1, "button velocity 3"));
	inputFeatures["all button velocities"] = features;
	feature0.push_back(InputFeature(manta.getButtonVelocityRef(0), -0.1, 0.1));
	feature1.push_back(InputFeature(manta.getButtonVelocityRef(1), -0.1, 0.1));
	feature2.push_back(InputFeature(manta.getButtonVelocityRef(2), -0.1, 0.1));
	feature3.push_back(InputFeature(manta.getButtonVelocityRef(3), -0.1, 0.1));
	inputFeatures["button velocity 0"] = feature0;
	inputFeatures["button velocity 1"] = feature1;
	inputFeatures["button velocity 2"] = feature2;
	inputFeatures["button velocity 3"] = feature3;
}

//-----------
void MantaLearn::addNumPadsVelocityAsInput() {
	vector<InputFeature> features;
	features.push_back(InputFeature(&manta.getNumPadsVelocity(), -1, 1));
	inputFeatures["number pads velocity"] = features;
}

//-----------
void MantaLearn::addPadSumVelocityAsInput() {
	vector<InputFeature> features;
	features.push_back(InputFeature(&manta.getPadSumVelocity(), -200, 200));
	inputFeatures["pad sum velocity"] = features;
}

//-----------
void MantaLearn::addPadAverageVelocityAsInput() {
	vector<InputFeature> features;
	features.push_back(InputFeature(&manta.getPadAverageVelocity(), -30, 30));
	inputFeatures["pad average velocity"] = features;
}

//-----------
void MantaLearn::addPerimeterVelocityAsInput() {
	vector<InputFeature> features;
	features.push_back(InputFeature(&manta.getPerimeterVelocity(), -1, 1));
	inputFeatures["perimeter velocity"] = features;
}

//-----------
void MantaLearn::addPadWidthVelocityAsInput() {
	vector<InputFeature> features;
	features.push_back(InputFeature(&manta.getWidthVelocity(), -0.1, 0.1));
	inputFeatures["width velocity"] = features;
}

//-----------
void MantaLearn::addPadHeightVelocityAsInput() {
	vector<InputFeature> features;
	features.push_back(InputFeature(&manta.getHeightVelocity(), -0.1, 0.1));
	inputFeatures["height velocity"] = features;
}

//-----------
void MantaLearn::addPadWHRatioVelocityAsInput() {
	vector<InputFeature> features;
	features.push_back(InputFeature(&manta.getWidthHeightRatioVelocity(), -0.2, 0.2));
	inputFeatures["width/height ratio velocity"] = features;
}

//-----------
void MantaLearn::addAverageInterFingerDistanceVelocityAsInput() {
	vector<InputFeature> features;
	features.push_back(InputFeature(&manta.getAverageInterFingerDistanceVelocity(), -0.1, 0.1));
	inputFeatures["avg finger distance velocity"] = features;
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
void MantaLearn::addInputFeatureSet(string name) {
    if (inputFeatures.count(name) == 0) {
        ofLog(OF_LOG_ERROR, "Error: no feature set named "+name+" found");
        return;
    }
    else {
        vector<LearnInputParameter*> newInputs;
        for (int f=0; f<inputFeatures[name].size(); f++) {
            string featureName = inputFeatures[name][f].name == "" ? name : inputFeatures[name][f].name;
            newInputs.push_back(Learn::addInput(featureName,
                                                inputFeatures[name][f].value,
                                                inputFeatures[name][f].min,
                                                inputFeatures[name][f].max,
                                                true));
        }
        addParametersAsInput(name, newInputs);
    }
}

//-----------
void MantaLearn::removeInputFeatureSet(string name) {
    if (inputFeatures.count(name) == 0) {
        ofLog(OF_LOG_ERROR, "Error: no feature set named "+name+" found");
        return;
    }
    else {
        Learn::removeParameterGroupAsInput(name);
    }
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
            e.getButton()->getValue() ? addInputFeatureSet(name) : removeInputFeatureSet(name);
        }
    }
    else if (e.getName()=="pad individual velocity") {
        if (guiRow >= -1 && guiCol >= -1) {
            string name = "pad velocity "+ofToString(guiRow)+" "+ofToString(guiCol);
            e.getButton()->getValue() ? addInputFeatureSet(name) : removeInputFeatureSet(name);
        }
    }
    else if (inputFeatures.count(e.getName()) != 0) {
        e.getButton()->getValue() ? addInputFeatureSet(e.getName()) : removeInputFeatureSet(e.getName());
    }

    // reset
    // add individual ones as well!
    if (e.getName() == "all pads" ||
        e.getName() == "all sliders" ||
        e.getName() == "all buttons") {
        resetManta();
    }
}

//-----------
LearnInputParameter * MantaLearn::addInput(string name, float min, float max) {
    if (inputFeatures.count(name) != 0) {
        for (int i=0; i<inputFeatures[name].size(); i++) {
            inputFeatures[name][i].min = min;
            inputFeatures[name][i].max = max;
        }
        
        // NO RETURN!!!
        addInputFeatureSet(name);
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
