#pragma once

#include "ofMain.h"
#include "Scene.h"

#define FORCE_RESOLUTION 256


class Streak
{
public:
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
    void setup(int width, int height);
    void update();
    void draw();

private:
    void setupForceField();
    void addNewStreak();
    void setupStreak(Streak *streak);

    ofParameter<int> numStreaks = 15000;
    ofParameter<float> complexity = 0.01;
    ofParameter<float> baseSpeed = 1.0;
    ofParameter<float> speedVar = 0.5;
    ofParameter<int> maxAge = 100.0;
    ofParameter<int> refreshAlpha = 5;
    ofParameter<int> streakAlpha = 50;
    ofParameter<float> maxThickness = 8;
    ofParameter<bool> toRefresh = false;
    ofParameter<ofColor> color;
    ofParameter<int> colorVar = 10;
    
    vector<Streak> streaks;
    ofVec2f force[FORCE_RESOLUTION][FORCE_RESOLUTION];
    float noiseSeed;
};
