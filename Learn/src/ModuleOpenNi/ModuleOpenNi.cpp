#include "ModuleOpenNi.h"


//-----------
ModuleOpenNi::ModuleOpenNi() : Learn() {
    setGuiInputsVisible(true);

    openNi.setup();
    
    
    addJointsAsInput();
    
    
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
        openNi.draw(610, 440, 400, 300);
    }
}

//-----------
void ModuleOpenNi::addJointsAsInput() {
    vector<LearnInputParameter*> newInputs;
    vector<ofPoint *> joints = openNi.getJoints();
    for (int i=0; i<9; i++) {
        newInputs.push_back(addInput(openNi.getJointName(i)+"_x", &joints[i]->x, 0, 1));
        newInputs.push_back(addInput(openNi.getJointName(i)+"_y", &joints[i]->y, 0, 1));
        newInputs.push_back(addInput(openNi.getJointName(i)+"_z", &joints[i]->z, 0, 1));
    }
    addParametersAsInput("all joints", newInputs);
}
