#include "Canvas.h"


//----------------
void Canvas::setup(int width, int height) {
    this->width = width;
    this->height = height;
    guiVisible = true;
    guiPosition = ofPoint(5, 5);
}

//----------------
void Canvas::setupMetaGui() {
    vector<string> choices;
    choices.push_back("all layers");
    for (int i=0; i<layers.size(); i++) {
        choices.push_back("layer "+ofToString(i));
    }
    metaGui.clearParameters();
    metaGui.setName("choose view");
    metaGui.setGuiPosition(ofGetWidth()-205, ofGetHeight() - 175 - 19*layers.size());
    metaGui.addMenu("choose view", choices, this, &Canvas::chooseGui);
//    metaGui.registerLabel("add layer");
    metaGui.addEvent(" +creator", this, &Canvas::addCreator);
    metaGui.addEvent(" +modifier", this, &Canvas::addModifier);
    metaGui.addEvent(" +postProcessor", this, &Canvas::addPostProcessor);
    metaGui.addEvent(" +postGlitch", this, &Canvas::addPostGlitch);
    metaGui.addEvent(" +postFx", this, &Canvas::addPostFx);
    
    string s = "all layers";
    chooseGui(s);
}

//----------------
void Canvas::chooseGui(string &s) {
    if (s=="all layers") {
        for (int i=0; i<layers.size(); i++) {
            layers[i]->setVisible(true);
            layers[i]->setGuiPosition(guiPosition.x+415*i, guiPosition.y);
        }
    }
    else {
        int idx = ofToInt(ofToString(s[6]));
        for (int i=0; i<layers.size(); i++) {
            layers[i]->setVisible(i==idx);
            if (i==idx) {
                layers[i]->setGuiPosition(guiPosition.x, guiPosition.y);
            }
        }
        guiVisible = true;
    }
}

//----------------
void Canvas::addLayer(CanvasLayer *newLayer) {
    if (layers.size() > 0)
        newLayer->setup(width, height, layers[layers.size()-1]);
    else
        newLayer->setup(width, height);
    layers.push_back(newLayer);
    setupMetaGui();
}

//----------------
CanvasLayer* Canvas::addLayer(LayerType type) {
    CanvasLayer *layer;
    if      (type == CANVAS_CREATOR)         layer = new CreatorLayer();
    else if (type == CANVAS_MODIFIER)        layer = new ModifierLayer();
    else if (type == CANVAS_POST_PROCESSING) layer = new PostProcessingLayer();
    else if (type == CANVAS_POST_GLITCH)     layer = new PostGlitchLayer();
    else if (type == CANVAS_POST_FX)         layer = new PostFxLayer();
    if (layers.size() > 0)
        layer->setup(width, height, layers[layers.size()-1]);
    else
        layer->setup(width, height);
    layers.push_back(layer);
    setupMetaGui();
    return layer;
}

//----------------
void Canvas::setVisible(bool guiVisible) {
    this->guiVisible = guiVisible;
    for (int i=0; i<layers.size(); i++) {
        layers[i]->setVisible(guiVisible);
    }
    metaGui.setVisible(guiVisible);
}

//----------------
void Canvas::toggleGuiVisible() {
    setVisible(!guiVisible);
}

//----------------
void Canvas::update() {
    for (int i=0; i<layers.size(); i++) {
        layers[i]->render();
    }
}

//----------------
void Canvas::draw(int x, int y) {
    layers[layers.size()-1]->draw(x, y);
}

//----------------
Canvas::~Canvas() {
    for (int i=0; i<layers.size(); i++) {
        delete layers[i];
    }
}
