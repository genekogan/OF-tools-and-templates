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


class Canvas
{
public:
    void setup(int width, int height, int numCreators, int numModifiers);
    void update();
    void draw(int x, int y);
    
    void setGuiPosition(int x, int y);
    void toggleGuiVisible();
    
private:

    void selectScene(int idx, string &s);
    void selectScene0(string &s) {selectScene(0, s);}
    void selectScene1(string &s) {selectScene(1, s);}
    void selectScene2(string &s) {selectScene(2, s);}
    void selectScene3(string &s) {selectScene(3, s);}
    void selectScene4(string &s) {selectScene(4, s);}
    
    int numCreators, numModifiers;
    int width, height;
    
    vector<ofFbo *> fbo;
    vector<Scene *> scenes;
    vector<Control *> selectors;
    vector<Modifier *> modifiers;
    
    ofPoint guiPosition;
    
    
    ofFbo fbo1, fbo2;
};


