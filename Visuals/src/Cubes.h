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
    
    void preset1(bool &b);
    void preset2(bool &b);
    void preset3(bool &b);
    void preset4(bool &b);
    void preset5(bool &b);
    void preset6(bool &b);
    void preset7(bool &b);

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

