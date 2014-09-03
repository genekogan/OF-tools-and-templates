#pragma once

#include "ofMain.h"
#include "Control.h"
#include "Scene.h"


enum LayerType {
    CANVAS_CREATOR,
    CANVAS_MODIFIER,
    CANVAS_POST_PROCESSING,
    CANVAS_POST_GLITCH,
    CANVAS_POST_FX };


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
        scene->setGuiPosition(x+208, y);
    }
    
    virtual void toggleVisible() {
        control.toggleVisible();
        scene->toggeVisible();
    }
    
    virtual void setVisible(bool visible) {
        control.setVisible(visible);
        scene->setVisible(visible);
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
    void selectShader(string & s);
private:
    void setupGui(bool isShader);
    bool settingUp;
};


class ModifierLayer : public CanvasLayer {
public:
    void setup();
    void render();
    void select(string & s);
};


#include "PostProcessor.h"
#include "PostGlitch.h"

