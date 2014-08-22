#pragma once

#include "ofMain.h"
#include "Scene.h"
#include "PointGeneration.h"


class Meshy : public Scene
{
public:
    void setup();
    void update();
    void draw();
    
private:
    
    void triangulate();
    
    CirclePackGenerator circlePacker;
    EvenPointsGenerator pointGenerator;
    Triangulator triangulator;
    
    vector<ITRIANGLE> triangles;
    vector<XYZ> vertices;
    vector<ofPoint> points;
    
    ofxParameter<bool> realtime;
    ofxParameter<bool> circles;
    ofxParameter<bool> filled;
    ofxParameter<int> numCandidates;
    ofxParameter<int> numPoints;
    ofxParameter<float> minDist;
    ofxParameter<float> margin;
    ofxParameter<float> triNoise;
    ofxParameter<float> colorNoise;
    ofxParameter<ofColor> color;
    ofxParameter<ofColor> varColor;
    ofxParameter<float> lineWidth;
};



