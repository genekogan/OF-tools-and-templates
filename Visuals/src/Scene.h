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
    }
    
    ~Scene() {
        ofRemoveListener(ofEvents().update, this, &Scene::update);
    }

    string getName() {
        return name;
    }
    
    void setName(string name) {
        this->name = name;
        bgColor.set(0, 0, 0, 255);
        control.setName(name);
        control.clearParameters();
        control.addParameter("clear", &toClear);
        control.addColor("bgColor", &bgColor);
        toClear = false;
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
        
        ofEnableSmoothing();
        
        ofTranslate(x, y);
        
        if (upsideDown) {
            ofTranslate(width, height);
            ofRotate(180);
        }
        
        if (toClear) {
            ofClear(0, 0);
        }
        
        if (bgColor.a > 0) {
            ofFill();
            ofSetColor(bgColor);
            ofRect(0, 0, width, height);
        }

        ofSetColor(255, 255);

        draw();     // call draw method of subclass
        
        ofDisableSmoothing();
        
        ofPopStyle();
        ofPopMatrix();
    }
    
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

    void setVisible(bool visible) {
        control.setVisible(visible);
    }
    
    void toggeVisible() {
        control.toggleVisible();
    }
    
    void setUpsideDown(bool upsideDown) {
        this->upsideDown = upsideDown;
    }
    
    Control & getControl() {
        return control;
    }

    Control control;
    string name;
    int width, height;
    ofColor bgColor;
    bool toClear;
    bool upsideDown;
    bool active;
};

