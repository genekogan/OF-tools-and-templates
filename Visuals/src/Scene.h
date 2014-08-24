#pragma once

#include "ofMain.h"
#include "Control.h"


class Scene
{
public:
    Scene() {
        ofAddListener(ofEvents().update, this, &Scene::update);
        control.setVisible(false);
        width = ofGetWidth();
        height = ofGetHeight();
        name = "Scene";
        upsideDown = false;
        active = false;
        bgColor.setName("bgColor");
    }
    
    virtual ~Scene() {
        ofRemoveListener(ofEvents().update, this, &Scene::update);
    }

    string getName() {
        return name;
    }
    
    void setName(string name) {
        this->name = name;
        control.setName(name);
        control.refreshGui();
        control.registerParameter("bgColor", &bgColor, ofColor(0,0), ofColor(255,255));
        bgColor = ofColor(0, 255);
    }
        
    virtual void setup() { }

    virtual void update() { }
    
    virtual void draw() { }
    
    void setup(int width, int height) {
        this->width = width;
        this->height = height;
        active = true;
        setup();
    }
    
    void update(ofEventArgs &data) {
        if (active) {
            update();
        }
    }
    
    void draw(int x, int y) {
        ofPushMatrix();
        ofPushStyle();
        
        ofTranslate(x, y);
        
        if (upsideDown) {
            ofTranslate(width, height);
            ofRotate(180);
        }
        
        if (bgColor->a > 0) {
            ofFill();
            ofSetColor(bgColor);
            ofRect(0, 0, width, height);
        }

        ofSetColor(255, 255);

        draw();
        
        ofPopStyle();
        ofPopMatrix();
    };
    
    bool getActive() {
        return active;
    }

    virtual void setActive(bool active) {
        this->active = active;
        control.setVisible(active);
    }

    void setGuiPosition(int x, int y) {
        control.setGuiPosition(x, y);
    }
    
    void toggeVisible() {
        control.toggleVisible();
    }
    
    void setUpsideDown(bool upsideDown) {
        this->upsideDown = upsideDown;
    }

    Control control;
    ofxParameter<ofColor> bgColor;
    int width, height;
    string name;
    bool upsideDown;
    bool active;
};

