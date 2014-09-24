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
    
    int numVertices;
    ofVec2f radRange;
    ofVec2f center;
    float speed;
    float noiseFactor;
    float noiseRegion;
    float offset;
    ofColor colorFill;
    ofColor colorStroke;
    float lineWidth;
    bool filled;
    bool stroked;
    bool curvedVertices;
    int cycleStroke;
    float densityStroke;
    int cycleFill;
    float densityFill;

};