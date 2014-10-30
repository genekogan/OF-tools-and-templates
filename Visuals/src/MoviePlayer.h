#pragma once

#include "ofMain.h"
#include "Scene.h"



class MoviePlayer : public Scene
{
public:
    enum MediaMode { IMAGE, MOVIE };
    
    void setup();
    void update();
    void draw();

private:
    void jumpBack(string &s);
    void jumpRandom(string &s);
    void selectMedia(string &s);
    
    void loadMovie(string path);
    void loadImage(string path);
    
    ofVideoPlayer player;
    ofImage img;
    
    int w, h;
    float speed;
    float oldSpeed;
    bool playing;
    
    MediaMode mode;
    bool centeredHoriz;
};



