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
    vector<LearnInputParameter*> newInputs;
    for (int row=0; row<6; row++) {
        for (int col=0; col<8; col++) {
            newInputs.push_back(Learn::addInput("manta"+ofToString(row)+ofToString(col), manta.getPadRef(row, col), 0, 196, true));
            *manta.getPadRef(row, col) = 0;
        }
    }
    addParametersAsInput("all 48 pads", newInputs);
}

//-----------
void MantaLearn::addSlidersAsInput() {
    vector<LearnInputParameter*> newInputs;
    newInputs.push_back(Learn::addInput("slider0", manta.getSliderRef(0), 0, 1, true));
    newInputs.push_back(Learn::addInput("slider1", manta.getSliderRef(1), 0, 1, true));
    addParametersAsInput("sliders", newInputs);
    *manta.getSliderRef(0) = 0;
    *manta.getSliderRef(1) = 0;
}

//-----------
void MantaLearn::addButtonsAsInput() {
    vector<LearnInputParameter*> newInputs;
    newInputs.push_back(Learn::addInput("button0", manta.getButtonRef(0), 0, 1, true));
    newInputs.push_back(Learn::addInput("button1", manta.getButtonRef(1), 0, 1, true));
    newInputs.push_back(Learn::addInput("button2", manta.getButtonRef(2), 0, 1, true));
    newInputs.push_back(Learn::addInput("button3", manta.getButtonRef(3), 0, 1, true));
    addParametersAsInput("buttons", newInputs);
    *manta.getButtonRef(0) = 0;
    *manta.getButtonRef(1) = 0;
    *manta.getButtonRef(2) = 0;
    *manta.getButtonRef(3) = 0;
}

//-----------
void MantaLearn::addNumPadsAsInput() {
    addParameterAsInput("num pads", Learn::addInput("num pads", &manta.getNumPads(), 0, 48, true));
}

//-----------
void MantaLearn::addPadSumAsInput() {
    addParameterAsInput("pad sum", Learn::addInput("pad sum", &manta.getPadSum(), 0, 3000, true));
}

//-----------
void MantaLearn::addPadAverageAsInput() {
    addParameterAsInput("pad avg", Learn::addInput("pad avg", &manta.getPadAverage(), 0, 196, true));
}

//-----------
void MantaLearn::addPerimeterAsInput() {
    addParameterAsInput("perimeter", Learn::addInput("perimeter", &manta.getPerimeter(), 0, 3, true));
}

//-----------
void MantaLearn::addPadWidthAsInput() {
    addParameterAsInput("width", Learn::addInput("width", &manta.getWidth(), 0, 1, true));
}

//-----------
void MantaLearn::addPadHeightAsInput() {
    addParameterAsInput("height", Learn::addInput("height", &manta.getHeight(), 0, 1, true));
}

//-----------
void MantaLearn::addPadWHRatioAsInput() {
    addParameterAsInput("width height ratio", Learn::addInput("width height ratio", &manta.getWidthHeightRatio(), 0, 10, true));
}

//-----------
void MantaLearn::addAverageInterFingerDistanceAsInput() {
    addParameterAsInput("avg finger distance", Learn::addInput("avg finger distance", &manta.getAverageInterFingerDistance(), 0, 2, true));
}

//-----------
void MantaLearn::addCentroidAsInput() {
    vector<LearnInputParameter*> newInputs;
    newInputs.push_back(Learn::addInput("centroid x", &manta.getCentroidX(), 0, 1, true));
    newInputs.push_back(Learn::addInput("centroid y", &manta.getCentroidY(), 0, 1, true));
    addParametersAsInput("centroid", newInputs);
}

//-----------
void MantaLearn::addWeightedCentroidAsInput() {
    vector<LearnInputParameter*> newInputs;
    newInputs.push_back(Learn::addInput("weighted centroid x", &manta.getWeightedCentroidX(), 0, 1, true));
    newInputs.push_back(Learn::addInput("weighted centroid y", &manta.getWeightedCentroidY(), 0, 1, true));
    addParametersAsInput("weighted centroid", newInputs);
}


