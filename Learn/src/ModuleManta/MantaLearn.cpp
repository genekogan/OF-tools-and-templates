#include "MantaLearn.h"


//-----------
MantaLearn::MantaLearn() : Learn() {
    for (int r=0; r<6; r++) {
        for (int c=0; c<8; c++) {
            customPads.push_back("pad ("+ofToString(r+1)+","+ofToString(c+1)+")");
        }
    }
    for (int i=0; i<6; i++) rows.push_back(ofToString(i+1));
    for (int i=0; i<8; i++) cols.push_back(ofToString(i+1));
    guiRow = -1;
    guiCol = -1;
    
    guiInputs = new ofxUICanvas();
    setupGuiInputs();
    guiInputs->setVisible(false);
    ofAddListener(guiInputs->newGUIEvent, this, &MantaLearn::guiInputEvent);
    
    gui = new ofxUICanvas("MantaLearn");
    gui->setPosition(5, 75);
    gui->setColorBack(ofColor(20));
    gui->clearWidgets();
    gui->addToggle("input selector", false);
    gui->autoSizeToFitWidgets();
    ofAddListener(gui->newGUIEvent, this, &MantaLearn::guiEvent);

    manta.setup();
    mantaVisible = true;
    setMantaVisible(mantaVisible);
    setGuiInputsVisible(false);
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
            manta.draw(5, 80, 410);
            manta.drawStats(5, 420, 410);
        }
        Learn::draw();
    }
}

//-----------
void MantaLearn::addAllPadsAsInput() {
    vector<LearnInputParameter*> newInputs;
    for (int row=0; row<6; row++) {
        for (int col=0; col<8; col++) {
            newInputs.push_back(addInput("mantaPad"+ofToString(col)+ofToString(row), manta.getPadRef(row, col), 0, 196));
            *manta.getPadRef(row, col) = 0;
        }
    }
    addParametersAsInput("all 48 pads", newInputs);
}

//-----------
void MantaLearn::addSlidersAsInput() {
    vector<LearnInputParameter*> newInputs;
    newInputs.push_back(addInput("slider1", manta.getSliderRef(0), 0, 4096));
    newInputs.push_back(addInput("slider2", manta.getSliderRef(1), 0, 4096));
    addParametersAsInput("sliders", newInputs);
    *manta.getSliderRef(0) = 0;
    *manta.getSliderRef(1) = 0;
}

//-----------
void MantaLearn::addButtonsAsInput() {
    vector<LearnInputParameter*> newInputs;
    newInputs.push_back(addInput("button1", manta.getButtonRef(0), 0, 250));
    newInputs.push_back(addInput("button2", manta.getButtonRef(1), 0, 250));
    newInputs.push_back(addInput("button3", manta.getButtonRef(2), 0, 250));
    newInputs.push_back(addInput("button4", manta.getButtonRef(3), 0, 250));
    addParametersAsInput("buttons", newInputs);
    *manta.getButtonRef(0) = 0;
    *manta.getButtonRef(1) = 0;
    *manta.getButtonRef(2) = 0;
    *manta.getButtonRef(3) = 0;
}

//-----------
void MantaLearn::addNumFingersAsInput() {
    addParameterAsInput("num fingers", addInput("numFingers", &manta.getNumFingers(), 0, 24));
}

//-----------
void MantaLearn::addPadSumAsInput() {
    addParameterAsInput("pad sum", addInput("padSum", &manta.getPadSum(), 0, 10));
}

//-----------
void MantaLearn::addPadAverageAsInput() {
    addParameterAsInput("pad average", addInput("padAverage", &manta.getPadAverage(), 0, 10));
}

//-----------
void MantaLearn::addPerimeterAsInput() {
    addParameterAsInput("perimeter", addInput("perimeter", &manta.getPerimeter(), 0, 10));
}

//-----------
void MantaLearn::addAverageInterFingerDistanceAsInput() {
    addParameterAsInput("avg finger distance", addInput("avgFingerDistance", &manta.getAverageInterFingerDistance(), 0, 10));
}

//-----------
void MantaLearn::addCentroidAsInput() {
    vector<LearnInputParameter*> newInputs;
    newInputs.push_back(addInput("centroidX", &manta.getCentroidX(), 0, 10));
    newInputs.push_back(addInput("centroidY", &manta.getCentroidY(), 0, 10));
    addParametersAsInput("centroid", newInputs);
}

//-----------
void MantaLearn::addWeightedCentroidAsInput() {
    vector<LearnInputParameter*> newInputs;
    newInputs.push_back(addInput("wCentroidX", &manta.getWeightedCentroidX(), 0, 10));
    newInputs.push_back(addInput("wCentroidY", &manta.getWeightedCentroidY(), 0, 10));
    addParametersAsInput("weighted centroid", newInputs);
}


