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
    
    ofVideoPlayer player;
    int w, h;
    ofParameter<float> speed;
    float oldSpeed;
};



