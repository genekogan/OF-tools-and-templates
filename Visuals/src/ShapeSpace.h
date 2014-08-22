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
    
    ofxParameter<int> numElements;
    ofxParameter<ofVec3f> size;
    ofxParameter<ofVec3f> noiseSpeed;
    ofxParameter<ofColor> color;
    ofxParameter<ofColor> colorVar;
};



