#include "LeapLearn.h"


//-----------
LeapLearn::LeapLearn() : Learn() {
    setGuiInputsVisible(false);

    leap.setup();
    leap.setVelocityTracking(true, 15);
    
    setGuiPosition(2, 65);
}

//-----------
void LeapLearn::update() {
    leap.update();
    Learn::update();
}

//-----------
void LeapLearn::draw() {
    if (visible) {
        leap.draw(x, y, 400, 330);
        leap.drawVelocityGraph(LEFT, x, y+350, 180, 180);
        leap.drawVelocityGraph(RIGHT, x+190, y+350, 180, 180);
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
void LeapLearn::setGuiPosition(int x, int y) {
    this->x = x;
    this->y = y;
    leap.setGuiPosition(x, y);
}

//-----------
void LeapLearn::setVisible(bool visible) {
    this->visible = visible;
    Learn::setVisible(visible);
}

//-----------
void LeapLearn::toggleGuiSummaryView() {
    Learn::toggleGuiSummaryView();
    visible = summaryVisible;
}

//-----------
LeapLearn::~LeapLearn() {
    leap.close();
}