//-----------
void MantaLearn::addAllPadVelocitiesAsInput() {
    vector<LearnInputParameter*> newInputs;
    for (int row=0; row<6; row++) {
        for (int col=0; col<8; col++) {
            newInputs.push_back(Learn::addInput("mantaVelocity"+ofToString(row)+ofToString(col), manta.getPadVelocityRef(row, col), -20, 20, true));
        }
    }
    addParametersAsInput("all 48 pad velocities", newInputs);
}

//-----------
void MantaLearn::addSliderVelocitiesAsInput() {
    vector<LearnInputParameter*> newInputs;
    newInputs.push_back(Learn::addInput("sliderVelocity0", manta.getSliderVelocityRef(0), -0.1, 0.1, true));
    newInputs.push_back(Learn::addInput("sliderVelocity1", manta.getSliderVelocityRef(1), -0.1, 0.1, true));
    addParametersAsInput("slider velocities", newInputs);
}

//-----------
void MantaLearn::addButtonVelocitiesAsInput() {
    vector<LearnInputParameter*> newInputs;
    newInputs.push_back(Learn::addInput("buttonVelocity0", manta.getButtonVelocityRef(0), -0.1, 0.1, true));
    newInputs.push_back(Learn::addInput("buttonVelocity1", manta.getButtonVelocityRef(1), -0.1, 0.1, true));
    newInputs.push_back(Learn::addInput("buttonVelocity2", manta.getButtonVelocityRef(2), -0.1, 0.1, true));
    newInputs.push_back(Learn::addInput("buttonVelocity3", manta.getButtonVelocityRef(3), -0.1, 0.1, true));
    addParametersAsInput("button velocities", newInputs);
}

//-----------
void MantaLearn::addNumPadsVelocityAsInput() {
    addParameterAsInput("num pads velocity", Learn::addInput("num pads velocity", &manta.getNumPadsVelocity(), -1, 1, true));
}

//-----------
void MantaLearn::addPadSumVelocityAsInput() {
    addParameterAsInput("pad sum velocity", Learn::addInput("pad sum velocity", &manta.getPadSumVelocity(), -200, 200, true));
}

//-----------
void MantaLearn::addPadAverageVelocityAsInput() {
    addParameterAsInput("pad avg velocity", Learn::addInput("pad avg velocity", &manta.getPadAverageVelocity(), -30, 30, true));
}

//-----------
void MantaLearn::addPerimeterVelocityAsInput() {
    addParameterAsInput("perimeter velocity", Learn::addInput("perimeter velocity", &manta.getPerimeterVelocity(), -1, 1, true));
}

//-----------
void MantaLearn::addPadWidthVelocityAsInput() {
    addParameterAsInput("width velocity", Learn::addInput("width velocity", &manta.getWidthVelocity(), -0.1, 0.1, true));
}

//-----------
void MantaLearn::addPadHeightVelocityAsInput() {
    addParameterAsInput("height velocity", Learn::addInput("height velocity", &manta.getHeightVelocity(), -0.1, 0.1));
}

//-----------
void MantaLearn::addPadWHRatioVelocityAsInput() {
    addParameterAsInput("width height ratio velocity", Learn::addInput("width height ratio velocity", &manta.getWidthHeightRatioVelocity(), -0.2, 0.2, true));
}

//-----------
void MantaLearn::addAverageInterFingerDistanceVelocityAsInput() {
    addParameterAsInput("avg finger distance velocity", Learn::addInput("avg finger distance velocity", &manta.getAverageInterFingerDistanceVelocity(), -0.1, 0.1, true));
}

//-----------
void MantaLearn::addCentroidVelocityAsInput() {
    vector<LearnInputParameter*> newInputs;
    newInputs.push_back(Learn::addInput("centroid x velocity", &manta.getCentroidVelocityX(), -0.1, 0.1, true));
    newInputs.push_back(Learn::addInput("centroid y velocity", &manta.getCentroidVelocityY(), -0.1, 0.1, true));
    addParametersAsInput("centroid velocity", newInputs);
}

