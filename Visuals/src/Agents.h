#pragma once

#include "ofMain.h"
#include "Scene.h"


enum AgentsDrawType { LINES, TRIANGLES };


class Agent
{    
public:
    void setup(int x, int y, float baseRad, int width, int height);
    void setSpeed(float speed);
    void setSize(float size);
    void update();
    void wrap();
    void draw();
    void addIntersectingAgent(Agent *other);
    bool isIntersecting(Agent *other);
    vector<Agent *> getIntersecting();

    float rad, baseRad, speed, size;
    int tOffset;
    ofVec2f pos, vel;
    int width, height;
    
protected:
    vector<Agent *> intersecting;
};


class Agents : public Scene
{
public:
    void setup();
    void update();
    void draw();
    
protected:
    void setDrawingMode(string & s);
    void addNewAgent();
    void refresh();
    void drawDebug();

    vector<Agent> agents;
    
    ofxParameter<int> numAgents;
    ofxParameter<bool> wrapping;
    ofxParameter<bool> debug;
    ofxParameter<float> speed;
    ofxParameter<int> size;
    ofxParameter<ofColor> color1;
    ofxParameter<ofColor> color2;
    ofxParameter<int> fillAlpha;
    ofxParameter<int> strokeAlpha;
    ofxParameter<int> cycle;
    ofxParameter<float> density;
    ofxParameter<bool> drawLines;
    ofxParameter<bool> drawTriangles;
    
};