//-----------
void MantaLearn::addAllPadVelocitiesAsInput() {
    vector<LearnInputParameter*> newInputs;
    for (int row=0; row<6; row++) {
        for (int col=0; col<8; col++) {
            newInputs.push_back(addInput("mantaVelocity"+ofToString(col)+ofToString(row), manta.getPadVelocityRef(row, col), -20, 20));
        }
    }
    addParametersAsInput("all 48 pad velocities", newInputs);
}

//-----------
void MantaLearn::addSliderVelocitiesAsInput() {
    vector<LearnInputParameter*> newInputs;
    newInputs.push_back(addInput("sliderVel1", manta.getSliderVelocityRef(0), -1, 1));
    newInputs.push_back(addInput("sliderVel2", manta.getSliderVelocityRef(1), -1, 1));
    addParametersAsInput("slider velocities", newInputs);
}

//-----------
void MantaLearn::addPadSumVelocityAsInput() {
    addParameterAsInput("pad sum velocity", addInput("padSumVel", &manta.getPadSumVelocity(), -200, 200));
}

//-----------
void MantaLearn::addPadAverageVelocityAsInput() {
    addParameterAsInput("pad average velocity", addInput("padAverageVel", &manta.getPadAverageVelocity(), -30, 30));
}

//-----------
void MantaLearn::addPerimeterVelocityAsInput() {
    addParameterAsInput("perimeter velocity", addInput("perimeterVel", &manta.getPerimeterVelocity(), -1, 1));
}

//-----------
void MantaLearn::addAverageInterFingerDistanceVelocityAsInput() {
    addParameterAsInput("avg finger distance velocity", addInput("avgFingerDistanceVel", &manta.getAverageInterFingerDistanceVelocity(), -0.1, 0.1));
}

//-----------
void MantaLearn::addCentroidVelocityAsInput() {
    vector<LearnInputParameter*> newInputs;
    newInputs.push_back(addInput("centroidXvel", &manta.getCentroidVelocityX(), -0.1, 0.1));
    newInputs.push_back(addInput("centroidYvel", &manta.getCentroidVelocityY(), -0.1, 0.1));
    addParametersAsInput("centroid velocity", newInputs);
}

//-----------
void MantaLearn::addWeightedCentroidVelocityAsInput() {
    vector<LearnInputParameter*> newInputs;
    newInputs.push_back(addInput("wCentroidXvel", &manta.getWeightedCentroidVelocityX(), -0.1, 0.1));
    newInputs.push_back(addInput("wCentroidYvel", &manta.getWeightedCentroidVelocityY(), -0.1, 0.1));
    addParametersAsInput("weighted centroid velocity", newInputs);
}

//-----------
void MantaLearn::setupInputs() {
    clearInputs();
    
    if (allPads)  addAllPadsAsInput();
    if (allSliders)  addSlidersAsInput();
    if (allButtons)  addButtonsAsInput();
    if (numFingers)  addNumFingersAsInput();
    if (padSum)  addPadSumAsInput();
    if (padAvg)  addPadAverageAsInput();
    if (perimeter)  addPerimeterAsInput();
    if (avgInterDist)  addAverageInterFingerDistanceAsInput();
    if (centroid)  addCentroidAsInput();
    if (wCentroid)  addWeightedCentroidAsInput();
    if (vAllPads)  addAllPadVelocitiesAsInput();
    if (vAllSliders)  addSliderVelocitiesAsInput();
    if (vPadSum)  addPadSumVelocityAsInput();
    if (vPadAvg)  addPadAverageVelocityAsInput();
    if (vPerimeter)  addPerimeterVelocityAsInput();
    if (vAvgInterDist)  addAverageInterFingerDistanceVelocityAsInput();
    if (vCentroid)  addCentroidVelocityAsInput();
    if (vWCentroid)  addWeightedCentroidVelocityAsInput();
    
    for (int r=0; r<8; r++) {
        for (int c=0; c<6; c++) {
            if (padVal[r][c]) {
                addParameterAsInput("mantaPad-"+ofToString(r)+"-"+ofToString(c),
                                    addInput("mantaPad"+ofToString(r)+ofToString(c),
                                             manta.getPadVelocityRef(r, c),
                                             -30, 30));
            }
            if (padVel[r][c]) {
                addParameterAsInput("mantaVelocity-"+ofToString(r)+"-"+ofToString(c),
                                    addInput("mantaVelocity"+ofToString(r)+ofToString(c),
                                             manta.getPadVelocityRef(guiRow, guiCol),
                                             -1, 1));
            }
        }
    }
    
    for (int i=0; i<2; i++) {
        if (sliderVal[i])   addParameterAsInput("sliderValue"+ofToString(i),
                                                addInput("sliderValue"+ofToString(i),
                                                         manta.getSliderRef(i), 0, 4096));
        if (sliderVel[i])   addParameterAsInput("sliderVelocity"+ofToString(i),
                                                addInput("sliderVelocity"+ofToString(i),
                                                         manta.getSliderVelocityRef(i), -20, 20));
    }

}

