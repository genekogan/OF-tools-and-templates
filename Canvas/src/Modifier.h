#pragma once

#include "ofMain.h"
#include "Shader.h"
#include "Control.h"


class Modifier
{
public:
    Modifier();
    ~Modifier();
    void setup(int width, int height);
    void apply(ofFbo *fboTex);
    void draw(int x, int y);
    
    void setGuiPosition(int x, int y);
    void toggleGuiVisible();
    
private:
    
    void selectModifier(string &s);
    
    Scene *shader;
    Control control;
    
    int width, height;
    
    ofPoint guiPosition;
};


