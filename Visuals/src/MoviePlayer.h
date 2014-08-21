#pragma once

#include "ofMain.h"
#include "Scene.h"



class MoviePlayer : public Scene
{
public:
    void setup();
    void update();
    void draw();
    
private:
    void jumpBack();
    void jumpRandom();
    
    ofVideoPlayer player;
    int w, h;
    ofxParameter<float> speed;
    float oldSpeed;
};



