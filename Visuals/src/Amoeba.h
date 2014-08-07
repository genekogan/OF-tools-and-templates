#pragma once

#include "ofMain.h"
#include "Scene.h"


class Amoeba : public Scene
{
public:
    void setup();
    void update();
    void draw();
    
    ofParameter<int> numVertices;
    ofParameter<ofVec2f> radRange;
    ofParameter<ofVec2f> center;
    ofParameter<float> speed;
    ofParameter<float> noiseFactor;
    ofParameter<float> noiseRegion;
    ofParameter<float> offset;
    ofParameter<ofColor> color;
    ofParameter<float> lineWidth;
    ofParameter<bool> filled;
    ofParameter<bool> curvedVertices;
    float time;
};