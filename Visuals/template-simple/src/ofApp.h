#pragma once

#include "ofMain.h"
#include "DebugScreen.h"
#include "Rivers.h"
#include "Agents.h"
#include "MoviePlayer.h"
#include "Shader.h"


class ofApp : public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);

private:

    Control control;

    Scene *debug, *rivers, *agents, *movie, *shader;
};
