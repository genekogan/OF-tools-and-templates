#include "PostGlitch.h"


//-----------
void PostGlitchLayer::setup() {
    post.setup(texLayer->getFbo());
    
    control.registerParameter("convergence", &convergence);
    control.registerParameter("glow", &glow);
    control.registerParameter("shaker", &shaker);
    control.registerParameter("cutslider", &cutslider);
    control.registerParameter("twist", &twist);
    control.registerParameter("outline", &outline);
    control.registerParameter("noise", &noise);
    control.registerParameter("slitscan", &slitscan);
    control.registerParameter("swell", &swell);
    control.registerParameter("invert", &invert);
    control.registerParameter("crHighContrast", &crHighContrast);
    control.registerParameter("crBlueRaise", &crBlueRaise);
    control.registerParameter("crRedRaise", &crRedRaise);
    control.registerParameter("crGreenRaise", &crGreenRaise);
    control.registerParameter("crBlueInvert", &crBlueInvert);
    control.registerParameter("crRedInvert", &crRedInvert);
    control.registerParameter("crGreenInvert", &crGreenInvert);
    
    convergence.addListener(this, &PostGlitchLayer::programChange);
    glow.addListener(this, &PostGlitchLayer::programChange);
    shaker.addListener(this, &PostGlitchLayer::programChange);
    cutslider.addListener(this, &PostGlitchLayer::programChange);
    twist.addListener(this, &PostGlitchLayer::programChange);
    outline.addListener(this, &PostGlitchLayer::programChange);
    noise.addListener(this, &PostGlitchLayer::programChange);
    slitscan.addListener(this, &PostGlitchLayer::programChange);
    swell.addListener(this, &PostGlitchLayer::programChange);
    invert.addListener(this, &PostGlitchLayer::programChange);
    crHighContrast.addListener(this, &PostGlitchLayer::programChange);
    crBlueRaise.addListener(this, &PostGlitchLayer::programChange);
    crRedRaise.addListener(this, &PostGlitchLayer::programChange);
    crGreenRaise.addListener(this, &PostGlitchLayer::programChange);
    crBlueInvert.addListener(this, &PostGlitchLayer::programChange);
    crRedInvert.addListener(this, &PostGlitchLayer::programChange);
    crGreenInvert.addListener(this, &PostGlitchLayer::programChange);
}

//-----------
void PostGlitchLayer::programChange(bool &b) {
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
void PostGlitchLayer::setGuiPosition(int x, int y) {
    this->guiPosition = ofPoint(x, y);
    control.setGuiPosition(x, y);
}

//-----------
void PostGlitchLayer::toggleVisible() {
    control.toggleVisible();
}

//-----------
void PostGlitchLayer::render() {
    post.generateFx();
    fbo = *texLayer->getFbo();
}

//----------
PostGlitchLayer::~PostGlitchLayer() {
    convergence.removeListener(this, &PostGlitchLayer::programChange);
    glow.removeListener(this, &PostGlitchLayer::programChange);
    shaker.removeListener(this, &PostGlitchLayer::programChange);
    cutslider.removeListener(this, &PostGlitchLayer::programChange);
    twist.removeListener(this, &PostGlitchLayer::programChange);
    outline.removeListener(this, &PostGlitchLayer::programChange);
    noise.removeListener(this, &PostGlitchLayer::programChange);
    slitscan.removeListener(this, &PostGlitchLayer::programChange);
    swell.removeListener(this, &PostGlitchLayer::programChange);
    invert.removeListener(this, &PostGlitchLayer::programChange);
    crHighContrast.removeListener(this, &PostGlitchLayer::programChange);
    crBlueRaise.removeListener(this, &PostGlitchLayer::programChange);
    crRedRaise.removeListener(this, &PostGlitchLayer::programChange);
    crGreenRaise.removeListener(this, &PostGlitchLayer::programChange);
    crBlueInvert.removeListener(this, &PostGlitchLayer::programChange);
    crRedInvert.removeListener(this, &PostGlitchLayer::programChange);
    crGreenInvert.removeListener(this, &PostGlitchLayer::programChange);
}