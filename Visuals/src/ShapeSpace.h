#pragma once

#include "ofMain.h"
#include "Scene.h"


class ShapeSpace : public Scene
{
public:
    ~ShapeSpace();
    void setup();
    void update();
    void draw();
    
private:
    ofEasyCam cam;
    ofLight light;
    
    int numElements;
    ofVec3f size;
    ofVec3f noiseSpeed;
    ofColor color;
    ofColor colorVar;
};



