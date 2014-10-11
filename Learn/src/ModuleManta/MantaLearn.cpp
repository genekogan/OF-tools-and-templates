#include "MantaLearn.h"



MantaLearn::MantaLearn() : Learn() {
    manta.setup();
    setGuiInputsVisible(false);
}

//-----------
MantaOutputParameter * MantaLearn::addOutput(string name, float *val, float min, float max) {
    MantaOutputParameter *newOutput = new MantaOutputParameter(name, val, min, max);
    newOutput->setGuiPosition(420, 80+55*outputs.size());
    newOutput->setInputParameters(inputs);
    newOutput->setVisible(outputsVisible);
    newOutput->addParameterChangedListener((Learn*) this, &Learn::outputParameterChanged);
    newOutput->addParameterDeletedListener((Learn*) this, &Learn::outputParameterDeleted);
    newOutput->addParameterSelectedListener((Learn*) this, &Learn::parameterSelected);
    newOutput->addParameterViewedListener((Learn*) this, &Learn::outputParameterViewed);
    outputs.push_back(newOutput);
    if (oscManager.getSending()) {
        oscManager.registerToOsc(newOutput, true);
    }
    if (oscManager.getReceiving()) {
        oscManager.registerToOsc(newOutput, false);
    }
    return newOutput;
}

//-----------
void MantaLearn::update() {
    Learn::update();
    manta.update();
}

//-----------
void MantaLearn::draw() {
    Learn::draw();
    manta.draw(5, 70, 410);
}

//-----------
void MantaLearn::addAllPadsAsInputs() {
    for (int x=0; x<8; x++) {
        for (int y=0; y<6; y++) {
            addInput("mantaPad"+ofToString(x)+ofToString(y), manta.getPadRef(y, x), 0, 196);
            *manta.getPadRef(y, x) = 0;
        }
    }
}
