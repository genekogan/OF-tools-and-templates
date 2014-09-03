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
    metaGui.registerMenu("choos view", this, &Canvas::chooseGui, choices);
    metaGui.registerLabel("add layer");
    metaGui.registerEvent(" +creator", this, &Canvas::addCreator);
    metaGui.registerEvent(" +modifier", this, &Canvas::addModifier);
    metaGui.registerEvent(" +postProcessor", this, &Canvas::addPostProcessor);
    metaGui.registerEvent(" +postGlitch", this, &Canvas::addPostGlitch);
    metaGui.registerEvent(" +postFx", this, &Canvas::addPostFx);
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
void Canvas::addLayer(LayerType type, int numLayers) {
    for (int i=0; i<numLayers; i++) {
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
    }
    setupMetaGui();
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