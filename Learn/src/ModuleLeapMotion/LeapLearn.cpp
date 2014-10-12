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
/*

//-----------
void LeapLearn::addAllPadsAsInput() {
    vector<LearnInputParameter*> newInputs;
    for (int row=0; row<6; row++) {
        for (int col=0; col<8; col++) {
            newInputs.push_back(addInput("mantaPad"+ofToString(col)+ofToString(row), manta.getPadRef(row, col), 0, 196));
            *manta.getPadRef(row, col) = 0;
        }
    }
    addParametersAsInput("all 48 pads", newInputs);
}

//-----------
void MantaLearn::addSlidersAsInput() {
    vector<LearnInputParameter*> newInputs;
    newInputs.push_back(addInput("slider1", manta.getSliderRef(0), 0, 4096));
    newInputs.push_back(addInput("slider2", manta.getSliderRef(1), 0, 4096));
    addParametersAsInput("sliders", newInputs);
    *manta.getSliderRef(0) = 0;
    *manta.getSliderRef(1) = 0;
}

//-----------
void MantaLearn::addButtonsAsInput() {
    vector<LearnInputParameter*> newInputs;
    newInputs.push_back(addInput("button1", manta.getButtonRef(0), 0, 250));
    newInputs.push_back(addInput("button2", manta.getButtonRef(1), 0, 250));
    newInputs.push_back(addInput("button3", manta.getButtonRef(2), 0, 250));
    newInputs.push_back(addInput("button4", manta.getButtonRef(3), 0, 250));
    addParametersAsInput("buttons", newInputs);
    *manta.getButtonRef(0) = 0;
    *manta.getButtonRef(1) = 0;
    *manta.getButtonRef(2) = 0;
    *manta.getButtonRef(3) = 0;
}

//-----------
void MantaLearn::addNumFingersAsInput() {
    addParameterAsInput("num fingers", addInput("numFingers", &manta.getNumFingers(), 0, 24));
}

//-----------
void MantaLearn::addPadSumAsInput() {
    addParameterAsInput("pad sum", addInput("padSum", &manta.getPadSum(), 0, 10));
}

//-----------
void MantaLearn::addPadAverageAsInput() {
    addParameterAsInput("pad average", addInput("padAverage", &manta.getPadAverage(), 0, 10));
}

//-----------
void MantaLearn::addPerimterAsInput() {
    addParameterAsInput("perimeter", addInput("perimeter", &manta.getPerimeter(), 0, 10));
}

//-----------
void MantaLearn::addAverageInterFingerDistanceAsInput() {
    addParameterAsInput("avg finger distance", addInput("avgFingerDistance", &manta.getAverageInterFingerDistance(), 0, 10));
}

//-----------
void MantaLearn::addCentroidAsInput() {
    vector<LearnInputParameter*> newInputs;
    newInputs.push_back(addInput("centroidX", &manta.getCentroidX(), 0, 10));
    newInputs.push_back(addInput("centroidY", &manta.getCentroidY(), 0, 10));
    addParametersAsInput("centroid", newInputs);
}

//-----------
void MantaLearn::addWeightedCentroidAsInput() {
    vector<LearnInputParameter*> newInputs;
    newInputs.push_back(addInput("wCentroidX", &manta.getWeightedCentroidX(), 0, 10));
    newInputs.push_back(addInput("wCentroidY", &manta.getWeightedCentroidY(), 0, 10));
    addParametersAsInput("weighted centroid", newInputs);
}
*/

LeapLearn::~LeapLearn() {
    leap.close();
}