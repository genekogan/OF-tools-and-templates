#include "PostGlitch.h"


//-----------
void PostGlitchLayer::setup() {
    post.setup(texLayer->getFbo());
    
    control.setName("PostGlitch");

    control.addParameter("convergence", &convergence);
    control.addParameter("glow", &glow);
    control.addParameter("shaker", &shaker);
    control.addParameter("cutslider", &cutslider);
    control.addParameter("twist", &twist);
    control.addParameter("outline", &outline);
    control.addParameter("noise", &noise);
    control.addParameter("slitscan", &slitscan);
    control.addParameter("swell", &swell);
    control.addParameter("invert", &invert);
    control.addParameter("crHighContrast", &crHighContrast);
    control.addParameter("crBlueRaise", &crBlueRaise);
    control.addParameter("crRedRaise", &crRedRaise);
    control.addParameter("crGreenRaise", &crGreenRaise);
    control.addParameter("crBlueInvert", &crBlueInvert);
    control.addParameter("crRedInvert", &crRedInvert);
    control.addParameter("crGreenInvert", &crGreenInvert);
    
    control.addParameter("customParameters", &customParameters);
    control.addParameter("delTime", &delTime, 0.0f, 2.0f);
    control.addParameter("stepMin", &stepMin, 2.0f, 100.0f);
    control.addParameter("stepMax", &stepMax, 10.0f, 160.0f);

    convergence = false;
    glow = false;
    shaker = false;
    cutslider = false;
    twist = false;
    outline = false;
    noise = false;
    slitscan = false;
    swell = false;
    invert = false;
    crHighContrast = false;
    crBlueRaise = false;
    crRedRaise = false;
    crGreenRaise = false;
    crBlueInvert = false;
    crRedInvert = false;
    crGreenInvert = false;
    
    customParameters = true;
    delTime = 1.2;
    stepMin = 4;
    stepMax = 16;
    
    sv0.setFunctionNoise(-25, 125, 0.1);
    sv1.setFunctionNoise(-25, 125, 0.1);
    sv2.setFunctionNoise(-25, 125, 0.1);
    sv3.setFunctionNoise(-25, 125, 0.1);
    rand.setFunctionNoise(-0.2, 1.2, 0.1);
    m0.setFunctionNoise(-150, ofGetWidth()+150, 0.1);
    m1.setFunctionNoise(-150, ofGetHeight()+150, 0.1);
    b0.setFunctionNoise(-0.5, 3.5, 0.1);
    b1.setFunctionNoise(-0.5, 3.5, 0.1);
}

//-----------
void PostGlitchLayer::setTexLayer(CanvasLayer *texLayer) {
    this->texLayer = texLayer;
    post.setFbo(texLayer->getFbo());
}

//-----------
void PostGlitchLayer::updateEffects() {
    post.setFx(OFXPOSTGLITCH_CONVERGENCE, convergence);
    post.setFx(OFXPOSTGLITCH_GLOW, glow);
    post.setFx(OFXPOSTGLITCH_SHAKER, shaker);
    post.setFx(OFXPOSTGLITCH_CUTSLIDER, cutslider);
    post.setFx(OFXPOSTGLITCH_TWIST, twist);
    post.setFx(OFXPOSTGLITCH_OUTLINE, outline);
    post.setFx(OFXPOSTGLITCH_NOISE, noise);
    post.setFx(OFXPOSTGLITCH_SLITSCAN, slitscan);
    post.setFx(OFXPOSTGLITCH_SWELL, swell);
    post.setFx(OFXPOSTGLITCH_INVERT, invert);
    post.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, crHighContrast);
    post.setFx(OFXPOSTGLITCH_CR_BLUERAISE, crBlueRaise);
    post.setFx(OFXPOSTGLITCH_CR_REDRAISE, crRedRaise);
    post.setFx(OFXPOSTGLITCH_CR_GREENRAISE, crGreenRaise);
    post.setFx(OFXPOSTGLITCH_CR_BLUEINVERT, crBlueInvert);
    post.setFx(OFXPOSTGLITCH_CR_REDINVERT, crRedInvert);
    post.setFx(OFXPOSTGLITCH_CR_GREENINVERT, crGreenInvert);
}

//-----------
void PostGlitchLayer::noiseChange() {
    sv0.setDelTime(delTime);
    sv1.setDelTime(delTime);
    sv2.setDelTime(delTime);
    sv3.setDelTime(delTime);
    rand.setDelTime(delTime);
    m0.setDelTime(delTime);
    m1.setDelTime(delTime);
    b0.setDelTime(delTime);
    b1.setDelTime(delTime);
    pDelTime = delTime;
}

//-----------
void PostGlitchLayer::setGuiPosition(int x, int y) {
    this->guiPosition = ofPoint(x, y);
    control.setGuiPosition(x, y);
}

//-----------
void PostGlitchLayer::toggleVisible() {
    control.toggleVisible();
}

//-----------
void PostGlitchLayer::setVisible(bool visible) {
    control.setVisible(visible);
}

//-----------
void PostGlitchLayer::render() {
    updateEffects();
    if (delTime != pDelTime)
        noiseChange();
    
    if (customParameters) {
        post.setShaderParameters((int) ofRandom(stepMin, stepMax),
                                 sv0.get(), sv1.get(), sv2.get(), sv3.get(),
                                 rand.get(), m0.get(), m1.get(), b0.get(), b1.get());
    } else {
        post.setShaderParameters();
    }
    
    post.generateFx();
    
    fbo = *texLayer->getFbo();
}
