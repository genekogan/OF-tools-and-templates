#pragma once

#include "ofMain.h"

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
    void addLayer(CanvasLayer *layer);
    
    void addCreator(bool &b) {addLayer(CANVAS_CREATOR);}
    void addModifier(bool &b) {addLayer(CANVAS_MODIFIER);}    
    void addPostProcessor(bool &b) {addLayer(CANVAS_POST_PROCESSING);}
    void addPostGlitch(bool &b) {addLayer(CANVAS_POST_GLITCH);}
    void addPostFx(bool &b) {addLayer(CANVAS_POST_FX);}
    
    void setVisible(bool guiVisible);
    void toggleGuiVisible();
    
private:
    
    Control control;
    bool guiVisible;
    ofPoint guiPosition;
    vector<CanvasLayer *> layers;
    int width, height;
};
