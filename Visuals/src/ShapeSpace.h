#pragma once

#include "ofMain.h"
#include "Scene.h"


class ShapeSpace : public Scene
{
public:
    void setup();
    void update();
    void draw();
    
private:
    ofEasyCam cam;
};



