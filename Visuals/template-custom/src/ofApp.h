#pragma once

#include "ofMain.h"
#include "Scene.h"
#include "Control.h"


class MyCustomScene : public Scene {
public:
    void setup() {
        setName("myScene");
        
        control.addParameter("position", &position, ofVec2f(200, 200), ofVec2f(800, 600));
        control.addParameter("size", &size, ofVec2f(10, 10), ofVec2f(200, 200));
        control.addColor("color", &color);
        
        position = ofVec2f(300, 300);
        size = ofVec2f(100, 100);
        color = ofColor(255, 0, 20);
    }
    
    void update() {
    }
    
    void draw() {
        ofSetColor(color);
        ofEllipse(position.x, position.y, size.x, size.y);
    }
    
private:
    
    ofVec2f position;
    ofVec2f size;
    ofColor color;
};


class ofApp : public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
private:
    Scene *scene;
};
