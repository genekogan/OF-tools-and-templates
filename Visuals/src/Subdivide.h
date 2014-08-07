#pragma once

#include "ofMain.h"
#include "Scene.h"


class Subdivision
{
public:
    Subdivision(int generation, int x, int y, int width, int height, ofParameter<ofColor> *color, ofParameter<ofColor> *varColor);
    Subdivision(int generation, Subdivision *parent, bool topleft);
    void draw();

private:
    void subdivide();
    void update();

    Subdivision *parent, *child1, *child2;
    int generation;
    float ratio;
    bool topleft, horizontal;

    int x, y;
    int width, height;
    
    ofParameter<ofColor> *color;
    ofParameter<ofColor> *varColor;
    ofVec3f offset;
};


class Subdivide : public Scene
{
public:
    void setup();
    void update();
    void draw();
    
    ofParameter<int> ndivisions;
    ofParameter<ofColor> color;
    ofParameter<ofColor> varColor;
    Subdivision *start;
};