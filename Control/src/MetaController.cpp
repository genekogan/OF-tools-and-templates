#include "MetaController.h"


void MetaController::setup(Control *control) {
    this->control = control;
    
    
    vector<ParameterBase*> parameters = control->getParameters();
    for (int i=0; i<parameters.size(); i++) {
        if (parameters[i]->getType() == ParameterBase::FLOAT ||
            parameters[i]->getType() == ParameterBase::INT) {
            seqParams.push_back(parameters[i]);
        }
    }

    sequencer = new Sequencer();
    sequencer->setup(seqParams.size(), 5);
    sequencer->setPosition(200, 200);
    sequencer->setActive(true);
    
    sequencer->addBeatListener(this, &MetaController::sequencerStepEvent);
    sequencer->addInterpolatedBeatListener(this, &MetaController::sequencerInterpolatedStepEvent);
    sequencer->setSmooth(true);

    

}

//-----------
void MetaController::sequencerStepEvent(vector<float> &column) {
    if (!sequencer->getSmooth()) {
        //processColumn(column);
    }
}

//-----------
void MetaController::sequencerInterpolatedStepEvent(vector<float> &column) {
    if (sequencer->getSmooth()) {
        //processColumn(column);
        for (int r=0; r<column.size(); r++) {
            if (seqParams[r]->getType() == ParameterBase::INT) {
                int val = ofMap(column[r], 0, 1, seqParams[r]->getMin<int>(), seqParams[r]->getMax<int>());
                ((Parameter<int> *) seqParams[r])->set(val);
            }
            else if (seqParams[r]->getType() == ParameterBase::FLOAT) {
                float val = ofMap(column[r], 0, 1, seqParams[r]->getMin<float>(), seqParams[r]->getMax<float>());
                ((Parameter<float> *) seqParams[r])->set(val);
            }
        }
    }
}


void MetaController::update() {
    sequencer->update();
    
}

void MetaController::draw() {
    sequencer->draw();
}

