#include "ModuleOpenNi.h"


//-----------
ModuleOpenNi::ModuleOpenNi() : Learn() {
    x = 5;
    y = 65;
    guiInputs = new ofxUICanvas();
    visible = true;
    ofAddListener(guiInputs->newGUIEvent, this, &ModuleOpenNi::guiInputEvent);
    setGuiInputsVisible(false);
    setGuiPosition(x, y);
}

//-----------
void ModuleOpenNi::setup(string oniFile) {
    oniFile=="" ? openNi.setup() : openNi.setup(oniFile);
    openNi.enableUserTracking(1);
    openNi.setNormalizeJoints(true);
    openNi.setGuiPosition(x, y);
    setupGuiInputs();
    
    // global joint features
    addJointsAsInput();
    addNormalizedJointsAsInput();
    addRelativeJointsAsInput();
    addVelocityJointsAsInput();
    addAccelerationJointsAsInput();
    addRelativeDistanceJointsAsInput();
    addVelocityMagnitudeJointsAsInput();
    addVelocityMeanJointsAsInput();
    addAccelerationMagnitudeJointsAsInput();
    addAccelerationMeanJointsAsInput();
    addAccelerationTrajectoryJointsAsInput();
    
    // individual joints
    for (int j=0; j<openNi.getJointNames().size(); j++) {
        addJointAsInput(j);
        addNormalizedJointAsInput(j);
        addRelativeJointAsInput(j);
        addVelocityJointAsInput(j);
        addAccelerationJointAsInput(j);
        
        addRelativeDistanceJointAsInput(j);
        addVelocityMagnitudeJointAsInput(j);
        addVelocityMeanJointAsInput(j);
        addAccelerationMagnitudeJointAsInput(j);
        addAccelerationMeanJointAsInput(j);
        addAccelerationTrajectoryJointAsInput(j);
    }
    
    // stats
    addSymmetryAsInput();
    addQomAsInput();
    addCiAsInput();
    addDepthAsInput();
    addYMaxHandsAsInput();
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
    vector<InputFeature> features;
	features.push_back(InputFeature(&vec->x, min, max, descriptor+"_x"));
	features.push_back(InputFeature(&vec->y, min, max, descriptor+"_y"));
	features.push_back(InputFeature(&vec->z, min, max, descriptor+"_z"));
    inputFeatures[descriptor] = features;
}

//-----------
void ModuleOpenNi::addVectorVec3AsInput(string descriptor, string prefix, vector<ofVec3f *> *vec, float min, float max) {
    vector<InputFeature> features;
    for (int i=0; i<vec->size(); i++) {
        features.push_back(InputFeature(&vec->at(i)->x, min, max, prefix+openNi.getJointName(i)+"_x"));
        features.push_back(InputFeature(&vec->at(i)->y, min, max, prefix+openNi.getJointName(i)+"_y"));
        features.push_back(InputFeature(&vec->at(i)->z, min, max, prefix+openNi.getJointName(i)+"_z"));
    }
    inputFeatures[descriptor] = features;
}

//-----------
void ModuleOpenNi::addVectorFloatAsInput(string descriptor, string prefix, vector<float *> *vec, float min, float max) {
    vector<InputFeature> features;
    for (int i=0; i<vec->size(); i++) {
        features.push_back(InputFeature(vec->at(i), min, max, prefix+openNi.getJointName(i)));
    }
    inputFeatures[descriptor] = features;
}

//-----------
void ModuleOpenNi::addIndividualAsInput(string descriptor, float *value, float min, float max) {
    addSingleInputFeature(descriptor, value, min, max);
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
    addVectorFloatAsInput("accel mag joints", "ag_", &openNi.getAccelerationMagnitudeJoints(0), -10, 10);
}
void ModuleOpenNi::addAccelerationMeanJointsAsInput() {
    addVectorFloatAsInput("accel mean joints", "am_", &openNi.getAccelerationMeanJoints(0), -10, 10);
}
void ModuleOpenNi::addAccelerationTrajectoryJointsAsInput() {
    addVectorFloatAsInput("accel trajectory joints", "at_", &openNi.getAccelerationTrajectoryJoints(0), -10, 10);
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
    addIndividualAsInput("quantity motion", openNi.getQom(0), 0, 100);
}
void ModuleOpenNi::addCiAsInput() {
    addIndividualAsInput("contraction index", openNi.getCi(0), 0, 1);
}
void ModuleOpenNi::addDepthAsInput() {
    addIndividualAsInput("depth", openNi.getDepth(0), 0, 100);
}
void ModuleOpenNi::addYMaxHandsAsInput() {
    addIndividualAsInput("ymaxhands", openNi.getYMaxHands(0), 0, 1);
}

