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
#include "Layer.h"
#include "PostProcessor.h"
#include "PostGlitch.h"
#include "PostFx.h"


class Canvas
{
public:
    ~Canvas();
    void setup(int width, int height);
    void update();
    void draw(int x, int y);

    CanvasLayer* addLayer(LayerType type);
    void addCreator() {addLayer(CANVAS_CREATOR);}
    void addModifier() {addLayer(CANVAS_MODIFIER);}
    void addPostProcessor() {addLayer(CANVAS_POST_PROCESSING);}
    void addPostGlitch() {addLayer(CANVAS_POST_GLITCH);}
    void addPostFx() {addLayer(CANVAS_POST_FX);}
    
    void setupMetaGui();
    void chooseGui(string &s);
    void setVisible(bool guiVisible);
    void toggleGuiVisible();

private:

    Control metaGui;
    bool guiVisible;
    ofPoint guiPosition;
    vector<CanvasLayer *> layers;
    int width, height;
};
