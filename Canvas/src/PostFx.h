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
#include "ofxContrast.h"
#include "ofxMedian.h"
#include "ofxInverse.h"
#include "ofxOldTv.h"
#include "ofxBarrelChromaAb.h"
#include "ofxChromaAb.h"
#include "ofxChromaGlitch.h"
#include "ofxGrayscale.h"


class PostFxLayer : public CanvasLayer{
    void setup();
    
    void setGuiPosition(int x, int y);
    void toggleVisible();
    void setVisible(bool visible);
    
private:
    
    void buildGui();
    void chooseFx(string &s);
    void chooseLut(string &s);
    void render();

    Control gui;
    
    string selection;
    ofDirectory dir;

    ofxLUT lut;
    ofxBloom bloom;
    ofxGaussianBlur gaussianBlur;
    ofxBokeh bokeh;
    ofxGlow glow;
    ofxBlur blur;
    ofxContrast contrast;
    ofxMedian median;
    ofxOldTv oldtv;
    ofxInverse inverse;
    ofxBarrelChromaAb barrelChromaAb;
    ofxChromaAb chromaAb;
    ofxChromaGlitch chromaGlitch;
    ofxGrayscale grayscale;
    
    ofxParameter<float> gaussianBlurRadius;
    ofxParameter<float> bokehRadius;
    ofxParameter<float> glowRadius;
    ofxParameter<float> blurFade;
};


