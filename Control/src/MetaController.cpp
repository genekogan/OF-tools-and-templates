#include "MetaController.h"


//-----------
void MetaController::setup(Control *control) {
    this->control = control;
    vector<ParameterBase*> parameters = control->getParameters();
    int n = 0;
    for (int i=0; i<parameters.size(); i++) {
        if (parameters[i]->getType() == ParameterBase::FLOAT ||
            parameters[i]->getType() == ParameterBase::INT) {
            seqParams.push_back(parameters[i]);
            n+=1;
        }
        else if (parameters[i]->getType() == ParameterBase::VEC2F) {
            seqParams.push_back(parameters[i]);
            n+=2;
        }
        else if (parameters[i]->getType() == ParameterBase::VEC3F) {
            seqParams.push_back(parameters[i]);
            n+=3;
        }
        else if (parameters[i]->getType() == ParameterBase::VEC4F) {
            seqParams.push_back(parameters[i]);
            n+=4;
        }
    }
    seqActive.resize(n);
    for (int i=0; i<n; i++) seqActive[i] = true;
    
    sequencer = new Sequencer();
    sequencer->setup(n, 8);
    sequencer->addBeatListener(this, &MetaController::sequencerStepEvent);
    sequencer->addInterpolatedBeatListener(this, &MetaController::sequencerInterpolatedStepEvent);
    sequencer->setSmooth(true);
    setPosition(86, ofGetHeight()-158, 200, 150);
    
    ofAddListener(ofEvents().mousePressed, this, &MetaController::mousePressed);
}

//-----------
void MetaController::disable() {
    setVisible(false);
    sequencer->removeBeatListener(this, &MetaController::sequencerStepEvent);
    sequencer->removeInterpolatedBeatListener(this, &MetaController::sequencerInterpolatedStepEvent);
    ofRemoveListener(ofEvents().mousePressed, this, &MetaController::mousePressed);
}

//-----------
void MetaController::setPosition(int x, int y, int width, int height) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    sequencer->setPosition(x, y, width, height);
}

//-----------
void MetaController::setVisible(bool visible) {
    this->visible = visible;
    sequencer->setVisible(visible);
}

//-----------
void MetaController::sequencerStepEvent(vector<float> &column) {
    if (!sequencer->getSmooth()) {
        processColumn(column);
    }
}

//-----------
void MetaController::sequencerInterpolatedStepEvent(vector<float> &column) {
    if (sequencer->getSmooth()) {
        processColumn(column);
    }
}

