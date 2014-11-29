#include "ModuleOpenNi.h"


//-----------
ModuleOpenNi::ModuleOpenNi() : Learn() {
    setGuiInputsVisible(false);
    x = 5;
    y = 65;
    visible = true;
    
    guiInputs = new ofxUICanvas();
    ofAddListener(guiInputs->newGUIEvent, this, &ModuleOpenNi::guiInputEvent);
}

//-----------
void ModuleOpenNi::setup(string oni) {
    if (oni=="") {
        openNi.setup();
    }
    else {
        openNi.setup(oni);
    }
    openNi.enableUserTracking(1);
    openNi.setNormalizeJoints(true);
    openNi.setGuiPosition(x, y);
    setupGuiInputs();
}

//-----------
void ModuleOpenNi::update() {
    openNi.update();
    Learn::update();
}

//-----------
void ModuleOpenNi::draw() {
    if (visible) {
        Learn::draw();
        openNi.draw();
    }
}

//-----------
void ModuleOpenNi::addVec3AsInput(string descriptor, ofVec3f *vec, float min, float max) {
    vector<LearnInputParameter*> newInputs;
    newInputs.push_back(addInput(descriptor+"_x", &vec->x, min, max));
    newInputs.push_back(addInput(descriptor+"_y", &vec->y, min, max));
    newInputs.push_back(addInput(descriptor+"_z", &vec->z, min, max));
    addParametersAsInput(descriptor, newInputs);
}

//-----------
void ModuleOpenNi::addVectorVec3AsInput(string descriptor, string prefix, vector<ofVec3f *> *vec, float min, float max) {
    vector<LearnInputParameter*> newInputs;
    for (int i=0; i<vec->size(); i++) {
        newInputs.push_back(addInput(prefix+openNi.getJointName(i)+"_x", &vec->at(i)->x, min, max));
        newInputs.push_back(addInput(prefix+openNi.getJointName(i)+"_y", &vec->at(i)->y, min, max));
        newInputs.push_back(addInput(prefix+openNi.getJointName(i)+"_z", &vec->at(i)->z, min, max));
    }
    addParametersAsInput(descriptor, newInputs);
}

//-----------
void ModuleOpenNi::addVectorFloatAsInput(string descriptor, string prefix, vector<float *> *vec, float min, float max) {
    vector<LearnInputParameter*> newInputs;
    for (int i=0; i<vec->size(); i++) {
        newInputs.push_back(addInput(prefix+openNi.getJointName(i), vec->at(i), min, max));
    }
    addParametersAsInput(descriptor, newInputs);
}

//-----------
void ModuleOpenNi::addIndividualAsInput(string descriptor, float *value, float min, float max) {
    vector<LearnInputParameter*> newInput;
    newInput.push_back(addInput(descriptor, value, min, max));
    addParametersAsInput(descriptor, newInput);
}

// collective
//-----------
void ModuleOpenNi::addJointsAsInput() {
    addVectorVec3AsInput("joints", "p_", &openNi.getJoints(0), -1000, 1000);
}
void ModuleOpenNi::addNormalizedJointsAsInput() {
    addVectorVec3AsInput("joints norm", "n_", &openNi.getNormalizedJoints(0), 0, 1);
}
void ModuleOpenNi::addRelativeJointsAsInput() {
    addVectorVec3AsInput("relative joints", "r_", &openNi.getRelativeJoints(0), -1, 1);
}
void ModuleOpenNi::addVelocityJointsAsInput() {
    addVectorVec3AsInput("velocity joints", "v_", &openNi.getVelocityJoints(0), -50, 50);
}
void ModuleOpenNi::addAccelerationJointsAsInput() {
    addVectorVec3AsInput("accel joints", "a_", &openNi.getAccelerationJoints(0), -50, 50);
}
//-----------
void ModuleOpenNi::addRelativeDistanceJointsAsInput() {
    addVectorFloatAsInput("rel dist joints", "rd_", &openNi.getRelativeDistanceJoints(0), 0, 1000000);
}
void ModuleOpenNi::addVelocityMagnitudeJointsAsInput() {
    addVectorFloatAsInput("vel mag joints", "vg_", &openNi.getVelocityMagnitudeJoints(0), 0, 100);
}
void ModuleOpenNi::addVelocityMeanJointsAsInput() {
    addVectorFloatAsInput("vel mean joints", "vm_", &openNi.getVelocityMeanJoints(0), 0, 100);
}
void ModuleOpenNi::addAccelerationMagnitudeJointsAsInput() {
    addVectorFloatAsInput("acc mag joints", "ag_", &openNi.getAccelerationMagnitudeJoints(0), -10, 10);
}
void ModuleOpenNi::addAccelerationMeanJointsAsInput() {
    addVectorFloatAsInput("acc mean joints", "am_", &openNi.getAccelerationMeanJoints(0), -10, 10);
}
void ModuleOpenNi::addAccelerationTrajectoryJointsAsInput() {
    addVectorFloatAsInput("acc traj joints", "at_", &openNi.getAccelerationTrajectoryJoints(0), -10, 10);
}

