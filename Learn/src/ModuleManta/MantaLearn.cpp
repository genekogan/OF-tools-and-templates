#include "MantaLearn.h"


//-----------
MantaLearn::MantaLearn() : Learn() {
    manta.setup();
    setGuiInputsVisible(false);
    
    mantaVisible = true;
    setMantaVisible(mantaVisible);
    
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
    
    for (int i=0; i<6; i++) {
        rows.push_back(ofToString(i+1));
    }
    for (int i=0; i<8; i++) {
        cols.push_back(ofToString(i+1));
    }
    for (int r=0; r<6; r++) {
        for (int c=0; c<8; c++) {
            customPads.push_back("pad ("+ofToString(r+1)+","+ofToString(c+1)+")");
        }
    }
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
void MantaLearn::addPerimterAsInput() {
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
void MantaLearn::addPerimterVelocityAsInput() {
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
void MantaLearn::setupGuiInputs() {
    guiInputs->clearWidgets();
    guiInputs->setColorBack(ofColor(60));
    guiInputs->setPosition(5, 96);
    
    guiInputs->addLabel("Select global inputs");
    guiInputs->addSpacer();
    guiInputs->addLabel("Manta features");
    guiInputs->addLabelButton("all pads", false);
    guiInputs->addLabelButton("all sliders", false);
    guiInputs->addLabelButton("all buttons", false);
    guiInputs->addLabelButton("number fingers", false);
    guiInputs->addLabelButton("pad sum", false);
    guiInputs->addLabelButton("pad average", false);
    guiInputs->addLabelButton("perimeter", false);
    guiInputs->addLabelButton("avg inter-finger dist", false);
    guiInputs->addLabelButton("centroid", false);
    guiInputs->addLabelButton("weighted centroid", false);
    guiInputs->addSpacer();
    guiInputs->addLabel("Velocity features");
    guiInputs->addLabelButton("all pad velocities", false);
    guiInputs->addLabelButton("all slider velocities", false);
    guiInputs->addLabelButton("pad sum velocity", false);
    guiInputs->addLabelButton("pad average velocity", false);
    guiInputs->addLabelButton("perimeter velocity", false);
    guiInputs->addLabelButton("avg inter-finger dist velocity", false);
    guiInputs->addLabelButton("centroid velocity", false);
    guiInputs->addLabelButton("weighted centroid velocity", false);
    guiInputs->addSpacer();
    
    
    guiInputs->addLabel("Pad ")->getRect()->setWidth(56.0f);;
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    
    
    guiInputs->addDropDownList("Row", rows, 42.0f);
    guiInputs->addDropDownList("Col", cols, 42.0f);
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    guiInputs->addLabelButton("pad value", false);
    guiInputs->addLabelButton("pad velocity", false);

    guiInputs->addSpacer();
    guiInputs->addLabelButton("slider 0 value", false);
    guiInputs->addLabelButton("slider 0 velocity", false);
    guiInputs->addLabelButton("slider 1 value", false);
    guiInputs->addLabelButton("slider 1 velocity", false);
    guiInputs->addSpacer();
    
    
    /*
    if (guiMantaActiveType == 0) {
        guiInputs->addLabel("Pad "+ofToString(guiMantaActive));
        guiInputs->addLabelButton("pad value => input", false);
        guiInputs->addLabelButton("pad velocity => input", false);
    }
    else if (guiMantaActiveType == 1) {
        guiInputs->addLabel("Slider "+ofToString(guiMantaActive));
        guiInputs->addLabelButton("slider value => input", false);
        guiInputs->addLabelButton("slider velocity => input", false);
    }
    else if (guiMantaActiveType == 2) {
        guiInputs->addLabel("Button "+ofToString(guiMantaActive));
        guiInputs->addLabelButton("button value => input", false);
    }
     */
    
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
    if (e.getButton()->getValue() == 1) return;
    
    if      (e.getName() == "all pads")  addAllPadsAsInput();
    else if (e.getName() == "all sliders")  addSlidersAsInput();
    else if (e.getName() == "all buttons")  addButtonsAsInput();
    else if (e.getName() == "number fingers")  addNumFingersAsInput();
    else if (e.getName() == "pad sum")  addPadSumAsInput();
    else if (e.getName() == "pad average")  addPadAverageAsInput();
    else if (e.getName() == "perimeter")  addPerimterAsInput();
    else if (e.getName() == "avg inter-finger dist")  addAverageInterFingerDistanceAsInput();
    else if (e.getName() == "centroid")  addCentroidAsInput();
    else if (e.getName() == "weighted centroid")  addWeightedCentroidAsInput();
    else if (e.getName() == "all pad velocities")  addAllPadVelocitiesAsInput();
    else if (e.getName() == "all slider velocities")  addSliderVelocitiesAsInput();
    else if (e.getName() == "pad sum velocity")  addPadSumVelocityAsInput();
    else if (e.getName() == "pad average velocity")  addPadAverageVelocityAsInput();
    else if (e.getName() == "perimeter velocity")  addPerimterVelocityAsInput();
    else if (e.getName() == "avg inter-finger dist velocity")  addAverageInterFingerDistanceVelocityAsInput();
    else if (e.getName() == "centroid velocity")  addCentroidVelocityAsInput();
    else if (e.getName() == "weighted centroid velocity")  addWeightedCentroidVelocityAsInput();

    
    
    /*
    else if (e.getName() == "pad value => input") {
        int col = guiMantaActive % 8;
        int row = guiMantaActive / 8;
        addParameterAsInput("mantaPad"+ofToString(col)+ofToString(row), addInput("mantaPad"+ofToString(col)+ofToString(row), manta.getPadVelocityRef(row, col), 0, 196));
    }
    else if (e.getName() == "pad velocity => input") {
        int col = guiMantaActive % 8;
        int row = guiMantaActive / 8;
        addParameterAsInput("mantaVelocity"+ofToString(col)+ofToString(row), addInput("mantaVelocity"+ofToString(col)+ofToString(row), manta.getPadVelocityRef(row, col), 0, 196));
    }
    
    
    
    else if (e.getName() == "slider value => input") {
        addParameterAsInput("slider"+ofToString(guiMantaActive), addInput("slider"+ofToString(guiMantaActive), manta.getSliderRef(guiMantaActive), 0, 196));
    }
    else if (e.getName() == "slider velocity => input") {
//        addParameterAsInput("sliderVelocity"+ofToString(guiMantaActive), addInput("mantaVelocity"+ofToString(guiMantaActive), manta.getSliderVelocityRef(guiMantaActive), 0, 196));
        
    }

    
    else if (e.getName() == "button value => input") {
        int col = guiMantaActive % 8;
        int row = guiMantaActive / 8;
        addParameterAsInput("button"+ofToString(guiMantaActive), addInput("button"+ofToString(guiMantaActive), manta.getButtonRef(guiMantaActive), 0, 196));
    }
    //guiInputs->setVisible(false);
     */
}

//-----------
void MantaLearn::padClickedEvent(int &e) {
    if (guiMantaActive == e && guiMantaActiveType == 0) {
        //guiInputs->setVisible(true);
    }
    else {
        guiMantaActive = e;
        guiMantaActiveType = 0;
        setupGuiInputs();
    }
}

//-----------
void MantaLearn::sliderClickedEvent(int &e) {
    if (guiMantaActive == e && guiMantaActiveType == 1) {
        //guiInputs->setVisible(true);
    }
    else {
        guiMantaActive = e;
        guiMantaActiveType = 1;
        setupGuiInputs();
    }
}

//-----------
void MantaLearn::buttonClickedEvent(int &e) {
    if (guiMantaActive == e && guiMantaActiveType == 2) {
        //guiInputs->setVisible(true);
    }
    else {
        guiMantaActive = e;
        guiMantaActiveType = 2;
        setupGuiInputs();
    }
}

//-----------
void MantaLearn::setMantaVisible(bool mantaVisible) {
    this->mantaVisible = mantaVisible;
    manta.setVisible(mantaVisible);
    if (mantaVisible) {
        manta.addPadClickListener(this, &MantaLearn::padClickedEvent);
        manta.addSliderClickListener(this, &MantaLearn::sliderClickedEvent);
        manta.addButtonClickListener(this, &MantaLearn::buttonClickedEvent);
    }
    else {
        manta.removePadClickListener(this, &MantaLearn::padClickedEvent);
        manta.removeSliderClickListener(this, &MantaLearn::sliderClickedEvent);
        manta.removeButtonClickListener(this, &MantaLearn::buttonClickedEvent);
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
