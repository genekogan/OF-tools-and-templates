#pragma once

#include "ofMain.h"
#include "Scene.h"
#include "ofxSyphon.h"


/*
 requires Syphon Framework added to build phases of app
*/

class Syphon : public Scene
{
public:
    void setup();
    void setClient(string serverName, string appName);
    void update();
    void draw();

private:
    ofxSyphonClient client;
};



