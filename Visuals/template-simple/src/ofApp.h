#pragma once

#include "ofMain.h"

#include "Agents.h"
#include "Amoeba.h"
#include "Bubbles.h"
#include "Cubes.h"
#include "DebugScreen.h"
#include "GridFly.h"
#include "Letters.h"
#include "Meshy.h"
#include "MoviePlayer.h"
#include "Polar.h"
#include "Rivers.h"
#include "Shader.h"
#include "ShapeSpace.h"
#include "Subdivide.h"
#include "Syphon.h"


class ofApp : public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    
private:
    
    void setInactiveAll();

    Scene *agents, *amoeba, *bubbles, *cubes,
        *debug, *gridfly, *letters, *meshy,
        *movie, *polar, *rivers, *shader,
        *shapespace, *subdivide, *syphon;
    
    bool autoUpdate;
    
};
