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

#include "Control.h"

#include "ofxPostGlitch.h"



class CanvasLayer
{
public:
    ~CanvasLayer() { delete scene; }
    
    void setup(int width, int height, CanvasLayer *texLayer) {
        this->width = width;
        this->height = height;
        this->texLayer = texLayer;
        fbo.allocate(width, height);
        setup();
    }
    
    void setup(int width, int height) {
        setup(width, height, NULL);
    }
    
    virtual void setup() {}
    
    virtual void render() {}
    
    virtual void setGuiPosition(int x, int y) {
        this->guiPosition = ofPoint(x, y);
        control.setGuiPosition(x, y);
        //if (scene != NULL)
        scene->setGuiPosition(x+208, y);
    }
    
    virtual void toggleVisible() {
        control.toggleVisible();
        //if (scene != NULL)
        scene->toggeVisible();
    }
    
    void draw(int x, int y) {
        fbo.draw(x, y);
    }
    
    ofFbo *getFbo() {
        return &fbo;
    }
    
    Control control;
    ofFbo fbo;
    Scene *scene;
    CanvasLayer *texLayer;
    ofPoint guiPosition;
    int width, height;
    vector<string> choices;
    
};

class CreatorLayer : public CanvasLayer {
public:
    void setup();
    void render();
    void select(string & s);
};

class ModifierLayer : public CanvasLayer {
public:
    void setup();
    void render();
    void select(string & s);
};

#include "PostProcessor.h"


class Canvas
{
public:
    ~Canvas();
    void setup(int width, int height);
    void update();
    void draw(int x, int y);
    
    void toggleGuiVisible();
    
    void addCreator(int n);
    void addModifier(int n);
    void addPostProcessingLayer(int n);

private:

    vector<CanvasLayer *> layers;
    int width, height;
    
};