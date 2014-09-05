#pragma once

#include "ofMain.h"
#include "Scene.h"
#include "Control.h"


class MyCustomScene : public Scene {
public:
    void setup() {
        setName("myScene");
        
        control.registerParameter("position", &position, ofVec2f(200, 200), ofVec2f(800, 600));
        control.registerParameter("size", &size, ofVec2f(10, 10), ofVec2f(200, 200));
        control.registerParameter("color", &color, ofColor(0, 0, 0), ofColor(255, 255, 255));
        
        position = ofVec2f(300, 300);
        size = ofVec2f(100, 100);
        color = ofColor(255, 0, 20);
    }
    
    void update() {
        if (ofGetFrameNum() % 120 == 0) {
            position.easeTo(ofVec2f(ofRandom(200, 800), ofRandom(200, 600)), 60);
        }
    }
    
    void draw() {
        ofSetColor(color);
        ofEllipse(position->x, position->y, size->x, size->y);
    }
    
private:
    
    ofxParameter<ofVec2f> position;
    ofxParameter<ofVec2f> size;
    ofxParameter<ofColor> color;
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
