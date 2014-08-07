#pragma once

#include "ofMain.h"
#include "Scene.h"


enum DebugType { FULL, FRAME, FRAMES };


class DebugScreen : public Scene
{
public:
    void setup();
    void update();
    void draw();

private:
    ofParameter<ofColor> color;
    DebugType type = FRAMES;
};
