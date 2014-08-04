#pragma once

#include "ofMain.h"
#include "Scene.h"


class Letters : public Scene
{
public:
    void setup(int width, int height);
    void update();
    void draw();
    
    ofParameter<int> nrows;
    ofParameter<int> ncols;
    ofParameter<int> rate;
    ofParameter<ofColor> color;
    
    vector<char> chars;
    ofTrueTypeFont font;
};