//-----------
void ModuleOpenNi::setupGuiInputs() {
    vector<string> globalFeatures;
    globalFeatures.push_back("symmetry");
    globalFeatures.push_back("quantity motion");
    globalFeatures.push_back("contraction index");
    globalFeatures.push_back("depth");
    globalFeatures.push_back("ymaxhands");
    
    vector<string> allJointFeatures;
    allJointFeatures.push_back("joints");
    allJointFeatures.push_back("joints norm");
    allJointFeatures.push_back("relative joints");
    allJointFeatures.push_back("velocity joints");
    allJointFeatures.push_back("accel joints");
    allJointFeatures.push_back("rel dist joints");
    allJointFeatures.push_back("vel mag joints");
    allJointFeatures.push_back("vel mean joints");
    allJointFeatures.push_back("accel mag joints");
    allJointFeatures.push_back("accel mean joints");
    allJointFeatures.push_back("accel trajectory joints");
    
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
    guiInputs->setPosition(x, y+160);
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
    if (inputFeatures.count(e.getName()) != 0) {
        e.getButton()->getValue() ? addInputFeatureSet(e.getName()) : Learn::removeInputGroup(e.getName());
    }
    else if (e.getName() == "add custom inputs") {
        if (e.getButton()->getValue())  return;
        vector<ofxUILabelToggle*> jointToggles = guiJointSelector->getToggles();
        vector<ofxUILabelToggle*> statToggles = guiStatSelector->getToggles();
        for (int j=0; j<jointToggles.size(); j++) {
            if (jointToggles[j]->getValue()) {
                statToggles[ 0]->getValue() ? addInputFeatureSet(openNi.getJointName(j)+" pos") : Learn::removeInputGroup(openNi.getJointName(j)+" pos");
                statToggles[ 1]->getValue() ? addInputFeatureSet(openNi.getJointName(j)+" pos norm") : Learn::removeInputGroup(openNi.getJointName(j)+" pos norm");
                statToggles[ 2]->getValue() ? addInputFeatureSet(openNi.getJointName(j)+" pos relative") : Learn::removeInputGroup(openNi.getJointName(j)+" pos relative");
                statToggles[ 3]->getValue() ? addInputFeatureSet(openNi.getJointName(j)+" velocity") : Learn::removeInputGroup(openNi.getJointName(j)+" velocity");
                statToggles[ 4]->getValue() ? addInputFeatureSet(openNi.getJointName(j)+" accel") : Learn::removeInputGroup(openNi.getJointName(j)+" accel");
                statToggles[ 5]->getValue() ? addInputFeatureSet(openNi.getJointName(j)+" relative dist") : Learn::removeInputGroup(openNi.getJointName(j)+" relative dist");
                statToggles[ 6]->getValue() ? addInputFeatureSet(openNi.getJointName(j)+" velocity mag") : Learn::removeInputGroup(openNi.getJointName(j)+" velocity mag");
                statToggles[ 7]->getValue() ? addInputFeatureSet(openNi.getJointName(j)+" velocity mean") : Learn::removeInputGroup(openNi.getJointName(j)+" velocity mean");
                statToggles[ 8]->getValue() ? addInputFeatureSet(openNi.getJointName(j)+" accel mag") : Learn::removeInputGroup(openNi.getJointName(j)+" accel mag");
                statToggles[ 9]->getValue() ? addInputFeatureSet(openNi.getJointName(j)+" accel mean") : Learn::removeInputGroup(openNi.getJointName(j)+" accel mean");
                statToggles[10]->getValue() ? addInputFeatureSet(openNi.getJointName(j)+" accel trajectory") : Learn::removeInputGroup(openNi.getJointName(j)+" accel trajectory");
            }
        }
    }
}

//-----------
void ModuleOpenNi::setGuiPosition(int x, int y) {
    this->x = x;
    this->y = y;
    openNi.setGuiPosition(x, y);
    guiInputs->setPosition(x, y+160);
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
