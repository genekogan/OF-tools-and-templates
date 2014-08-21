#pragma once

#include "ofMain.h"

#include "DebugScreen.h"
#include "Rivers.h"
#include "Agents.h"
#include "MoviePlayer.h"
#include "Shader.h"
#include "Syphon.h"
#include "Letters.h"
#include "Amoeba.h"
#include "Bubbles.h"
#include "Subdivide.h"
#include "Polar.h"
#include "Cubes.h"
#include "GridFly.h"
#include "ShapeSpace.h"


class ofApp : public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    
private:
    
    Scene *debug, *rivers, *agents,
        *movie, *shader, *syphon,
        *letters, *amoeba, *bubbles,
        *subdivide, *polar, *cubes,
        *gridfly, *shapespace;
    
    
    void disableAll() {
        debug->setActive(false);
        rivers->setActive(false);
        agents->setActive(false);
        movie->setActive(false);
        shader->setActive(false);
        syphon->setActive(false);
        letters->setActive(false);
        amoeba->setActive(false);
        bubbles->setActive(false);
        subdivide->setActive(false);
        polar->setActive(false);
        cubes->setActive(false);
        gridfly->setActive(false);
        shapespace->setActive(false);
    }
    
    bool autoUpdate;
};
