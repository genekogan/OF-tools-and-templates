#include "Birl.h"


//-------
void Birl::setup(int port) {
	osc.setup(port);

    keys.resize(KEYS_NUMBER);
    keysDiscrete.resize(KEYS_NUMBER);
    pressure.resize(PRESSURE_NUMBER);
    embouchure.resize(EMBOUCHURE_NUMBER);
}

//-------
void Birl::updateFakeData() {
    for (int i=0; i<KEYS_NUMBER; i++) {
        keys[i] = ofNoise(i, ofGetFrameNum()*0.01 + 0.003*ofNoise(ofGetFrameNum()*0.003, i*5+5));
        keysDiscrete[i] = keys[i] > KEYS_DISCRETE_THRESHOLD ? 1.0 : 0.0;
    }
    for (int i=0; i<EMBOUCHURE_NUMBER; i++) {
        embouchure[i] = ofNoise(i*0.1, 0.05*ofGetFrameNum());
    }
    for (int i=0; i<PRESSURE_NUMBER; i++) {
        pressure[i] = max(0.0f, (i%2==0?-1:1)*(1.0f-2.0f*ofNoise(0.05*ofGetFrameNum())));
    }
}

//-------
void Birl::update() {
    while(osc.hasWaitingMessages()){
        ofxOscMessage m;
        osc.getNextMessage(&m);
        if (m.getAddress() == "/birl/keys/") {
            for (int i=0; i<KEYS_NUMBER; i++) {
                keys[i] = ofClamp(ofMap(m.getArgAsInt32(i), 0, KEYS_MAX, 0, 1), 0, 1);
                keysDiscrete[i] = keys[i] > KEYS_DISCRETE_THRESHOLD ? 1.0 : 0.0;
            }
        }
        else if (m.getAddress() == "/birl/breathpos/") {
            pressure[1] = m.getArgAsInt32(0) / maxbreathpos;
        }
        else if (m.getAddress() == "/birl/breathneg/") {
            pressure[0] = m.getArgAsInt32(0) / maxbreathneg;
        }
        else if (m.getAddress() == "/birl/maxbreathpos/") {
            maxbreathpos = m.getArgAsInt32(0);
        }
        else if (m.getAddress() == "/birl/maxbreathneg/") {
            maxbreathneg = m.getArgAsInt32(0);
        }
        else if (m.getAddress() == "/birl/embouchure/") {
            for (int i=0; i<EMBOUCHURE_NUMBER; i++) {
                embouchure[i] = ofMap(m.getArgAsInt32(i), 0, EMBOUCHURE_MAX, 0, 1);
            }
        }
    }
}

//-------
void Birl::draw(int x, int y, int w, int h) {
    ofPoint fingerPositions[KEYS_NUMBER] = {
        ofPoint(0.6,  0.1 ),
        ofPoint(0.5,  0.19),
        ofPoint(0.6,  0.28),
        ofPoint(0.8,  0.37),
        ofPoint(0.5,  0.52),
        ofPoint(0.5,  0.61),
        ofPoint(0.4,  0.7 ),
        ofPoint(0.2,  0.79),
        ofPoint(0.15, 0.86)
    };
    
    ofPushMatrix();
    ofPushStyle();
    
    ofTranslate(x, y);
    
    // labels
    ofSetColor(0);
    ofDrawBitmapString("embouchure", 5, 0);
    ofDrawBitmapString("pressure", 5, BIRL_DRAW_EMBOUCHURE_H+15);
    ofDrawBitmapString("keys", 5, BIRL_DRAW_EMBOUCHURE_H+BIRL_DRAW_PRESSURE_H+32);
    
    // draw embouchure
    ofTranslate(0, 3);
    ofSetColor(255);
    ofFill();
    ofRectRounded(0, 0, w, BIRL_DRAW_EMBOUCHURE_H, 5);
    ofNoFill();
    ofSetLineWidth(3);
    ofSetColor(20, 20, 255);
    ofRectRounded(0, 0, w, BIRL_DRAW_EMBOUCHURE_H, 5);
    ofSetLineWidth(2);
    ofNoFill();
    ofSetColor(0);
    
    ofBeginShape();
    for (int i=0; i<EMBOUCHURE_NUMBER; i++) {
        float x = ofMap(i, 0, EMBOUCHURE_NUMBER-1, 0, w);
        float y = ofMap(embouchure[i], 0, 1, BIRL_DRAW_EMBOUCHURE_H, 0);
        ofVertex(x, y);
    }
    ofEndShape();
    
    // draw breath pressure
    ofTranslate(0, BIRL_DRAW_EMBOUCHURE_H+15);
    
    ofFill();
    ofSetColor(255);
    ofRect(0, 0, w, BIRL_DRAW_PRESSURE_H);
    ofNoFill();
    ofSetColor(200, 60, 30);
    ofRect(0, 0, w/2, BIRL_DRAW_PRESSURE_H);
    ofRect(w/2, 0, w/2, BIRL_DRAW_PRESSURE_H);
    ofFill();
    ofSetColor(20, 20, 255);
    ofRect(-1+w/2, 0, pressure[0] * -0.5 * w, BIRL_DRAW_PRESSURE_H);
    ofRect( 1+w/2, 0, pressure[1] *  0.5 * w, BIRL_DRAW_PRESSURE_H);
    
    // draw keys
    ofTranslate(0, BIRL_DRAW_PRESSURE_H+17);
    
    int keysHeight = h - BIRL_DRAW_EMBOUCHURE_H - BIRL_DRAW_PRESSURE_H - 20;
    
    ofFill();
    ofSetColor(200, 60, 30);
    ofRectRounded(0, 0, w, keysHeight, 8);
    ofNoFill();
    ofSetLineWidth(2);
    ofSetColor(20, 20, 255);
    ofRectRounded(0, 0, w, keysHeight, 8);
    
    for (int i=0; i<9; i++) {
        ofPushMatrix();
        ofTranslate(w * fingerPositions[i].x,
                    keysHeight * fingerPositions[i].y);
        
        ofFill();
        ofSetLineWidth(0);
        ofSetColor(255);
        ofEllipse(0, 0, 40, 40);
        ofNoFill();
        ofSetLineWidth(2);
        ofSetColor(200);
        ofEllipse(0, 0, 40, 40);
        
        float s = 40 * keys[i];
        ofFill();
        ofSetLineWidth(0);
        ofSetColor(20, 20, 255);
        ofEllipse(0, 0, s, s);
        
        ofPopMatrix();
    }
    
    ofPopStyle();
    ofPopMatrix();
}