//-----------
void MetaController::processColumn(vector<float> &column) {
    int idx = 0;
    for (int p=0; p<seqParams.size(); p++) {
        if (seqParams[p]->getType() == ParameterBase::INT) {
            if (seqActive[idx]) {
                int val = ofMap(column[idx], 0, 1, seqParams[p]->getMin<int>(), seqParams[p]->getMax<int>());
                ((Parameter<int> *) seqParams[p])->set(val);
            }
            idx += 1;
        }
        else if (seqParams[p]->getType() == ParameterBase::FLOAT) {
            if (seqActive[idx]) {
                float val = ofMap(column[idx], 0, 1, seqParams[p]->getMin<float>(), seqParams[p]->getMax<float>());
                ((Parameter<float> *) seqParams[p])->set(val);
            }
            idx += 1;
        }
        else if (seqParams[p]->getType() == ParameterBase::VEC2F) {
            ofVec2f min = seqParams[p]->getMin<ofVec2f>();
            ofVec2f max = seqParams[p]->getMax<ofVec2f>();
            ofVec2f cur = seqParams[p]->get<ofVec2f>();
            ofVec2f val = ofVec2f(seqActive[idx  ] ? column[idx  ] * min.x + (1.0-column[idx  ]) * max.x : cur.x,
                                  seqActive[idx+1] ? column[idx+1] * min.y + (1.0-column[idx+1]) * max.y : cur.y);
            ((Parameter<ofVec2f> *) seqParams[p])->set(val);
            idx += 2;
        }
        else if (seqParams[p]->getType() == ParameterBase::VEC3F) {
            ofVec3f min = seqParams[p]->getMin<ofVec3f>();
            ofVec3f max = seqParams[p]->getMax<ofVec3f>();
            ofVec3f cur = seqParams[p]->get<ofVec3f>();
            ofVec3f val = ofVec3f(seqActive[idx  ] ? column[idx  ] * min.x + (1.0-column[idx  ]) * max.x : cur.x,
                                  seqActive[idx+1] ? column[idx+1] * min.y + (1.0-column[idx+1]) * max.y : cur.y,
                                  seqActive[idx+2] ? column[idx+2] * min.z + (1.0-column[idx+2]) * max.z : cur.z);
            ((Parameter<ofVec3f> *) seqParams[p])->set(val);
            idx += 3;
        }
        else if (seqParams[p]->getType() == ParameterBase::VEC4F) {
            ofVec4f min = seqParams[p]->getMin<ofVec4f>();
            ofVec4f max = seqParams[p]->getMax<ofVec4f>();
            ofVec4f cur = seqParams[p]->get<ofVec4f>();
            ofVec4f val = ofVec4f(seqActive[idx  ] ? column[idx  ] * min.x + (1.0-column[idx  ]) * max.x : cur.x,
                                  seqActive[idx+1] ? column[idx+1] * min.y + (1.0-column[idx+1]) * max.y : cur.y,
                                  seqActive[idx+2] ? column[idx+2] * min.z + (1.0-column[idx+2]) * max.z : cur.z,
                                  seqActive[idx+3] ? column[idx+3] * min.w + (1.0-column[idx+3]) * max.w : cur.w);
            ((Parameter<ofVec4f> *) seqParams[p])->set(val);
            idx += 4;
        }
    }
}

//-----------
void MetaController::update() {
    sequencer->update();
}

//-----------
void MetaController::draw() {
    if (!visible)   return;
    ofPushStyle();
    ofSetColor(0, 150);
    ofRect(x-82, y-4, width + 195, height+8);
    sequencer->draw();
    int idx = 0;
    for (int i=0; i<seqParams.size(); i++) {
        if (seqParams[i]->getType() == ParameterBase::FLOAT ||
            seqParams[i]->getType() == ParameterBase::INT) {
            drawLabel(i, idx);
            idx+=1;
        }
        else if (seqParams[i]->getType() == ParameterBase::VEC2F) {
            drawLabel(i, idx,   ".x");
            drawLabel(i, idx+1, ".y");
            idx+=2;
        }
        else if (seqParams[i]->getType() == ParameterBase::VEC3F) {
            drawLabel(i, idx,   ".x");
            drawLabel(i, idx+1, ".y");
            drawLabel(i, idx+2, ".z");
            idx+=3;
        }
        else if (seqParams[i]->getType() == ParameterBase::VEC4F) {
            drawLabel(i, idx,   ".x");
            drawLabel(i, idx+1, ".y");
            drawLabel(i, idx+2, ".z");
            drawLabel(i, idx+3, ".w");
            idx+=4;
        }
    }
    ofPopStyle();
}

//-----------
void MetaController::drawLabel(int paramIdx, int idx, string suffix) {
    ofSetColor(seqActive[idx] ? ofColor(0, 255, 0) : ofColor(255, 0, 0));
    ofDrawBitmapString(seqParams[paramIdx]->getName() + suffix, x-80, ofMap(idx+0.5, 0, sequencer->getNumberRows(), y, y+height) + 3);
}

//-----------
void MetaController::mousePressed(ofMouseEventArgs &evt) {
    ofRectangle rect(x-80, y, 80, height);
    if (rect.inside(evt.x, evt.y)) {
        int idx = floor( (evt.y - y) / ((float) height / (float) (sequencer->getNumberRows())) );
        seqActive[idx] = !seqActive[idx];
    }
}

//-----------
MetaController::~MetaController() {
    sequencer->disable();
    delete sequencer;
}
