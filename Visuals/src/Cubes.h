#pragma once

#include "ofMain.h"
#include "Scene.h"
#include "TimeFunction.h"


class Cube
{
public:
    void setup(ofVec3f mainPosition, ofVec3f marginPosition, ofVec3f targetSize, ofVec3f marginSize, ofVec3f ang, ofVec3f dAng, ofColor color, ofVec3f *pan);
    void draw();
    
private:
    ofVec3f mainPosition, marginPosition, wPosition;
    ofVec3f mainSize, targetSize, marginSize, wSize;
    ofVec3f ang, dAng;
    ofVec3f offsetP, offsetS;
    ofColor color;
    ofVec3f *pan;
};


class Cubes : public Scene
{
public:
    void setup();
    void update();
    void draw();
    
    void setPreset(string &s);
    void preset1(string &s);
    void preset2(string &s);
    void preset3(string &s);
    void preset4(string &s);
    void preset5(string &s);
    void preset6(string &s);
    void preset7(string &s);

private:
    void resetTimeFunctions();
    void addNewBox();

    int MAXBOXES = 200;
    vector<Cube> cubes;
    ofVec3f panPosition;
    
    TimeFunction<ofVec3f> mainPosition, marginPosition;
    TimeFunction<ofVec3f> targetSize, marginSize;
    TimeFunction<ofVec3f> ang, dAng;
    
    ofVec2f translation;
    ofVec3f pan;
    int newBoxRate;
    ofColor color;
    int colorVar;
};