//-----------
void MantaLearn::addWeightedCentroidVelocityAsInput() {
    vector<LearnInputParameter*> newInputs;
    newInputs.push_back(Learn::addInput("weighted centroid x velocity", &manta.getWeightedCentroidVelocityX(), -0.1, 0.1, true));
    newInputs.push_back(Learn::addInput("weighted centroid y velocity", &manta.getWeightedCentroidVelocityY(), -0.1, 0.1, true));
    addParametersAsInput("weighted centroid velocity", newInputs);
}

//-----------
void MantaLearn::setupInputs() {
    clearInputs();
    
    if (allPads)  addAllPadsAsInput();
    if (allSliders)  addSlidersAsInput();
    if (allButtons)  addButtonsAsInput();
    if (numPads)  addNumPadsAsInput();
    if (padSum)  addPadSumAsInput();
    if (padAvg)  addPadAverageAsInput();
    if (perimeter)  addPerimeterAsInput();
    if (width)  addPadWidthAsInput();
    if (height)  addPadHeightAsInput();
    if (whRatio)  addPadWHRatioAsInput();
    if (avgInterDist)  addAverageInterFingerDistanceAsInput();
    if (centroid)  addCentroidAsInput();
    if (wCentroid)  addWeightedCentroidAsInput();
    if (vAllPads)  addAllPadVelocitiesAsInput();
    if (vAllSliders)  addSliderVelocitiesAsInput();
    if (vAllButtons)  addButtonVelocitiesAsInput();
    if (vNumPads)  addNumPadsVelocityAsInput();
    if (vPadSum)  addPadSumVelocityAsInput();
    if (vPadAvg)  addPadAverageVelocityAsInput();
    if (vPerimeter)  addPerimeterVelocityAsInput();
    if (vWidth)  addPadWidthVelocityAsInput();
    if (vHeight)  addPadHeightVelocityAsInput();
    if (vWhRatio)  addPadWHRatioVelocityAsInput();
    if (vAvgInterDist)  addAverageInterFingerDistanceVelocityAsInput();
    if (vCentroid)  addCentroidVelocityAsInput();
    if (vWCentroid)  addWeightedCentroidVelocityAsInput();
    
    for (int r=0; r<6; r++) {
        for (int c=0; c<8; c++) {
            if (padVal[r][c]) {
                addParameterAsInput("manta"+ofToString(r)+ofToString(c),
                                    Learn::addInput("manta"+ofToString(r)+ofToString(c),
                                             manta.getPadRef(r, c), 0, 196, true));
            }
            if (padVel[r][c]) {
                addParameterAsInput("mantaVelocity"+ofToString(r)+ofToString(c),
                                    Learn::addInput("mantaVelocity"+ofToString(r)+ofToString(c),
                                             manta.getPadVelocityRef(guiRow, guiCol), -20, 20, true));
            }
        }
    }
    for (int i=0; i<2; i++) {
        if (sliderVal[i])   addParameterAsInput("slider"+ofToString(i),
                                                Learn::addInput("slider"+ofToString(i),
                                                         manta.getSliderRef(i), 0, 1, true));
        if (sliderVel[i])   addParameterAsInput("sliderVelocity"+ofToString(i),
                                                Learn::addInput("sliderVelocity"+ofToString(i),
                                                         manta.getSliderVelocityRef(i), -0.1, 0.1, true));
    }
    for (int i=0; i<4; i++) {
        if (buttonVal[i])   addParameterAsInput("button"+ofToString(i),
                                                Learn::addInput("button"+ofToString(i),
                                                         manta.getButtonRef(i), 0, 1, true));
        if (buttonVel[i])   addParameterAsInput("buttonVelocity"+ofToString(i),
                                                Learn::addInput("buttonVelocity"+ofToString(i),
                                                         manta.getButtonVelocityRef(i), -0.1, 0.1, true));
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
    
    guiInputs->addLabelToggle("avg inter-finger dist", &avgInterDist, 140.0f);
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiInputs->addLabelToggle("avg inter-finger dist velocity", &vAvgInterDist, 50.0f)->setLabelText("velocity");
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
    guiInputs->addLabelToggle("slider 0 value", &sliderVal[0], 140.0f);
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiInputs->addLabelToggle("slider 0 velocity", &sliderVel[0], 50.0f)->setLabelText("velocity");
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    guiInputs->addLabelToggle("slider 1 value", &sliderVal[1], 140.0f);
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiInputs->addLabelToggle("slider 1 velocity", &sliderVel[1], 50.0f)->setLabelText("velocity");
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    guiInputs->addSpacer();
    guiInputs->addLabelToggle("button 0 value", &buttonVal[0], 140.0f);
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiInputs->addLabelToggle("button 0 velocity", &buttonVel[0], 50.0f)->setLabelText("velocity");
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    guiInputs->addLabelToggle("button 1 value", &buttonVal[1], 140.0f);
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiInputs->addLabelToggle("button 1 velocity", &buttonVel[1], 50.0f)->setLabelText("velocity");
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    guiInputs->addLabelToggle("button 2 value", &buttonVal[2], 140.0f);
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiInputs->addLabelToggle("button 2 velocity", &buttonVel[2], 50.0f)->setLabelText("velocity");
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    guiInputs->addLabelToggle("button 3 value", &buttonVal[3], 140.0f);
    guiInputs->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiInputs->addLabelToggle("button 3 velocity", &buttonVel[3], 50.0f)->setLabelText("velocity");
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
    else {
        if (e.getName()=="all pads" ||
            e.getName()=="all sliders" ||
            e.getName()=="all buttons" ||
            e.getName()=="number pads" ||
            e.getName()=="pad sum" ||
            e.getName()=="pad average" ||
            e.getName()=="perimeter" ||
            e.getName()=="width" ||
            e.getName()=="height" ||
            e.getName()=="width/height ratio" ||
            e.getName()=="avg inter-finger dist" ||
            e.getName()=="centroid" ||
            e.getName()=="weighted centroid" ||
            e.getName()=="all pad velocities" ||
            e.getName()=="all slider velocities" ||
            e.getName()=="all button velocities" ||
            e.getName()=="number pads velocity" ||
            e.getName()=="pad sum velocity" ||
            e.getName()=="pad average velocity" ||
            e.getName()=="perimeter velocity" ||
            e.getName()=="width velocity" ||
            e.getName()=="height velocity" ||
            e.getName()=="width/height ratio velocity" ||
            e.getName()=="avg inter-finger dist velocity" ||
            e.getName()=="centroid velocity" ||
            e.getName()=="weighted centroid velocity" ||
            e.getName()=="pad value" ||
            e.getName()=="pad velocity" ||
            e.getName()=="slider 0 value" ||
            e.getName()=="slider 0 velocity" ||
            e.getName()=="slider 1 value" ||
            e.getName()=="slider 1 velocity" ||
            e.getName()=="button 0 value" ||
            e.getName()=="button 0 velocity" ||
            e.getName()=="button 1 value" ||
            e.getName()=="button 1 velocity" ||
            e.getName()=="button 2 value" ||
            e.getName()=="button 2 velocity" ||
            e.getName()=="button 3 value" ||
            e.getName()=="button 3 velocity" ||
            e.getName()=="pad individual value" ||
            e.getName()=="pad individual velocity") {
            setupInputs();
        }
    }
}

//-----------
LearnInputParameter * MantaLearn::addInput(string name, float min, float max) {
    if      (name == "slider0")  return Learn::addInput(name, manta.getSliderRef(0), min, max);
    else if (name == "slider1")  return Learn::addInput(name, manta.getSliderRef(1), min, max);
    else if (name == "button0")  return Learn::addInput(name, manta.getButtonRef(0), min, max);
    else if (name == "button1")  return Learn::addInput(name, manta.getButtonRef(1), min, max);
    else if (name == "button2")  return Learn::addInput(name, manta.getButtonRef(2), min, max);
    else if (name == "button3")  return Learn::addInput(name, manta.getButtonRef(3), min, max);
    else if (name == "num pads")	return Learn::addInput(name, &manta.getNumPads(), min, max);
    else if (name == "pad sum")  return Learn::addInput(name, &manta.getPadSum(), min, max);
    else if (name == "pad avg")  return Learn::addInput(name, &manta.getPadAverage(), min, max);
    else if (name == "perimeter")  return Learn::addInput(name, &manta.getPerimeter(), min, max);
    else if (name == "width")  return Learn::addInput(name, &manta.getWidth(), min, max);
    else if (name == "height")  return Learn::addInput(name, &manta.getHeight(), min, max);
    else if (name == "width height ratio")  return Learn::addInput(name, &manta.getWidthHeightRatio(), min, max);
    else if (name == "avg finger distance")  return Learn::addInput(name, &manta.getAverageInterFingerDistance(), min, max);
    else if (name == "centroid x")  return Learn::addInput(name, &manta.getCentroidX(), min, max);
    else if (name == "centroid y")  return Learn::addInput(name, &manta.getCentroidY(), min, max);
    else if (name == "weighted centroid x")  return Learn::addInput(name, &manta.getWeightedCentroidX(), min, max);
    else if (name == "weighted centroid y")  return Learn::addInput(name, &manta.getWeightedCentroidY(), min, max);
    else if (name == "sliderVelocity0")  return Learn::addInput(name, manta.getSliderVelocityRef(0), min, max);
    else if (name == "sliderVelocity1")  return Learn::addInput(name, manta.getSliderVelocityRef(1), min, max);
    else if (name == "buttonVelocity0")  return Learn::addInput(name, manta.getButtonVelocityRef(0), min, max);
    else if (name == "buttonVelocity1")  return Learn::addInput(name, manta.getButtonVelocityRef(1), min, max);
    else if (name == "buttonVelocity2")  return Learn::addInput(name, manta.getButtonVelocityRef(2), min, max);
    else if (name == "buttonVelocity3")  return Learn::addInput(name, manta.getButtonVelocityRef(3), min, max);
    else if (name == "num pads velocity")  return Learn::addInput(name, &manta.getNumPadsVelocity(), min, max);
    else if (name == "pad sum velocity")  return Learn::addInput(name, &manta.getPadSumVelocity(), min, max);
    else if (name == "pad avg velocity")  return Learn::addInput(name, &manta.getPadAverageVelocity(), min, max);
    else if (name == "perimeter velocity")  return Learn::addInput(name, &manta.getPerimeterVelocity(), min, max);
    else if (name == "width velocity")  return Learn::addInput(name, &manta.getWidthVelocity(), min, max);
    else if (name == "height velocity")  return Learn::addInput(name, &manta.getHeightVelocity(), min, max);
    else if (name == "width height ratio velocity")  return Learn::addInput(name, &manta.getWidthHeightRatioVelocity(), min, max);
    else if (name == "avg finger distance velocity")  return Learn::addInput(name, &manta.getAverageInterFingerDistanceVelocity(), min, max);
    else if (name == "centroid x velocity")  return Learn::addInput(name, &manta.getCentroidVelocityX(), min, max);
    else if (name == "centroid y velocity")  return Learn::addInput(name, &manta.getCentroidVelocityY(), min, max);
    else if (name == "weighted centroid x velocity")  return Learn::addInput(name, &manta.getWeightedCentroidVelocityX(), min, max);
    else if (name == "weighted centroid y velocity")  return Learn::addInput(name, &manta.getWeightedCentroidVelocityY(), min, max);
    else {
        for (int r=0; r<6; r++) {
            for (int c=0; c<8; c++) {
                if (name == "manta"+ofToString(r)+ofToString(c)) {
                    return Learn::addInput(name, manta.getPadRef(r, c), min, max);
                }
                if (name == "mantaVelocity"+ofToString(r)+ofToString(c)) {
                    return Learn::addInput(name, manta.getPadVelocityRef(r, c), min, max);
                }
            }
        }
        for (int i=0; i<2; i++) {
            if (name == "slider"+ofToString(i)) {
                return Learn::addInput(name, manta.getSliderRef(i), min, max);
            }
            if (name == "sliderVelocity"+ofToString(i)) {
                return Learn::addInput(name, manta.getSliderVelocityRef(i), min, max);
            }
        }
        for (int i=0; i<4; i++) {
            if (name == "button"+ofToString(i)) {
                return Learn::addInput(name, manta.getButtonRef(i), min, max);
            }
            if (name == "buttonVelocity"+ofToString(i)) {
                return Learn::addInput(name, manta.getButtonVelocityRef(i), min, max);
            }
        }
        return Learn::addInput(name, new float(), min, max);
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
