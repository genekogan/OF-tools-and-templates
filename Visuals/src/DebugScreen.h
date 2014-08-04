#pragma once

#include "ofMain.h"
#include "Scene.h"


enum DebugType { FULL, FRAME, FRAMES };


class DebugScreen : public Scene
{
public:
    void setup(int width, int height);
    void update();
    void draw();

private:
    ofParameter<ofColor> color;
    DebugType type = FRAMES;
};
