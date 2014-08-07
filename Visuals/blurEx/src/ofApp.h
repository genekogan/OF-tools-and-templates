#pragma once

#include "ofMain.h"
#include "ofxBlur.h"


/*
 *  ofxBlurShader.h
 *
 *  Created by Rick Companje on 25-10-10.
 *  updated on 15-07-11 for of007
 *  Based on the blurShader example by Theo
 *
 */

#pragma once

#include "ofMain.h"

class ofxBlurShader {
public:
    
    ofxBlurShader();
    
	void setup(float w, float h);
	void begin(int amount=5, int iterations=2);
	void end();
	void draw(float x=0, float y=0, float w=0, float h=0);
    
    int iterations, amount;
    bool enabled;
    
    ofFbo fbo1,fbo2;
    ofShader hShader, vShader;
    
private:
    bool initialized;
};


class Bubble
{
public:
    Bubble() {
        position = ofPoint(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
        blurIdx = (int) ofRandom(30);
        size = ofRandom(0.1, 0.6);
        alph = ofRandom(60, 180);
        color = ofColor(ofRandom(100, 255), ofRandom(100), ofRandom(100,255));
        
        
        off1 = ofRandom(100);
        off2 = ofRandom(100);
    }
    void draw() {
        
        blurIdx = (int)(15+13*sin(ofGetFrameNum()*0.05 + off2));
        alph = 127 + 127 * sin(ofGetFrameNum()*0.02 + off1);
        
        ofSetColor(color, alph);
        fbos->at(blurIdx).draw(position.x-400*size, position.y-400*size, 800*size, 800*size);
    }
    void setFbos(vector<ofFbo> *fbos) {
        this->fbos = fbos;
    }
    vector<ofFbo> *fbos;
    
    int blurIdx;
    ofPoint position;
    float size;
    float alph;
    ofColor color;
    
    float off1, off2;
};



class ofApp : public ofBaseApp
{
public:
	void setup();
	void update();
	void draw();
	
	ofxBlurShader blur;
    
    vector<ofFbo> fbos;
    vector<Bubble> bubbles;

};
