#pragma once

#include "ofMain.h"
#include "ofxLUT.h"
#include "Control.h"
#include "Layer.h"
#include "PostProcessor.h"
#include "PostGlitch.h"
#include "PostFx.h"
#include "Presets.h"


class Canvas
{
public:
    ~Canvas();
    
    void setup(int width, int height);
    void update();
    void draw(int x, int y);
    
    CanvasLayer* addLayer(LayerType type);
    void addLayer(CanvasLayer *layer);
    
    void addCreator(string &s) {addLayer(CANVAS_CREATOR);}
    void addModifier(string &s) {addLayer(CANVAS_MODIFIER);}
    void addPostProcessor(string &s) {addLayer(CANVAS_POST_PROCESSING);}
    void addPostGlitch(string &s) {addLayer(CANVAS_POST_GLITCH);}
    void addPostFx(string &s) {addLayer(CANVAS_POST_FX);}
    void clearLayers(bool skipCreator);
    
    void setVisible(bool guiVisible);
    void toggleGuiVisible();
    
    void loadPreset(string &filename);
    bool savePreset(string filename="");
    void savePresetFromGui(string &s);
    
    int getWidth() {return width;}
    int getHeight() {return height;}
    
protected:
    
    void setupGui();
    
    void setDeleteLayer(string &s);
    void deleteLayer();
    
    void runAutoMode();
    
    Control control;
    bool guiVisible;
    ofPoint guiPosition;
    vector<CanvasLayer *> layers;
    int width, height;

    int idxLayer;
    bool toDeleteLayer;    
    bool skipCreator;
    bool autoMode;
    int autoNumFrames, lastFrame, idxAutoPreset;
 
    // preloading postprocessor LUT and presets
    void preLoadPostProcessorLUT();
    vector<ofxLUT*> lut;
    vector<string> presetFilenames;
};
