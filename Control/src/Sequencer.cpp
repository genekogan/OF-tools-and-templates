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
    
    // setup gui
    gui = new ofxUICanvas("sequencer");
    gui->clearWidgets();
    gui->addLabel("Sequencer");
    gui->addSpacer();
    gui->addIntSlider("bpm", 60, 1500, &bpm);
    gui->addToggle("active", &active);
    gui->addToggle("discrete", &discrete);
    gui->addButton("reset", false);
    gui->addButton("randomize", false);
    gui->autoSizeToFitWidgets();
    ofAddListener(gui->newGUIEvent, this, &Sequencer::guiEvent);
}

//-------
void Sequencer::setSize(int rows, int cols) {
    this->rows = rows;
    this->cols = cols;
    sequencer.setSize(rows, cols);
}

//-------
void Sequencer::setSmooth(bool smooth) {
    this->smooth = smooth;
    if (smooth) sequencer.addBeatListener(this, &Sequencer::playBeat);
    else        sequencer.removeBeatListener(this, &Sequencer::playBeat);
}

//-------
void Sequencer::update() {
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
    //control->setGuiPosition(x+w+5, y);
    gui->setPosition(x+w+5, y);
}

//-------
void Sequencer::setVisible(bool visible) {
    this->visible = visible;
    sequencer.setVisible(visible);
    //control->setVisible(visible);
    gui->setVisible(visible);
}

//-------
void Sequencer::setValue(int row, int col, float value) {
    sequencer.setValue(row, col, value);
}

//-------
void Sequencer::playBeat(vector<float> &column) {
    bpmTime = ofGetElapsedTimeMillis();
}

//-------
void Sequencer::guiEvent(ofxUIEventArgs &e) {
    if (e.getName() == "reset") {
        sequencer.reset();
    }
    else if (e.getName() == "randomize") {
        sequencer.randomize();
    }
    else if (e.getName() == "bpm") {
        sequencer.setBpm(bpm);
        bpmInterval = 60000.0 / bpm;
    }
    else if (e.getName() == "active") {
        active ? sequencer.start() : sequencer.stop();
    }
    else if (e.getName() == "discrete") {
        sequencer.setDiscrete(discrete);
    }
}

//-------
Sequencer::~Sequencer() {
//    delete control;
    gui->disable();
    delete gui;
}