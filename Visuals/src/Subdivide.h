#pragma once

#include "ofMain.h"
#include "Scene.h"


enum DrawStrategy { RECTS, DIAMONDS, CIRCLES };


class Subdivision
{
public:
    Subdivision(int generation,
                int x, int y,
                int width, int height,
                ofParameter<ofColor> *color,
                ofParameter<ofColor> *varColor,
                ofParameter<bool> *isLerp);
    Subdivision(int generation, Subdivision *parent, bool topleft, ofParameter<bool> *isLerp);
    void draw(DrawStrategy drawStrategy);

private:
    
    void subdivide();
    void update();
    
    void drawRect();
    void drawDiamond();
    void drawCircle();

    Subdivision *parent, *child1, *child2;
    int generation;
    float ratio;
    bool topleft, horizontal;

    int x, y;
    int width, height;
    
    ofParameter<ofColor> *color;
    ofParameter<ofColor> *varColor;
    ofVec3f offset;
    ofParameter<bool> *isLerp;
};


class Subdivide : public Scene
{
public:
    void setup();
    void update();
    void draw();

private:
    void setDrawType(string & s);
    
    DrawStrategy drawStrategy = CIRCLES;

    ofxParameter<int> ndivisions;
    ofxParameter<ofColor> color;
    ofxParameter<ofColor> varColor;
    ofxParameter<bool> isLerp;
    Subdivision *start;
};
