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

#include "Modifier.h"
#include "Control.h"



class CanvasLayer {
public:
    ~CanvasLayer() { delete scene; }
    void setup(int width, int height, CanvasLayer *texLayer);
    void setup(int width, int height) { setup(width, height, NULL); }
    virtual void setup() {}
    virtual void render() {}
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
    ofPoint position;
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

private:

    vector<CanvasLayer *> layers;
    int width, height;
    
};