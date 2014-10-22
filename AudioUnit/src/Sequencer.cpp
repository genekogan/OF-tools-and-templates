#include "Sequencer.h"


//-------
void Sequencer::setup(int rows, int cols) {
    this->rows = rows;
    this->cols = cols;
    sequencer.setup(rows, cols);

    active = false;
    pActive = false;
    bpm = 120;
    pBpm = 120;
    discrete = false;
    pDiscrete = false;
    bpmInterval = 1;
    
    sequencer.setBpm(bpm, 4);
    bpmInterval = 60000.0 / bpm;
    
    control.setName("sequencer");
    control.addParameter("bpm", &bpm, 60, 1500);
    control.addParameter("active", &active);
    control.addParameter("discrete", &discrete);
    control.addEvent("reset", this, &Sequencer::reset);
    control.addEvent("randomize", this, &Sequencer::randomize);
}

//-------
void Sequencer::setSize(int rows, int cols) {
    this->rows = rows;
    this->cols = cols;
    sequencer.setSize(rows, cols);
}

//-------
void Sequencer::update() {
    if (bpm != pBpm) {
        pBpm = bpm;
        sequencer.setBpm(bpm);
        bpmInterval = 60000.0 / bpm;
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
    
    if (smooth) {
        vector<float> column;
        for (int r=0; r<rows; r++) {
            column.push_back(getValueInterpolated(r, sequencer.getColumn()));
        }
        ofNotifyEvent(interpolatedSequencerEvent, column, this);
    }
    
}

//-------
float Sequencer::getValue(int row, int col) {
    return sequencer.getValue(row, col);
}

//-------
float Sequencer::getValueInterpolated(int row, int col) {
    float t = (ofGetElapsedTimeMillis() - bpmTime) / bpmInterval;
    return ofLerp(sequencer.getValue(row, col), sequencer.getValue(row, (col+1)%cols), t);
}

//-------
void Sequencer::draw() {
    sequencer.draw();
    if (smooth) {
        ofSetColor(0);
        ofDrawBitmapString("smooth", x, y);
        ofSetColor(255);
    }
}

//-------
void Sequencer::setPosition(int x, int y, int w, int h) {
    this->x = x;
    this->y = y;
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