// individual
//-----------
void ModuleOpenNi::addJointAsInput(int joint) {
    addVec3AsInput(openNi.getJointName(joint)+" pos", openNi.getJoint(0, joint), -1000, 1000);
}
void ModuleOpenNi::addNormalizedJointAsInput(int joint) {
    addVec3AsInput(openNi.getJointName(joint)+" pos norm", openNi.getNormalizedJoint(0, joint), 0, 1);
}
void ModuleOpenNi::addRelativeJointAsInput(int joint) {
    addVec3AsInput(openNi.getJointName(joint)+" pos relative", openNi.getRelativeJoint(0, joint), -1, 1);
}
void ModuleOpenNi::addVelocityJointAsInput(int joint) {
    addVec3AsInput(openNi.getJointName(joint)+" velocity", openNi.getVelocityJoint(0, joint), -50, 50);
}
void ModuleOpenNi::addAccelerationJointAsInput(int joint) {
    addVec3AsInput(openNi.getJointName(joint)+" accel", openNi.getAccelerationJoint(0, joint), -50, 50);
}
//-----------
void ModuleOpenNi::addRelativeDistanceJointAsInput(int joint) {
    addIndividualAsInput(openNi.getJointName(joint)+" relative dist", openNi.getRelativeDistanceJoint(0, joint), 0, 1000000);
}
void ModuleOpenNi::addVelocityMagnitudeJointAsInput(int joint) {
    addIndividualAsInput(openNi.getJointName(joint)+" velocity mag", openNi.getVelocityMagnitudeJoint(0, joint), 0, 100);
}
void ModuleOpenNi::addVelocityMeanJointAsInput(int joint) {
    addIndividualAsInput(openNi.getJointName(joint)+" velocity mean", openNi.getVelocityMeanJoint(0, joint), 0, 100);
}
void ModuleOpenNi::addAccelerationMagnitudeJointAsInput(int joint) {
    addIndividualAsInput(openNi.getJointName(joint)+" accel mag", openNi.getAccelerationMagnitudeJoint(0, joint), -10, 10);
}
void ModuleOpenNi::addAccelerationMeanJointAsInput(int joint) {
    addIndividualAsInput(openNi.getJointName(joint)+" accel mean", openNi.getAccelerationMeanJoint(0, joint), -10, 10);
}
void ModuleOpenNi::addAccelerationTrajectoryJointAsInput(int joint) {
    addIndividualAsInput(openNi.getJointName(joint)+" accel trajectory", openNi.getAccelerationTrajectoryJoint(0, joint), -10, 10);
}

// stats
//-----------
void ModuleOpenNi::addSymmetryAsInput() {
    addIndividualAsInput("symmetry", openNi.getSymmetry(0), 0, 1);
}
void ModuleOpenNi::addQomAsInput() {
    addIndividualAsInput("qom", openNi.getQom(0), 0, 100);
}
void ModuleOpenNi::addCiAsInput() {
    addIndividualAsInput("ci", openNi.getCi(0), 0, 1);
}
void ModuleOpenNi::addDepthAsInput() {
    addIndividualAsInput("depth", openNi.getDepth(0), 0, 100);
}
void ModuleOpenNi::addYMaxHandsAsInput() {
    addIndividualAsInput("ymax", openNi.getYMaxHands(0), 0, 1);
}

//-----------
void ModuleOpenNi::setupGuiInputs() {
    vector<string> globalFeatures;
    globalFeatures.push_back("SymmetryAsInput");
    globalFeatures.push_back("Quantity of Motion");
    globalFeatures.push_back("Contraction index");
    globalFeatures.push_back("Depth");
    globalFeatures.push_back("YMaxHands");
    
    vector<string> allJointFeatures;
    allJointFeatures.push_back("Raw Joints");
    allJointFeatures.push_back("Normalized Joints");
    allJointFeatures.push_back("Relative Joints");
    allJointFeatures.push_back("Velocity Joints");
    allJointFeatures.push_back("Acceleration Joints");
    allJointFeatures.push_back("RelativeDistance Joints");
    allJointFeatures.push_back("VelocityMagnitude Joints");
    allJointFeatures.push_back("VelocityMean Joints");
    allJointFeatures.push_back("AccelerationMagnitude Joints");
    allJointFeatures.push_back("AccelerationMean Joints");
    allJointFeatures.push_back("AccelerationTrajectory Joints");
    
    vector<string> jointStats;
    jointStats.push_back("position raw (3)");
    jointStats.push_back("position norm (3)");
    jointStats.push_back("position relative (3)");
    jointStats.push_back("velocity (3)");
    jointStats.push_back("accel (3)");
    jointStats.push_back("distance relative (1)");
    jointStats.push_back("velocity mag (1)");
    jointStats.push_back("velocity mean (1)");
    jointStats.push_back("accel mag (1)");
    jointStats.push_back("accel mean (1)");
    jointStats.push_back("accel trajectory (1)");
    
    guiInputs->clearWidgets();
    guiInputs->setPosition(x+420, y);
    guiInputs->addLabel("Global stats");
    guiGloalStatSelector = guiInputs->addDropDownList("select global stats", globalFeatures);
    guiGloalStatSelector->setAutoClose(true);
    guiInputs->addSpacer();
    guiInputs->addLabel("Features for all joints");
    guiJointsFeatures = guiInputs->addDropDownList("select all joint features", allJointFeatures);
    guiJointsFeatures->setAutoClose(true);
    guiInputs->addSpacer();
    guiInputs->addLabel("Custom inputs");
    guiInputs->addLabelButton("add custom inputs", false);
    guiJointSelector = guiInputs->addDropDownList("select joint", openNi.getJointNames());
    guiStatSelector = guiInputs->addDropDownList("joint stat", jointStats);
    guiJointSelector->setAllowMultiple(true);
    guiStatSelector->setAllowMultiple(true);
}