//-----------
void MantaLearn::setupGuiInputs() {
    guiInputs->clearWidgets();
    guiInputs->setColorBack(ofColor(60));
    guiInputs->setPosition(5, 96);
    guiInputs->addLabel("Manta features");
    guiInputs->addLabelToggle("all pads", &allPads);
    guiInputs->addLabelToggle("all sliders", &allSliders);
    guiInputs->addLabelToggle("all buttons", &allButtons);
    guiInputs->addLabelToggle("number fingers", &numFingers);
    guiInputs->addLabelToggle("pad sum", &padSum);
    guiInputs->addLabelToggle("pad average", &padAvg);
    guiInputs->addLabelToggle("perimeter", &perimeter);
    guiInputs->addLabelToggle("avg inter-finger dist", &avgInterDist);
    guiInputs->addLabelToggle("centroid", &centroid);
    guiInputs->addLabelToggle("weighted centroid", &wCentroid);
    guiInputs->addSpacer();
    guiInputs->addLabel("Velocity features");
    guiInputs->addLabelToggle("all pad velocities", &vAllPads);
    guiInputs->addLabelToggle("all slider velocities", &vAllSliders);
    guiInputs->addLabelToggle("pad sum velocity", &vPadSum);
    guiInputs->addLabelToggle("pad average velocity", &vPadAvg);
    guiInputs->addLabelToggle("perimeter velocity", &vPerimeter);
    guiInputs->addLabelToggle("avg inter-finger dist velocity", &vAvgInterDist);
    guiInputs->addLabelToggle("centroid velocity", &vCentroid);
    guiInputs->addLabelToggle("weighted centroid velocity", &vWCentroid);
    guiInputs->addSpacer();
    guiInputs->addLabelToggle("slider 0 value", false);
    guiInputs->addLabelToggle("slider 0 velocity", false);
    guiInputs->addLabelToggle("slider 1 value", false);
    guiInputs->addLabelToggle("slider 1 velocity", false);
    guiInputs->addSpacer();
    guiInputs->addDropDownList("Row", rows, 42.0f);
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiInputs->addDropDownList("Col", cols, 42.0f);
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    guiInputs->addLabel("Pad ")->getRect()->setWidth(56.0f);;
    if (guiRow >= 0 && guiCol >= 0) {
        guiInputs->addLabelToggle("pad value", padVal[guiRow][guiCol])->setLabelText("pad ("+ofToString(guiRow)+","+ofToString(guiCol)+") value");
        guiInputs->addLabelToggle("pad velocity", padVel[guiRow][guiCol])->setLabelText("pad ("+ofToString(guiRow)+","+ofToString(guiCol)+") velocity");
    }
    guiInputs->addSpacer();
    guiInputs->autoSizeToFitWidgets();
}


//-----------
void MantaLearn::guiEvent(ofxUIEventArgs &e) {
    if (e.getName() == "input selector") {
        guiInputs->setVisible(e.getButton()->getValue());
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
    else if (e.getName() == "pad value") {
        if (guiRow >= 0 && guiCol >= 0) {
            padVal[guiRow][guiCol] = e.getButton()->getValue();
            setupGuiInputs();
        }
    }
    else if (e.getName() == "pad velocity") {
        if (guiRow >= 0 && guiCol >= 0) {
            padVel[guiRow][guiCol] = e.getButton()->getValue();
            setupGuiInputs();
        }
    }
    else if (e.getName() == "slider 0 value") {
        sliderVal[0] = e.getButton()->getValue();
        setupGuiInputs();
    }
    else if (e.getName() == "slider 1 value") {
        sliderVal[1] = e.getButton()->getValue();
        setupGuiInputs();
    }
    else if (e.getName() == "slider 0 velocity") {
        sliderVel[0] = e.getButton()->getValue();
        setupGuiInputs();
    }
    else if (e.getName() == "slider 1 velocity") {
        sliderVel[1] = e.getButton()->getValue();
        setupGuiInputs();
    }
    else {
        if (e.getName()=="all pads" ||
            e.getName()=="all sliders" ||
            e.getName()=="all buttons" ||
            e.getName()=="number fingers" ||
            e.getName()=="pad sum" ||
            e.getName()=="pad average" ||
            e.getName()=="perimeter" ||
            e.getName()=="avg inter-finger dist" ||
            e.getName()=="centroid" ||
            e.getName()=="weighted centroid" ||
            e.getName()=="all pad velocities" ||
            e.getName()=="all slider velocities" ||
            e.getName()=="pad sum velocity" ||
            e.getName()=="pad average velocity" ||
            e.getName()=="perimeter velocity" ||
            e.getName()=="avg inter-finger dist velocity" ||
            e.getName()=="centroid velocity" ||
            e.getName()=="weighted centroid velocity") {
        }
        setupInputs();
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
MantaLearn::~MantaLearn() {
    setMantaVisible(false);
    ofRemoveListener(guiInputs->newGUIEvent, this, &MantaLearn::guiInputEvent);
    guiInputs->disable();
    delete guiInputs;
    manta.close();
}
