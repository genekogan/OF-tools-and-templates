#pragma once

#include "ofMain.h"
#include "Scene.h"


class Amoeba : public Scene
{
public:
    void setup();
    void update();
    void draw();
    
private:
    
    void drawAmoeba();
    
    float time;
    
    ofxParameter<int> numVertices;
    ofxParameter<ofVec2f> radRange;
    ofxParameter<ofVec2f> center;
    ofxParameter<float> speed;
    ofxParameter<float> noiseFactor;
    ofxParameter<float> noiseRegion;
    ofxParameter<float> offset;
    ofxParameter<ofColor> colorFill;
    ofxParameter<ofColor> colorStroke;
    ofxParameter<float> lineWidth;
    ofxParameter<bool> filled;
    ofxParameter<bool> stroked;
    ofxParameter<bool> curvedVertices;
    ofxParameter<int> cycleStroke;
    ofxParameter<float> densityStroke;
    ofxParameter<int> cycleFill;
    ofxParameter<float> densityFill;

};