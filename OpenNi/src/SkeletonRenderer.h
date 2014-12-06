#pragma once

#include "OpenNi.h"


class OpenNi;


class SkeletonRenderer {
public:
    void setup(OpenNi *openNi, int width, int height) {
        this->openNi = openNi;
        this->width = width;
        this->height = height;
        setupControl();
    }
    
    void setGuiPosition(int x, int y) {control.setGuiPosition(x, y);}
    void setGuiVisible(bool visible);
    void toggleGuiVisible() {setGuiVisible(!visible);}

    void setCalibration(ofxKinectProjectorToolkit *kpt) {
        this->kpt = kpt;
        calibrated = true;
    }
    
    void update() {
        
        checkChanges();
        
        if (drawFluids) {
            updateFluids();
        }
        
    }
    
    void draw() {
        ofPushMatrix();
        ofPushStyle();
        
        if (drawJointLines)     renderJointLines();
        if (drawOutwardLines)   renderOutwardLines();
        if (drawFluids)         renderFluids();
        
        ofPopStyle();
        ofPopMatrix();
    }
    
    void renderJointLines();
    void renderOutwardLines();

    void setupFluids();
    void updateFluids();
    void renderFluids();
    
    OpenNi *openNi;
    ofxKinectProjectorToolkit *kpt;
    bool calibrated = false;
    int width, height;
    
    
private:
    
    void setupControl();
    void checkChanges();
    
    Control control;
    bool visible;
    
    bool drawJointLines, drawOutwardLines, drawFluids;
    bool pDrawJointLines, pDrawOutwardLines, pDrawFluids;
    
    // joint lines
    int iterations;
    float randomOffset;
    ofColor color;
    float lineWidth;
    int numLines;
    int radius;
    
    // fluids
    ofxFluid fluid;
    vector<vector<ofVec2f> > pJoints;
    int maxUsers = 3;
    float dissipation, velDissipation;
    float displacement;
    float strength;
    float gravityX, gravityY;
};