#include "MetaController.h"


void MetaController::setup(Control *control) {
    this->control = control;
    
    
    vector<ParameterBase *> parameters = control->getParameters();
    
    sequencer = new Sequencer();
    sequencer->setup(5, 5);
    sequencer->setPosition(200, 200);
    sequencer->setActive(true);

}

void MetaController::update() {
    sequencer->update();
}

void MetaController::draw() {
    sequencer->draw();
}

