#include "LeapLearn.h"


//-----------
LeapLearn::LeapLearn() : Learn() {
    setGuiInputsVisible(false);

    leap.setup();
    leap.setVelocityTracking(true, 15);
    leap.setNormalizeTips(true);
    
    guiInputs = new ofxUICanvas("Manta Inputs");
    guiInputs->setColorOutline(ofColor(255, 200));
    guiInputs->setDrawOutline(true);
    guiInputs->setColorBack(ofColor(60));
    ofAddListener(guiInputs->newGUIEvent, this, &LeapLearn::guiInputEvent);
    setupGuiInputs();
    setGuiPosition(2, 65);
    
    addLeftHandFingerTipsAsInput();
    addLeftHandIndexFingerTipAsInput();
    addLeftHandVelocityAsInput();
    addLeftOpenHandSizeAsInput();
    addLeftHandNormalAsInput();
    addLeftHandDirectionAsInput();
    addLeftHandRollAsInput();
    addLeftHandPitchAsInput();
    addLeftHandYawAsInput();
    addRightHandFingerTipsAsInput();
    addRightHandIndexFingerTipAsInput();
    addRightHandVelocityAsInput();
    addRightOpenHandSizeAsInput();
    addRightHandNormalAsInput();
    addRightHandDirectionAsInput();
    addRightHandRollAsInput();
    addRightHandPitchAsInput();
    addRightHandYawAsInput();
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
    vector<InputFeature> features;
    for (int i=0; i<left->size(); i++) {
        features.push_back(InputFeature(&left->at(i).x, 0, 10, "left_"+ofToString(i)+"_x"));
        features.push_back(InputFeature(&left->at(i).y, 0, 10, "left_"+ofToString(i)+"_y"));
        features.push_back(InputFeature(&left->at(i).z, 0, 10, "left_"+ofToString(i)+"_z"));
    }
    inputFeatures["left hand finger tips"] = features;
}

//-----------
void LeapLearn::addLeftHandIndexFingerTipAsInput() {
    ofPoint * finger = leap.getLeftHandIndexFingerTipRef();
    vector<InputFeature> features;
	features.push_back(InputFeature(&finger->x, 0, 10, "lIndex_x"));
    features.push_back(InputFeature(&finger->y, 0, 10, "lIndex_y"));
    features.push_back(InputFeature(&finger->z, 0, 10, "lIndex_z"));
    inputFeatures["left hand index finger"] = features;
}
//-----------
void LeapLearn::addLeftHandVelocityAsInput() {
    ofPoint * velocity = leap.getRightHandVelocityRef();
    vector<InputFeature> features;
	features.push_back(InputFeature(&velocity->x, -10, 10, "rVelocity_x"));
    features.push_back(InputFeature(&velocity->y, -10, 10, "rVelocity_y"));
    features.push_back(InputFeature(&velocity->z, -10, 10, "rVelocity_z"));
    inputFeatures["right hand velocity"] = features;
}

//-----------
void LeapLearn::addLeftOpenHandSizeAsInput() {
    vector<InputFeature> features;
	features.push_back(InputFeature(leap.getLeftOpenHandSizeRef(), 0, 100));
	inputFeatures["left hand size"] = features;
}

//-----------
void LeapLearn::addLeftHandNormalAsInput() {
    ofPoint * normal = leap.getLeftHandNormalRef();
    vector<InputFeature> features;
	features.push_back(InputFeature(&normal->x, -1, 1, "lNormal_x"));
    features.push_back(InputFeature(&normal->y, -1, 1, "lNormal_y"));
    features.push_back(InputFeature(&normal->z, -1, 1, "lNormal_z"));
    inputFeatures["left hand normal"] = features;
}

//-----------
void LeapLearn::addLeftHandDirectionAsInput() {
    ofPoint * dir = leap.getLeftHandDirectionRef();
    vector<InputFeature> features;
	features.push_back(InputFeature(&dir->x, -1, 1, "lDirection_x"));
    features.push_back(InputFeature(&dir->y, -1, 1, "lDirection_y"));
    features.push_back(InputFeature(&dir->z, -1, 1, "lDirection_z"));
    inputFeatures["left hand direction"] = features;
}

//-----------
void LeapLearn::addLeftHandRollAsInput() {
    vector<InputFeature> features;
	features.push_back(InputFeature(leap.getLeftHandRollRef(), -1, 1));
	inputFeatures["left hand roll"] = features;
}

//-----------
void LeapLearn::addLeftHandPitchAsInput() {
    vector<InputFeature> features;
	features.push_back(InputFeature(leap.getLeftHandPitchRef(), -1, 1));
	inputFeatures["left hand pitch"] = features;
}

//-----------
void LeapLearn::addLeftHandYawAsInput() {
    vector<InputFeature> features;
	features.push_back(InputFeature(leap.getLeftHandYawRef(), -1, 1));
	inputFeatures["left hand yaw"] = features;
}

//-----------
void LeapLearn::addRightHandFingerTipsAsInput() {
    vector<ofPoint> *right  = leap.getRightHandFingerTipsRef();
    vector<InputFeature> features;
	for (int i=0; i<right->size(); i++) {
		features.push_back(InputFeature(&right->at(i).x, 0, 10, "right_"+ofToString(i)+"_x"));
		features.push_back(InputFeature(&right->at(i).y, 0, 10, "right_"+ofToString(i)+"_y"));
		features.push_back(InputFeature(&right->at(i).z, 0, 10, "right_"+ofToString(i)+"_z"));
    }
    inputFeatures["right hand finger tips"] = features;
}

