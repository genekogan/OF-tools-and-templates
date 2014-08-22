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
    Scene *agents, *amoeba, *bubbles, *cubes,
        *debug, *gridfly, *letters, *meshy,
        *movie, *polar, *rivers, *shader,
        *shapespace, *subdivide, *syphon;
    
    bool autoUpdate;
    
    void disableAll() {
        agents->setActive(false);
        amoeba->setActive(false);
        bubbles->setActive(false);
        cubes->setActive(false);
        debug->setActive(false);
        gridfly->setActive(false);
        letters->setActive(false);
        meshy->setActive(false);
        movie->setActive(false);
        polar->setActive(false);
        rivers->setActive(false);
        shader->setActive(false);
        shapespace->setActive(false);
        subdivide->setActive(false);
        syphon->setActive(false);
    }
};
