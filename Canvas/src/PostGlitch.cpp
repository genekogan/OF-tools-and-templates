#include "PostGlitch.h"


//-----------
void PostGlitchLayer::setup() {
    post.setup(texLayer->getFbo());
    
    control.setName("PostGlitch");
    control.registerLabel("Glitch effects");
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
    
    control.registerLabel("Parameterization");
    control.registerParameter("customParameters", &customParameters);
    control.registerParameter("delTime", &delTime, 0.0f, 2.0f);
    control.registerParameter("stepMin", &stepMin, 2.0f, 100.0f);
    control.registerParameter("stepMax", &stepMax, 10.0f, 160.0f);

    convergence.addListener(this, &PostGlitchLayer::effectsChange);
    glow.addListener(this, &PostGlitchLayer::effectsChange);
    shaker.addListener(this, &PostGlitchLayer::effectsChange);
    cutslider.addListener(this, &PostGlitchLayer::effectsChange);
    twist.addListener(this, &PostGlitchLayer::effectsChange);
    outline.addListener(this, &PostGlitchLayer::effectsChange);
    noise.addListener(this, &PostGlitchLayer::effectsChange);
    slitscan.addListener(this, &PostGlitchLayer::effectsChange);
    swell.addListener(this, &PostGlitchLayer::effectsChange);
    invert.addListener(this, &PostGlitchLayer::effectsChange);
    crHighContrast.addListener(this, &PostGlitchLayer::effectsChange);
    crBlueRaise.addListener(this, &PostGlitchLayer::effectsChange);
    crRedRaise.addListener(this, &PostGlitchLayer::effectsChange);
    crGreenRaise.addListener(this, &PostGlitchLayer::effectsChange);
    crBlueInvert.addListener(this, &PostGlitchLayer::effectsChange);
    crRedInvert.addListener(this, &PostGlitchLayer::effectsChange);
    crGreenInvert.addListener(this, &PostGlitchLayer::effectsChange);
    
    delTime.addListener(this, &PostGlitchLayer::noiseChange);
    
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
void PostGlitchLayer::effectsChange(bool &b) {
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
void PostGlitchLayer::noiseChange(float &v) {
    sv0.setDelTime(delTime);
    sv1.setDelTime(delTime);
    sv2.setDelTime(delTime);
    sv3.setDelTime(delTime);
    rand.setDelTime(delTime);
    m0.setDelTime(delTime);
    m1.setDelTime(delTime);
    b0.setDelTime(delTime);
    b1.setDelTime(delTime);
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

//----------
PostGlitchLayer::~PostGlitchLayer() {
    convergence.removeListener(this, &PostGlitchLayer::effectsChange);
    glow.removeListener(this, &PostGlitchLayer::effectsChange);
    shaker.removeListener(this, &PostGlitchLayer::effectsChange);
    cutslider.removeListener(this, &PostGlitchLayer::effectsChange);
    twist.removeListener(this, &PostGlitchLayer::effectsChange);
    outline.removeListener(this, &PostGlitchLayer::effectsChange);
    noise.removeListener(this, &PostGlitchLayer::effectsChange);
    slitscan.removeListener(this, &PostGlitchLayer::effectsChange);
    swell.removeListener(this, &PostGlitchLayer::effectsChange);
    invert.removeListener(this, &PostGlitchLayer::effectsChange);
    crHighContrast.removeListener(this, &PostGlitchLayer::effectsChange);
    crBlueRaise.removeListener(this, &PostGlitchLayer::effectsChange);
    crRedRaise.removeListener(this, &PostGlitchLayer::effectsChange);
    crGreenRaise.removeListener(this, &PostGlitchLayer::effectsChange);
    crBlueInvert.removeListener(this, &PostGlitchLayer::effectsChange);
    crRedInvert.removeListener(this, &PostGlitchLayer::effectsChange);
    crGreenInvert.removeListener(this, &PostGlitchLayer::effectsChange);
    
    delTime.removeListener(this, &PostGlitchLayer::noiseChange);
}
