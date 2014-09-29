#pragma once

#include "Canvas.h"
#include "Control.h"

#include "ofxBloom.h"
#include "ofxGaussianBlur.h"
#include "ofxBokeh.h"
#include "ofxBlur.h"
#include "ofxLUT.h"
#include "ofxMultiTexture.h"
#include "ofxGlow.h"
#include "ofxMedian.h"
#include "ofxInverse.h"
#include "ofxOldTv.h"
#include "ofxBarrelChromaAb.h"
#include "ofxChromaAb.h"
#include "ofxChromaGlitch.h"
#include "ofxGrayscale.h"


class PostFxLayer : public CanvasLayer{
    void setup();
    void render();

private:    
    void chooseFx(string &s);
    void chooseLut(string &s);

    void checkGuiCalls() {}

    void setGuiPosition(int x, int y) {
        this->guiPosition = ofPoint(x, y);
        control.setGuiPosition(x, y);
    }
    
    void toggleVisible() {control.toggleVisible();}
    void setVisible(bool visible) {control.setVisible(visible);}
    
    string selection;
    ofDirectory dir;

    ofxLUT lut;
    ofxBloom bloom;
    ofxGaussianBlur gaussianBlur;
    ofxBokeh bokeh;
    ofxGlow glow;
    ofxBlur blur;
    ofxMedian median;
    ofxOldTv oldtv;
    ofxInverse inverse;
    ofxBarrelChromaAb barrelChromaAb;
    ofxChromaAb chromaAb;
    ofxChromaGlitch chromaGlitch;
    ofxGrayscale grayscale;
    
    float radius, fade;
};


