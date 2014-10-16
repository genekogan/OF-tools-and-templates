#include "Sequencer.h"


//-------
void Sequencer::setup(int rows, int cols) {
    sequencer.setup(rows, cols);

    active = false;
    pActive = false;
    bpm = 120;
    pBpm = 120;
    discrete = false;
    pDiscrete = false;
    
    sequencer.setBpm(bpm, 4);
    
    control.setName("sequencer");
    control.addParameter("bpm", &bpm, 60, 1500);
    control.addParameter("active", &active);
    control.addParameter("discrete", &discrete);
    control.addEvent("reset", this, &Sequencer::reset);
}

//-------
void Sequencer::update() {
    if (bpm != pBpm) {
        pBpm = bpm;
        sequencer.setBpm(bpm);
    }
    if (pActive != active) {
        pActive = active;
        if (active) {
            sequencer.start();
        }
        else {
            sequencer.stop();
        }
    }
    if (pDiscrete != discrete) {
        pDiscrete = discrete;
        sequencer.setDiscrete(discrete);
    }
}

//-------
void Sequencer::draw() {
    sequencer.draw();
}

//-------
void Sequencer::setPosition(int x, int y, int w, int h) {
    sequencer.setPosition(x, y, w, h);
    control.setGuiPosition(x+w+5, y);
}

//-------
void Sequencer::setVisible(bool visible) {
    this->visible = visible;
    sequencer.setVisible(visible);
    control.setVisible(visible);
}

//-------
void Sequencer::setValue(int row, int col, float value) {
    sequencer.setValue(row, col, value);
}
