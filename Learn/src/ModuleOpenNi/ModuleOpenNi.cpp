#include "ModuleOpenNi.h"


//-----------
ModuleOpenNi::ModuleOpenNi() : Learn() {
    setGuiInputsVisible(false);

    openNi.setup();
    //openNi.setup("/Users/Gene/Downloads/hometest_single.oni");
    openNi.setMaxUsers(1);
    openNi.setTrackingBlobs(false);
    openNi.setNormalizeJoints(true);
    openNi.setGuiPosition(5, 75);
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
        openNi.draw(5, 515, 640, 480);
    }
}

//-----------
void ModuleOpenNi::addJointsAsInput() {
    vector<LearnInputParameter*> newInputs;
    vector<ofVec3f *> *joints = &openNi.getNormalizedJoints(0);
    for (int i=0; i<joints->size(); i++) {
        newInputs.push_back(addInput(openNi.getJointName(i)+"_x", &joints->at(i)->x, 0, 1));
        newInputs.push_back(addInput(openNi.getJointName(i)+"_y", &joints->at(i)->y, 0, 1));
        newInputs.push_back(addInput(openNi.getJointName(i)+"_z", &joints->at(i)->z, 0, 1));
    }
    addParametersAsInput("all joints", newInputs);
}

//-----------
void ModuleOpenNi::addUpperBodyAsInput() {
    vector<LearnInputParameter*> newInputs;
    vector<ofVec3f *> *joints = &openNi.getNormalizedJoints(0);
    for (int i=0; i<9; i++) {
        newInputs.push_back(addInput(openNi.getJointName(i)+"_x", &joints->at(i)->x, 0, 1));
        newInputs.push_back(addInput(openNi.getJointName(i)+"_y", &joints->at(i)->y, 0, 1));
        newInputs.push_back(addInput(openNi.getJointName(i)+"_z", &joints->at(i)->z, 0, 1));
    }
    addParametersAsInput("upper body", newInputs);
}

//-----------
void ModuleOpenNi::toggleVisible() {
    Learn::toggleVisible();
    openNi.toggleGuiVisible();
}