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
    void setupControl();
    void setClipsHidden(bool clipsHidden);
    void toggleClipsHidden(string & s);
    
    void jumpBack(string &s);
    void jumpRandom(string &s);
    void selectMedia(string &s);
    void chooseMovie(string &s);
    
    void loadMovie(string path);
    void loadImage(string path);
    
    void triggerMovie(int idx);
    void triggerMovie();
    void triggerCallback(string &s) {triggerMovie();}
    
    vector<ofVideoPlayer> player;
    vector<string> moviePaths;
    int active;
    
    ofImage img;
    
    int w, h;
    float speed;
    float oldSpeed;
    bool playing;
    
    MediaMode mode;
    bool centeredHoriz;
    
    bool clipsHidden;
};



