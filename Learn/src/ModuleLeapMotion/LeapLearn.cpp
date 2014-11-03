#include "LeapLearn.h"


//-----------
LeapLearn::LeapLearn() : Learn() {
    setGuiInputsVisible(false);

    leap.setup();
    leap.setGuiPosition(4, 80);
    leap.setVelocityTracking(true, 15);
}

//-----------
void LeapLearn::update() {
    leap.update();
    Learn::update();
}

//-----------
void LeapLearn::draw() {
    if (visible) {
        leap.draw(2, 80, 400, 330);
        leap.drawVelocityGraph(LEFT, 2, 430, 180, 180);
        leap.drawVelocityGraph(RIGHT, 195, 430, 180, 180);
        Learn::draw();
    }
}

//-----------
void LeapLearn::addLeftHandFingerTipsAsInput() {
    vector<ofPoint> *left  = leap.getLeftHandFingerTipsRef();
    vector<LearnInputParameter*> newInputs;
    for (int i=0; i<left->size(); i++) {
        newInputs.push_back(addInput("left_"+ofToString(i)+"_x", &left->at(i).x, 0, 10));
        newInputs.push_back(addInput("left_"+ofToString(i)+"_y", &left->at(i).y, 0, 10));
        newInputs.push_back(addInput("left_"+ofToString(i)+"_z", &left->at(i).z, 0, 10));
    }
    addParametersAsInput("left fingertips", newInputs);
}

//-----------
void LeapLearn::addRightHandFingerTipsAsInput() {
    vector<ofPoint> *right = leap.getRightHandFingerTipsRef();
    vector<LearnInputParameter*> newInputs;
    for (int i=0; i<right->size(); i++) {
        newInputs.push_back(addInput("left_"+ofToString(i)+"_x", &right->at(i).x, 0, 10));
        newInputs.push_back(addInput("left_"+ofToString(i)+"_y", &right->at(i).y, 0, 10));
        newInputs.push_back(addInput("left_"+ofToString(i)+"_z", &right->at(i).z, 0, 10));
    }
    addParametersAsInput("right fingertips", newInputs);
}

//-----------
LeapLearn::~LeapLearn() {
    leap.close();
}