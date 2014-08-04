#pragma once

#include "ofMain.h"
#include "Control.h"


class Scene
{
public:
    Scene() {
        width = ofGetWidth();
        height = ofGetHeight();
    }

    template<class T> const T& getName2() const;
    
    virtual void setup(int width, int height) {
        this->width = width;
        this->height = height;
        control.registerParameter("bgColor", &bgColor, ofColor(0,0), ofColor(255,255));
        bgColor = ofColor(0, 255);
        ofAddListener(ofEvents().update, this, &Scene::update);
    };
    
    void update(ofEventArgs &data) {update();}

    virtual void update() { }
    
    void draw(int x, int y) {
        ofPushMatrix();
        ofPushStyle();
        ofTranslate(x, y);
        
        ofFill();
        ofSetColor(bgColor);
        ofRect(0, 0, width, height);

        ofSetColor(255);
        draw();
        
        ofPopStyle();
        ofPopMatrix();
    };
    
    virtual void draw() {}
    
    void setGuiPosition(int x, int y) {
        control.setGuiPosition(x, y);
    }
    
    void toggeVisible() {
        control.toggleVisible();
    }

    Control control;
    ofParameter<ofColor> bgColor;
    int width, height;
};
