#pragma once

#include "ofMain.h"
#include "Scene.h"
#include "TimeFunction.h"



class Cube
{
public:
    void setup(ofVec3f mainPosition, ofVec3f marginPosition, ofVec3f targetSize, ofVec3f marginSize, ofVec3f ang, ofVec3f dAng, ofxParameter<ofVec3f> *pan);
    void draw();
    
private:
    ofVec3f mainPosition, marginPosition, wPosition;
    ofVec3f mainSize, targetSize, marginSize, wSize;
    ofVec3f ang, dAng;
    ofVec3f offsetP, offsetS;
    ofxParameter<ofVec3f> *pan;
};


class Cubes : public Scene
{
public:
    void setup();
    void update();
    void draw();
    
    void preset1();
    void preset2();
    void preset3();
    void preset4();
    void preset5();
    void preset6();
    void preset7();

private:
    void resetTimeFunctions();
    void addNewBox();

    int MAXBOXES = 200;
    vector<Cube> cubes;
    ofVec3f panPosition;
    
    TimeFunction<ofVec3f> mainPosition, marginPosition;
    TimeFunction<ofVec3f> targetSize, marginSize;
    TimeFunction<ofVec3f> ang, dAng;
    
    ofxParameter<ofVec2f> translation;
    ofxParameter<ofVec3f> pan;
    ofxParameter<int> newBoxRate;
};

