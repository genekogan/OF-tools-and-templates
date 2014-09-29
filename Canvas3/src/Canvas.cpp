#include "Canvas.h"


//----------------
void Canvas::setup(int width, int height) {
    this->width = width;
    this->height = height;
    guiVisible = true;
    guiPosition = ofPoint(5, 5);

    control.setName("add layer");
    control.addEvent(" +creator", this, &Canvas::addCreator);
    control.addEvent(" +modifier", this, &Canvas::addModifier);
    control.addEvent(" +postProcessor", this, &Canvas::addPostProcessor);
    control.addEvent(" +postGlitch", this, &Canvas::addPostGlitch);
    control.addEvent(" +postFx", this, &Canvas::addPostFx);
}

//----------------
void Canvas::addLayer(CanvasLayer *newLayer) {
    if (layers.size() > 0)
        newLayer->setup(width, height, layers[layers.size()-1]);
    else
        newLayer->setup(width, height);
    layers.push_back(newLayer);
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
    layer->setGuiPosition(guiPosition.x+170*layers.size(), guiPosition.y);
    control.setGuiPosition(guiPosition.x+170*(layers.size()+1), guiPosition.y);
    layers.push_back(layer);
    return layer;
}

//----------------
void Canvas::setVisible(bool guiVisible) {
    this->guiVisible = guiVisible;
    for (int i=0; i<layers.size(); i++) {
        layers[i]->setVisible(guiVisible);
    }
    control.setVisible(guiVisible);
}

//----------------
void Canvas::toggleGuiVisible() {
    setVisible(!guiVisible);
}

//----------------
void Canvas::update() {
    //cout << " begin update " << ofGetFrameNum() << endl;
      //  cout << "render " << i << " :: " << layers[i]->scene->getName() << endl;
      //  layers[i]->render();
        //cout << "chkgui " << i << " :: " << layers[i]->scene->getName() << endl;
        //cout << "done__ " << i << " :: " << layers[i]->scene->getName() << endl;
    //cout << " end update "<< ofGetFrameNum() << endl;
    for (int i=0; i<layers.size(); i++) {
        layers[i]->render();
    }

    
    

}

//----------------
void Canvas::draw(int x, int y) {
    layers[layers.size()-1]->draw(x, y);
    
    for (int i=0; i<layers.size(); i++) {
        layers[i]->checkGuiCalls();
    }

}

//----------------
Canvas::~Canvas() {
    for (int i=0; i<layers.size(); i++) {
        //delete layers[i];
    }
}
