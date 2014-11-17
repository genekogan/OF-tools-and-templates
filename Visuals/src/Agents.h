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
    bool isIntersecting(Agent &other);
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
    
    int numAgents;
    bool wrapping;
    bool debug;
    float speed;
    int size;
    ofColor color1;
    ofColor color2;
    int fillAlpha;
    int strokeAlpha;
    float lineWidth;
    int cycle;
    float density;
    bool drawLines;
    bool drawTriangles;
  
};