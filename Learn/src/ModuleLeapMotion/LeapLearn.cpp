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
void LeapLearn::addLeftHandIndexFingerTipAsInput() {
    ofPoint * finger = leap.getLeftHandIndexFingerTipRef();
    vector<LearnInputParameter*> newInputs;
    newInputs.push_back(addInput("lIndex_x", &finger->x, 0, 10));
    newInputs.push_back(addInput("lIndex_y", &finger->y, 0, 10));
    newInputs.push_back(addInput("lIndex_z", &finger->z, 0, 10));
    addParametersAsInput("left index finger", newInputs);
}
//-----------
void LeapLearn::addLeftHandVelocityAsInput() {
    ofPoint * velocity = leap.getRightHandVelocityRef();
    vector<LearnInputParameter*> newInputs;
    newInputs.push_back(addInput("rVelocity_x", &velocity->x, -10, 10));
    newInputs.push_back(addInput("rVelocity_y", &velocity->y, -10, 10));
    newInputs.push_back(addInput("rVelocity_z", &velocity->z, -10, 10));
    addParametersAsInput("right velocity", newInputs);
}

//-----------
void LeapLearn::addLeftOpenHandSizeAsInput() {
    addInput("left hand size", leap.getLeftOpenHandSizeRef(), 0, 100);
}

//-----------
void LeapLearn::addLeftHandNormalAsInput() {
    ofPoint * normal = leap.getLeftHandNormalRef();
    vector<LearnInputParameter*> newInputs;
    newInputs.push_back(addInput("lNormal_x", &normal->x, -1, 1));
    newInputs.push_back(addInput("lNormal_y", &normal->y, -1, 1));
    newInputs.push_back(addInput("lNormal_z", &normal->z, -1, 1));
    addParametersAsInput("left normal", newInputs);
}

//-----------
void LeapLearn::addLeftHandDirectionAsInput() {
    ofPoint * dir = leap.getLeftHandDirectionRef();
    vector<LearnInputParameter*> newInputs;
    newInputs.push_back(addInput("lDirection_x", &dir->x, -1, 1));
    newInputs.push_back(addInput("lDirection_y", &dir->y, -1, 1));
    newInputs.push_back(addInput("lDirection_z", &dir->z, -1, 1));
    addParametersAsInput("left direction", newInputs);
}

//-----------
void LeapLearn::addLeftHandRollAsInput() {
    addInput("left hand roll", leap.getLeftHandRollRef(), -1, 1);
}

//-----------
void LeapLearn::addLeftHandPitchAsInput() {
    addInput("left hand pitch", leap.getLeftHandPitchRef(), -1, 1);
}

//-----------
void LeapLearn::addLeftHandYawAsInput() {
    addInput("left hand yaw", leap.getLeftHandYawRef(), -1, 1);
}

//-----------
void LeapLearn::addRightHandFingerTipsAsInput() {
    vector<ofPoint> *right  = leap.getRightHandFingerTipsRef();
    vector<LearnInputParameter*> newInputs;
    for (int i=0; i<right->size(); i++) {
        newInputs.push_back(addInput("right_"+ofToString(i)+"_x", &right->at(i).x, 0, 10));
        newInputs.push_back(addInput("right_"+ofToString(i)+"_y", &right->at(i).y, 0, 10));
        newInputs.push_back(addInput("right_"+ofToString(i)+"_z", &right->at(i).z, 0, 10));
    }
    addParametersAsInput("right fingertips", newInputs);
}

//-----------
void LeapLearn::addRightHandIndexFingerTipAsInput() {
    ofPoint * finger = leap.getRightHandIndexFingerTipRef();
    vector<LearnInputParameter*> newInputs;
    newInputs.push_back(addInput("rIndex_x", &finger->x, 0, 10));
    newInputs.push_back(addInput("rIndex_y", &finger->y, 0, 10));
    newInputs.push_back(addInput("rIndex_z", &finger->z, 0, 10));
    addParametersAsInput("right index finger", newInputs);
}
//-----------
void LeapLearn::addRightHandVelocityAsInput() {
    ofPoint * finger = leap.getRightHandIndexFingerTipRef();
    vector<LearnInputParameter*> newInputs;
    newInputs.push_back(addInput("rIndex_x", &finger->x, 0, 10));
    newInputs.push_back(addInput("rIndex_y", &finger->y, 0, 10));
    newInputs.push_back(addInput("rIndex_z", &finger->z, 0, 10));
    addParametersAsInput("right index finger", newInputs);
}

//-----------
void LeapLearn::addRightOpenHandSizeAsInput() {
    addInput("right hand size", leap.getRightOpenHandSizeRef(), 0, 100);
}

//-----------
void LeapLearn::addRightHandNormalAsInput() {
    ofPoint * normal = leap.getRightHandNormalRef();
    vector<LearnInputParameter*> newInputs;
    newInputs.push_back(addInput("rNormal_x", &normal->x, -1, 1));
    newInputs.push_back(addInput("rNormal_y", &normal->y, -1, 1));
    newInputs.push_back(addInput("rNormal_z", &normal->z, -1, 1));
    addParametersAsInput("right normal", newInputs);
}

//-----------
void LeapLearn::addRightHandDirectionAsInput() {
    ofPoint * dir = leap.getRightHandDirectionRef();
    vector<LearnInputParameter*> newInputs;
    newInputs.push_back(addInput("rDirection_x", &dir->x, -1, 1));
    newInputs.push_back(addInput("rDirection_y", &dir->y, -1, 1));
    newInputs.push_back(addInput("rDirection_z", &dir->z, -1, 1));
    addParametersAsInput("right direction", newInputs);
}

//-----------
void LeapLearn::addRightHandRollAsInput() {
    addInput("right hand roll", leap.getRightHandRollRef(), -1, 1);
}

//-----------
void LeapLearn::addRightHandPitchAsInput() {
    addInput("right hand pitch", leap.getRightHandPitchRef(), -1, 1);
}

//-----------
void LeapLearn::addRightHandYawAsInput() {
    addInput("right hand yaw", leap.getRightHandYawRef(), -1, 1);
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