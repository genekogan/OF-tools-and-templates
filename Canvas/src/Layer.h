#pragma once

#include "ofMain.h"
#include "Control.h"

#include "Scene.h"

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


enum LayerType {
    CANVAS_CREATOR,
    CANVAS_MODIFIER,
    CANVAS_POST_PROCESSING,
    CANVAS_POST_GLITCH,
    CANVAS_POST_FX };


//----------------
class CanvasLayer
{
public:
    virtual ~CanvasLayer() { }

    virtual string getType() {return "CanvasLayer";}
    
    void setup(int width, int height, CanvasLayer *texLayer=NULL) {
        this->width = width;
        this->height = height;
        this->texLayer = texLayer;
        fbo.allocate(width, height);
        setup();
    }
    
    virtual void setTexLayer(CanvasLayer *texLayer=NULL) {
        this->texLayer = texLayer;
    }
    
    virtual void setup() {}
    
    virtual void render() {}
    
    void draw(int x, int y) {
        fbo.draw(x, y);
    }
    
    virtual void setGuiPosition(int x, int y) {
        this->guiPosition = ofPoint(x, y);
        control.setGuiPosition(x, y);
        scene->setGuiPosition(x, y);
    }
    
    virtual void toggleVisible() {
        control.toggleVisible();
        scene->toggeVisible();
    }
    
    virtual void setVisible(bool visible) {
        control.setVisible(visible);
        scene->setVisible(false);
    }
    
    virtual void checkGuiCalls() {
        if (scene->getControl().headerSelected) {
            scene->getControl().headerSelected = false;
            control.setVisible(true);
            scene->setVisible(false);
        }
        else if (control.headerSelected) {
            control.headerSelected = false;
            control.setVisible(false);
            scene->setVisible(true);
        }
    }
    
    Scene * getScene() {return scene;}
    
    string getName() {return control.getName();}
    ofFbo *getFbo() {return &fbo;}
    
    Control control;
    ofFbo fbo;
    Scene *scene;
    CanvasLayer *texLayer;
    ofPoint guiPosition;
    int width, height;
    vector<string> choices, shaders;
};


//----------------
class CreatorLayer : public CanvasLayer {
public:
    ~CreatorLayer();
    void setup();
    string getType() {return "CreatorLayer";}
    void render();
    void selectScene(string &s);
    void selectShader(string &s);

protected:
    virtual void setupScene(string s);
    virtual void setupShader(string s);
    virtual void setupCreators();
    virtual void setupChoices();

    Scene *agents, *amoeba, *bubbles, *cubes,
          *debug, *gridfly, *letters, *meshy,
          *movie, *polar, *rivers, *shader,
          *shapespace, *subdivide, *syphon;
};


//----------------
class ModifierLayer : public CanvasLayer {
public:
    ~ModifierLayer();
    void setup();
    string getType() {return "ModifierLayer";}
    void setTexLayer(CanvasLayer *texLayer=NULL);
    void render();
    void selectScene(string &s);
    virtual void setupScene(string s);

protected:
    virtual void setupChoices();
};



#include "PostProcessor.h"
#include "PostGlitch.h"
#include "PostFx.h"