//-----------
void LeapLearn::addRightHandIndexFingerTipAsInput() {
    ofPoint * finger = leap.getRightHandIndexFingerTipRef();
    vector<InputFeature> features;
	features.push_back(InputFeature(&finger->x, 0, 10, "rIndex_x"));
    features.push_back(InputFeature(&finger->y, 0, 10, "rIndex_y"));
    features.push_back(InputFeature(&finger->z, 0, 10, "rIndex_z"));
    inputFeatures["right hand index finger"] = features;
}
//-----------
void LeapLearn::addRightHandVelocityAsInput() {
    ofPoint * finger = leap.getRightHandIndexFingerTipRef();
    vector<InputFeature> features;
	features.push_back(InputFeature(&finger->x, 0, 10, "rIndex_x"));
    features.push_back(InputFeature(&finger->y, 0, 10, "rIndex_y"));
    features.push_back(InputFeature(&finger->z, 0, 10, "rIndex_z"));
    inputFeatures["right index finger"] = features;
}

//-----------
void LeapLearn::addRightOpenHandSizeAsInput() {
    vector<InputFeature> features;
	features.push_back(InputFeature(leap.getRightOpenHandSizeRef(), 0, 100));
	inputFeatures["right hand size"] = features;
}

//-----------
void LeapLearn::addRightHandNormalAsInput() {
    ofPoint * normal = leap.getRightHandNormalRef();
    vector<InputFeature> features;
	features.push_back(InputFeature(&normal->x, -1, 1, "rNormal_x"));
    features.push_back(InputFeature(&normal->y, -1, 1, "rNormal_y"));
    features.push_back(InputFeature(&normal->z, -1, 1, "rNormal_z"));
    inputFeatures["right hand normal"] = features;
}

//-----------
void LeapLearn::addRightHandDirectionAsInput() {
    ofPoint * dir = leap.getRightHandDirectionRef();
    vector<InputFeature> features;
	features.push_back(InputFeature(&dir->x, -1, 1, "rDirection_x"));
    features.push_back(InputFeature(&dir->y, -1, 1, "rDirection_y"));
    features.push_back(InputFeature(&dir->z, -1, 1, "rDirection_z"));
    inputFeatures["right hand direction"] = features;
}

//-----------
void LeapLearn::addRightHandRollAsInput() {
    vector<InputFeature> features;
	features.push_back(InputFeature(leap.getRightHandRollRef(), -1, 1));
	inputFeatures["right hand roll"] = features;
}

//-----------
void LeapLearn::addRightHandPitchAsInput() {
    vector<InputFeature> features;
	features.push_back(InputFeature(leap.getRightHandPitchRef(), -1, 1));
	inputFeatures["right hand pitch"] = features;
}

//-----------
void LeapLearn::addRightHandYawAsInput() {
    vector<InputFeature> features;
	features.push_back(InputFeature(leap.getRightHandYawRef(), -1, 1));
	inputFeatures["right hand yaw"] = features;
}

//-----------
void LeapLearn::setGuiPosition(int x, int y) {
    this->x = x;
    this->y = y;
    leap.setGuiPosition(x, y);
    guiInputs->setPosition(x+200, y);
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

//-----------
void LeapLearn::setupGuiInputs() {
    guiInputs->clearWidgets();
    guiInputs->addLabel("Leap features");
    guiInputs->addLabelToggle("left hand finger tips", &lTips, 160.0f);
    guiInputs->addLabelToggle("left hand index finger", &lIndex, 160.0f);
    guiInputs->addLabelToggle("left hand velocity", &lVel, 160.0f);
    guiInputs->addLabelToggle("left hand size", &lSize, 160.0f);
    guiInputs->addLabelToggle("left hand normal", &lNormal, 160.0f);
    guiInputs->addLabelToggle("left hand direction", &lDir, 160.0f);
    guiInputs->addLabelToggle("left hand roll", &lRoll, 160.0f);
    guiInputs->addLabelToggle("left hand pitch", &lPitch, 160.0f);
    guiInputs->addLabelToggle("left hand yaw", &lYaw, 160.0f);
    guiInputs->addSpacer();
    guiInputs->addLabelToggle("right hand finger tips", &rTips, 160.0f);
    guiInputs->addLabelToggle("right hand index finger", &rIndex, 160.0f);
    guiInputs->addLabelToggle("right hand velocity", &rVel, 160.0f);
    guiInputs->addLabelToggle("right hand size", &rSize, 160.0f);
    guiInputs->addLabelToggle("right hand normal", &rNormal, 160.0f);
    guiInputs->addLabelToggle("right hand direction", &rDir, 160.0f);
    guiInputs->addLabelToggle("right hand roll", &rRoll, 160.0f);
    guiInputs->addLabelToggle("right hand pitch", &rPitch, 160.0f);
    guiInputs->addLabelToggle("right hand yaw", &rYaw, 160.0f);
    guiInputs->addSpacer();
    guiInputs->autoSizeToFitWidgets();
    guiInputs->setWidth(170);
}

//-----------
void LeapLearn::guiInputEvent(ofxUIEventArgs &e) {
    if (inputFeatures.count(e.getName()) != 0) {
        e.getButton()->getValue() ? addInputFeatureSet(e.getName()) : Learn::removeInputGroup(e.getName());
    }
}
