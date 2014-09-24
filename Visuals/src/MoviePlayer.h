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
    void play(bool &b);
    void jumpBack(bool &b);
    void jumpRandom(bool &b);
    
    ofVideoPlayer player;
    int w, h;
    float speed;
    float oldSpeed;
    bool playing;
};



