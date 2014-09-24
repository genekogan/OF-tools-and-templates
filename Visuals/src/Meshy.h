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
    
    void triangulate(bool &b);
    
    CirclePackGenerator circlePacker;
    EvenPointsGenerator pointGenerator;
    Triangulator triangulator;
    
    vector<ITRIANGLE> triangles;
    vector<XYZ> vertices;
    vector<ofPoint> points;
    
    bool realtime;
    bool circles;
    bool filled;
    int numCandidates;
    int numPoints;
    float minDist;
    float margin;
    float triNoise;
    float colorNoise;
    ofColor color;
    ofColor varColor;
    float lineWidth;
};



