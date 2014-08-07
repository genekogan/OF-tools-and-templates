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

    float rad, baseRad, speed, size, tOffset;
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
    void addNewAgent();
    void refresh();
    void drawDebug();

    vector<Agent> agents;
    
    ofParameter<int> numAgents;
    ofParameter<bool> wrapping;
    ofParameter<bool> debug;
    ofParameter<float> speed;
    ofParameter<int> size;
    ofParameter<ofColor> color;
    ofParameter<int> fillAlpha;
    ofParameter<int> strokeAlpha;
    ofParameter<float> t1;
    ofParameter<float> t2;
    
    AgentsDrawType type = LINES;
};