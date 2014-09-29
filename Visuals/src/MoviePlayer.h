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
    void play(string &s);
    void jumpBack(string &s);
    void jumpRandom(string &s);
    
    ofVideoPlayer player;
    int w, h;
    float speed;
    float oldSpeed;
    bool playing;
};