//-----------
void ModuleOpenNi::guiInputEvent(ofxUIEventArgs &e) {
    if 		(e.getName() == "Raw Joints")	addJointsAsInput();
    else if (e.getName() == "Normalized Joints")	addNormalizedJointsAsInput();
    else if (e.getName() == "Relative Joints")	addRelativeJointsAsInput();
    else if (e.getName() == "Velocity Joints")	addVelocityJointsAsInput();
    else if (e.getName() == "Acceleration Joints")	addAccelerationJointsAsInput();
    else if (e.getName() == "RelativeDistance Joints")	addRelativeDistanceJointsAsInput();
    else if (e.getName() == "VelocityMagnitude Joints")	addVelocityMagnitudeJointsAsInput();
    else if (e.getName() == "VelocityMean Joints")	addVelocityMeanJointsAsInput();
    else if (e.getName() == "AccelerationMagnitude Joints")	addAccelerationMagnitudeJointsAsInput();
    else if (e.getName() == "AccelerationMean Joints")	addAccelerationMeanJointsAsInput();
    else if (e.getName() == "AccelerationTrajectory Joints")	addAccelerationTrajectoryJointsAsInput();

    else if (e.getName() == "SymmetryAsInput")	addSymmetryAsInput();
    else if (e.getName() == "Quantity of Motion")	addQomAsInput();
    else if (e.getName() == "Contraction index")	addCiAsInput();
    else if (e.getName() == "Depth")	addDepthAsInput();
    else if (e.getName() == "YMaxHands")	addYMaxHandsAsInput();

    else if (e.getName() == "add custom inputs") {
        if (e.getButton()->getValue())  return;
        vector<ofxUILabelToggle*> jointToggles = guiJointSelector->getToggles();
        vector<ofxUILabelToggle*> statToggles = guiStatSelector->getToggles();
        for (int j=0; j<jointToggles.size(); j++) {
            if (jointToggles[j]->getValue()) {
                if (statToggles[ 0]->getValue()) addJointAsInput(j);
                if (statToggles[ 1]->getValue()) addNormalizedJointAsInput(j);
                if (statToggles[ 2]->getValue()) addRelativeJointAsInput(j);
                if (statToggles[ 3]->getValue()) addVelocityJointAsInput(j);
                if (statToggles[ 4]->getValue()) addAccelerationJointAsInput(j);
                if (statToggles[ 5]->getValue()) addRelativeDistanceJointAsInput(j);
                if (statToggles[ 6]->getValue()) addVelocityMagnitudeJointAsInput(j);
                if (statToggles[ 7]->getValue()) addVelocityMeanJointAsInput(j);
                if (statToggles[ 8]->getValue()) addAccelerationMagnitudeJointAsInput(j);
                if (statToggles[ 9]->getValue()) addAccelerationMeanJointAsInput(j);
                if (statToggles[10]->getValue()) addAccelerationTrajectoryJointAsInput(j);
            }
        }
    }
}

//-----------
void ModuleOpenNi::setGuiPosition(int x, int y) {
    this->x = x;
    this->y = y;
    openNi.setGuiPosition(x, y);
    guiInputs->setPosition(x+420, y);
}

//-----------
void ModuleOpenNi::setVisible(bool visible) {
    this->visible = visible;
    Learn::setVisible(visible);
    openNi.setVisible(visible);
    guiInputs->setVisible(visible);
}

//-----------
void ModuleOpenNi::toggleGuiSummaryView() {
    Learn::setGuiSummaryView(!summaryVisible);
    guiInputs->setVisible(summaryVisible);
    openNi.setVisible(summaryVisible);
}
