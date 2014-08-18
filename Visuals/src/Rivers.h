#pragma once

#include "ofMain.h"
#include "Scene.h"



#define FORCE_RESOLUTION 256


class Streak
{
public:
    Streak() { }
    void setup(float x, float y, int maxAge, float speed, int maxThickness, float strokeAlpha, int width, int height, ofColor color);
    void update(ofPoint vel);
    void draw();
    bool isActive();

    ofVec2f pos, prev;
    float age, maxAge, speed, speedVar, maxThickness, strokeAlpha;
    ofColor color;
    int width, height;
};



class Rivers : public Scene
{
public:
    void setup();
    void update();
    void draw();

private:
    void setupForceField();
    void addNewStreak();
    void setupStreak(Streak *streak);
    
    ofxParameter<int> numStreaks;
    ofxParameter<float> complexity;
    ofxParameter<float> baseSpeed;
    ofxParameter<float> speedVar;
    ofxParameter<int> minAge;
    ofxParameter<int> maxAge;
    ofxParameter<int> refreshAlpha;
    ofxParameter<int> streakAlpha;
    ofxParameter<float> maxThickness;
    ofxParameter<ofColor> color;
    ofxParameter<int> colorVar;
    
    vector<Streak *> streaks;
    ofVec2f force[FORCE_RESOLUTION][FORCE_RESOLUTION];
    float noiseSeed;
};
