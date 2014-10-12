#include "MantaLearn.h"


//-----------
MantaLearn::MantaLearn() : Learn() {
    manta.setup();
    setGuiInputsVisible(false);
    addAllPadsAsInput();
}

//-----------
void MantaLearn::update() {
    manta.update();
    Learn::update();
}

//-----------
void MantaLearn::draw() {
    if (visible) {
        manta.draw(5, 80, 410);
        manta.drawStats(5, 420, 410);
        Learn::draw();
    }
}

//-----------
void MantaLearn::addAllPadsAsInput() {
    LearnOutputParameter::GuiInputGroup group1;
    group1.name = "all pads";
    for (int row=0; row<6; row++) {
        for (int col=0; col<8; col++) {
            LearnInputParameter *newInput = addInput("mantaPad"+ofToString(col)+ofToString(row), manta.getPadRef(row, col), 0, 196);
            group1.inputs.push_back(newInput);
            *manta.getPadRef(row, col) = 0;
        }
    }
    inputGroups.push_back(group1);
    resetInputGroups();
}