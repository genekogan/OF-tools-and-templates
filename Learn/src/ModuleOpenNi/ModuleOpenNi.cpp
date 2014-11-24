#include "ModuleOpenNi.h"


//-----------
ModuleOpenNi::ModuleOpenNi() : Learn() {
    setGuiInputsVisible(false);
    x = 5;
    y = 65;
    visible = true;
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
void ModuleOpenNi::setGuiPosition(int x, int y) {
    this->x = x;
    this->y = y;
    openNi.setGuiPosition(x, y);
}

//-----------
void ModuleOpenNi::setVisible(bool visible) {
    this->visible = visible;
    Learn::setVisible(visible);
    openNi.setVisible(visible);
}

//-----------
void ModuleOpenNi::toggleSummaryView() {
    summary = !summary;
    openNi.setVisible(summary);
    Learn::setGuiSummaryView(summary);
}
