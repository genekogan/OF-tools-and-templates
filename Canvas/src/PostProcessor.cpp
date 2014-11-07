#include "PostProcessor.h"

//-----------
void PostProcessingLayer::setup() {
    post.init(width, height);
    
    alias = post.createPass<FxaaPass>();
    bloom = post.createPass<BloomPass>();
    dof = post.createPass<DofPass>();
    kaleidoscope = post.createPass<KaleidoscopePass>();
    noiseWarp = post.createPass<NoiseWarpPass>();
    pixelate = post.createPass<PixelatePass>();
    edges = post.createPass<EdgePass>();
    vTiltShift = post.createPass<VerticalTiltShifPass>();
    hTiltShift = post.createPass<HorizontalTiltShifPass>();
    godRay = post.createPass<GodRaysPass>();
    toon = post.createPass<ToonPass>();
    bleachBypass = post.createPass<BleachBypassPass>();
    convolve = post.createPass<ConvolutionPass>();
    fakeSSS = post.createPass<FakeSSSPass>();
    limbDarken = post.createPass<LimbDarkeningPass>();
    shift = post.createPass<RGBShiftPass>();
    rimHighlight = post.createPass<RimHighlightingPass>();
    ssao = post.createPass<SSAOPass>();
    zoomBlur = post.createPass<ZoomBlurPass>();
    
    control.setName("PostProcessing");
    control.addParameter("alias", &aliasEnabled);
    control.addParameter("bloom", &bloomEnabled);
    control.addParameter("dof", &dofEnabled);
    control.addParameter("kaleidoscope", &kaleidoscopeEnabled);
    control.addParameter("noiseWarp", &noiseWarpEnabled);
    control.addParameter("pixelate", &pixelateEnabled);
    control.addParameter("edges", &edgesEnabled);
    control.addParameter("vTiltShift", &vTiltShiftEnabled);
    control.addParameter("hTiltShift", &hTiltShiftEnabled);
    control.addParameter("godRay", &godRayEnabled);
    control.addParameter("toon", &toonEnabled);
    control.addParameter("bleachBypass", &bleachBypassEnabled);
    control.addParameter("convolve", &convolveEnabled);
    control.addParameter("fakeSSS", &fakeSSSEnabled);
    control.addParameter("limbDarken", &limbDarkenEnabled);
    control.addParameter("shift", &shiftEnabled);
    control.addParameter("rimHighlight", &rimHighlightEnabled);
    control.addParameter("ssao", &ssaoEnabled);
    control.addParameter("zoomBlur", &zoomBlurEnabled);
    
    /* add all parameters to control, but make them invisible in main gui */
    /* they are accessible instead in the secondary gui */
    control.addParameter("aperture", &dofAperture, 0.0f, 1.0f, true);
    control.addParameter("focus", &dofFocus, 0.95f, 1.0f, true);
    control.addParameter("maxBlur", &dofMaxBlur, 0.0f, 1.0f, true);
    control.addParameter("segments", &kaleidoscopeSegments, 0.0f, 16.0f, true);
    control.addParameter("amplitude", &noiseWarpAmplitude, 0.0f, 1.0f, true);
    control.addParameter("frequency", &noiseWarpFrequency, 0.0f, 1.0f, true);
    control.addParameter("speed", &noiseWarpSpeed, 0.0f, 1.0f, true);
    control.addParameter("resolution", &pixelateResolution, ofVec2f(0, 0), ofVec2f(100, 100), true);
    control.addParameter("hue", &edgesHue, 0.0f, 1.0f, true);
    control.addParameter("saturation", &edgesSaturation, 0.0f, 1.0f, true);
    control.addParameter("V", &vTiltShiftV, 0.0f, 1.0f, true);
    control.addParameter("R", &vTiltShiftR, 0.0f, 1.0f, true);
    control.addParameter("H", &hTiltShiftH, 0.0f, 0.1f, true);
    control.addParameter("R", &hTiltShiftR, 0.0f, 0.1f, true);
    control.addParameter("lightDirDOTviewDir", &godRayLightDirDOTviewDir, 0.0f, 1.0f, true);
    control.addParameter("lightPoisitonOnScreen", &godRayLightPoisitonOnScreen, ofVec3f(-1, -1, -1), ofVec3f(1, 1, 1), true);
    control.addParameter("specularColor", &toonSpecularColor, ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1), true);
    control.addParameter("shinyness", &toonShinyness, 0.0f, 1.0f, true);
    control.addParameter("level", &toonLevel, 0.0f, 1.0f, true);
    control.addParameter("enableSpecular", &toonEnableSpecular, true);
    control.addParameter("edgeThreshold", &toonEdgeThreshold, 0.0f, 1.0f, true);
    control.addParameter("diffuseColor", &toonDiffuseColor, ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1), true);
    control.addParameter("ambientColor", &toonAmbientColor, ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1), true);
    control.addParameter("bypassOpacity", &bleachBypassOpacity, 0.0f, 1.0f, true);
    control.addParameter("attenuationOffset", &fakeSSSAttenuationOffset, 0.0f, 1.0f, true);
    control.addParameter("baseColor", &fakeSSSBaseColor, ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1), true);
    control.addParameter("extinctionCoefficient", &fakeSSSExtinctionCoefficient, ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1), true);
    control.addParameter("lightColor", &fakeSSSLightColor, ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1), true);
    control.addParameter("lightPosition", &fakeSSSLightPosition, ofPoint(0, 0), ofPoint(1, 1), true);
    control.addParameter("materialThickness", &fakeSSSMaterialThickness, 0.0f, 1.0f, true);
    control.addParameter("rimScale", &fakeSSSRimScale, 0.0f, 1.0f, true);
    control.addParameter("specular", &fakeSSSSpecular, 0.0f, 1.0f, true);
    control.addParameter("specularColor", &fakeSSSSpecularColor, ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1), true);
    control.addParameter("brightness", &limbDarkenBrightness, 0.0f, 1.0f, true);
    control.addParameter("endColor", &limbDarkenEndColor, ofVec3f(0, 0, 0), ofVec3f(1, 1, 1), true);
    control.addParameter("radialScale", &limbDarkenRadialScale, 0.0f, 1.0f, true);
    control.addParameter("startColor", &limbDarkenStartColor, ofVec3f(0, 0, 0), ofVec3f(1, 1, 1), true);
    control.addParameter("amount", &shiftAmount, 0.0f, 1.0f, true);
    control.addParameter("angle", &shiftAngle, 0.0f, 1.0f, true);
    control.addParameter("cameraNear", &ssaoCameraNear, 0.0f, 1.0f, true);
    control.addParameter("cameraFar", &ssaoCameraFar, 0.0f, 1.0f, true);
    control.addParameter("fogNear", &ssaoFogNear, 0.0f, 1.0f, true);
    control.addParameter("fogFar", &ssaoFogFar, 0.0f, 1.0f, true);
    control.addParameter("fogEnabled", &ssaoFogEnabled, true);
    control.addParameter("onlyAO", &ssaoOnlyAO, true);
    control.addParameter("aoClamp", &ssaoAoClamp, 0.0f, 1.0f, true);
    control.addParameter("lumInfluence", &ssaoLumInfluence, 0.0f, 1.0f, true);
    control.addParameter("centerX", &zoomBlurCenterX, 0.0f, 1.0f, true);
    control.addParameter("centerY", &zoomBlurCenterY, 0.0f, 1.0f, true);
    control.addParameter("exposure", &zoomBlurExposure, 0.0f, 1.0f, true);
    control.addParameter("decay", &zoomBlurDecay, 0.0f, 1.0f, true);
    control.addParameter("density", &zoomBlurDensity, 0.0f, 1.0f, true);
    control.addParameter("weight", &zoomBlurWeight, 0.0f, 1.0f, true);
    control.addParameter("clamp", &zoomBlurClamp, 0.0f, 1.0f, true);

    /*
     // if to use menu instead of radio list
     string pp[] = {"alias", "bloom", "dof", "kaleidoscope", "noiseWarp",
     "pixelate", "edges", "vTiltShift", "hTiltShift", "godRay",
     "toon", "bleachBypass", "convolve", "fakeSSS",
     "limbDarken", "shift", "rimHighlights", "ssao", "zoomBlur"};
     vector<string> items = vector<string>(pp, pp + sizeof(pp) / sizeof(pp[0]));
     control.setName("PostProcessing");
     control.addMenu("Effects", items, this, &PostProcessingLayer::enableEffect);
    */
    
    aliasEnabled = false;
    bloomEnabled = false;
    dofEnabled = false;
    kaleidoscopeEnabled = false;
    noiseWarpEnabled = false;
    pixelateEnabled = false;
    edgesEnabled = false;
    vTiltShiftEnabled = false;
    hTiltShiftEnabled = false;
    godRayEnabled = false;
    toonEnabled = false;
    bleachBypassEnabled = false;
    convolveEnabled = false;
    fakeSSSEnabled = false;
    limbDarkenEnabled = false;
    shiftEnabled = false;
    rimHighlightEnabled = false;
    ssaoEnabled = false;
    zoomBlurEnabled = false;
    
    setupParameterGui();
}

//-----------
void PostProcessingLayer::setupParameterGui() {
    gui.clear();
    gui.setName("parameters");
    if (alias->getEnabled()) {
        //gui.registerLabel("alias");
    }
    if (bloom->getEnabled()) {
        //gui.registerLabel("bloom");
    }
    if (dof->getEnabled()) {
        //gui.registerLabel("dof");
        gui.addParameter("aperture", &dofAperture, 0.0f, 1.0f);
        gui.addParameter("focus", &dofFocus, 0.95f, 1.0f);
        gui.addParameter("maxBlur", &dofMaxBlur, 0.0f, 1.0f);
    }
    if (kaleidoscope->getEnabled()) {
        //gui.registerLabel("kaleidoscope");
        gui.addParameter("segments", &kaleidoscopeSegments, 0.0f, 16.0f);
    }
    if (noiseWarp->getEnabled()) {
        //gui.registerLabel("noiseWarp");
        gui.addParameter("amplitude", &noiseWarpAmplitude, 0.0f, 1.0f);
        gui.addParameter("frequency", &noiseWarpFrequency, 0.0f, 1.0f);
        gui.addParameter("speed", &noiseWarpSpeed, 0.0f, 1.0f);
    }
    if (pixelate->getEnabled()) {
        //gui.registerLabel("pixelate");
        gui.addParameter("resolution", &pixelateResolution, ofVec2f(0, 0), ofVec2f(100, 100));
    }
    if (edges->getEnabled()) {
        //gui.registerLabel("edges");
        gui.addParameter("hue", &edgesHue, 0.0f, 1.0f);
        gui.addParameter("saturation", &edgesSaturation, 0.0f, 1.0f);
    }
    if (vTiltShift->getEnabled()) {
        //gui.registerLabel("vTiltShift");
        gui.addParameter("V", &vTiltShiftV, 0.0f, 1.0f);
        gui.addParameter("R", &vTiltShiftR, 0.0f, 1.0f);
    }
    if (hTiltShift->getEnabled()) {
        //gui.registerLabel("hTiltShift");
        gui.addParameter("H", &hTiltShiftH, 0.0f, 0.1f);
        gui.addParameter("R", &hTiltShiftR, 0.0f, 0.1f);
    }
    if (godRay->getEnabled()) {
        //gui.registerLabel("godRay");
        gui.addParameter("lightDirDOTviewDir", &godRayLightDirDOTviewDir, 0.0f, 1.0f);
        gui.addParameter("lightPoisitonOnScreen", &godRayLightPoisitonOnScreen, ofVec3f(-1, -1, -1), ofVec3f(1, 1, 1));
    }
    if (toon->getEnabled()) {
        //gui.registerLabel("toon");
        gui.addParameter("specularColor", &toonSpecularColor, ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1));
        gui.addParameter("shinyness", &toonShinyness, 0.0f, 1.0f);
        gui.addParameter("level", &toonLevel, 0.0f, 1.0f);
        gui.addParameter("enableSpecular", &toonEnableSpecular);
        gui.addParameter("edgeThreshold", &toonEdgeThreshold, 0.0f, 1.0f);
        gui.addParameter("diffuseColor", &toonDiffuseColor, ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1));
        gui.addParameter("ambientColor", &toonAmbientColor, ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1));
    }
    if (bleachBypass->getEnabled()) {
        //gui.registerLabel("bleachBypass");
        gui.addParameter("bypassOpacity", &bleachBypassOpacity, 0.0f, 1.0f);
    }
    if (convolve->getEnabled()) {
        //gui.registerLabel("convolve");
    }
    if (fakeSSS->getEnabled()) {
        //gui.registerLabel("fakeSSS");
        gui.addParameter("attenuationOffset", &fakeSSSAttenuationOffset, 0.0f, 1.0f);
        gui.addParameter("baseColor", &fakeSSSBaseColor, ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1));
        gui.addParameter("extinctionCoefficient", &fakeSSSExtinctionCoefficient, ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1));
        gui.addParameter("lightColor", &fakeSSSLightColor, ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1));
        gui.addParameter("lightPosition", &fakeSSSLightPosition, ofPoint(0, 0), ofPoint(1, 1));
        gui.addParameter("materialThickness", &fakeSSSMaterialThickness, 0.0f, 1.0f);
        gui.addParameter("rimScale", &fakeSSSRimScale, 0.0f, 1.0f);
        gui.addParameter("specular", &fakeSSSSpecular, 0.0f, 1.0f);
        gui.addParameter("specularColor", &fakeSSSSpecularColor, ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1));
    }
    if (limbDarken->getEnabled()) {
        //gui.registerLabel("limbDarken");
        gui.addParameter("brightness", &limbDarkenBrightness, 0.0f, 1.0f);
        gui.addParameter("endColor", &limbDarkenEndColor, ofVec3f(0, 0, 0), ofVec3f(1, 1, 1));
        gui.addParameter("radialScale", &limbDarkenRadialScale, 0.0f, 1.0f);
        gui.addParameter("startColor", &limbDarkenStartColor, ofVec3f(0, 0, 0), ofVec3f(1, 1, 1));
    }
    if (shift->getEnabled()) {
        //gui.registerLabel("shift");
        gui.addParameter("amount", &shiftAmount, 0.0f, 1.0f);
        gui.addParameter("angle", &shiftAngle, 0.0f, 1.0f);
    }
    if (rimHighlight->getEnabled()) {
        //gui.registerLabel("rimHighlight");
    }
    if (ssao->getEnabled()) {
        //gui.registerLabel("ssao");
        gui.addParameter("cameraNear", &ssaoCameraNear, 0.0f, 1.0f);
        gui.addParameter("cameraFar", &ssaoCameraFar, 0.0f, 1.0f);
        gui.addParameter("fogNear", &ssaoFogNear, 0.0f, 1.0f);
        gui.addParameter("fogFar", &ssaoFogFar, 0.0f, 1.0f);
        gui.addParameter("fogEnabled", &ssaoFogEnabled);
        gui.addParameter("onlyAO", &ssaoOnlyAO);
        gui.addParameter("aoClamp", &ssaoAoClamp, 0.0f, 1.0f);
        gui.addParameter("lumInfluence", &ssaoLumInfluence, 0.0f, 1.0f);
    }
    if (zoomBlur->getEnabled()) {
        //gui.registerLabel("zoomBlur");
        gui.addParameter("centerX", &zoomBlurCenterX, 0.0f, 1.0f);
        gui.addParameter("centerY", &zoomBlurCenterY, 0.0f, 1.0f);
        gui.addParameter("exposure", &zoomBlurExposure, 0.0f, 1.0f);
        gui.addParameter("decay", &zoomBlurDecay, 0.0f, 1.0f);
        gui.addParameter("density", &zoomBlurDensity, 0.0f, 1.0f);
        gui.addParameter("weight", &zoomBlurWeight, 0.0f, 1.0f);
        gui.addParameter("clamp", &zoomBlurClamp, 0.0f, 1.0f);
    }
    gui.setVisible(false);
}

//-----------
void PostProcessingLayer::checkEffectActive(shared_ptr<RenderPass> pass, bool isEnabled) {
    bool wasEnabled = pass->getEnabled();
    pass->setEnabled(isEnabled);
    if (wasEnabled != isEnabled) {
        setupParameterGui();
    }
}

//-----------
void PostProcessingLayer::update() {
    checkEffectActive(alias, aliasEnabled);
    checkEffectActive(bloom, bloomEnabled);
    checkEffectActive(dof, dofEnabled);
    checkEffectActive(kaleidoscope, kaleidoscopeEnabled);
    checkEffectActive(noiseWarp, noiseWarpEnabled);
    checkEffectActive(pixelate, pixelateEnabled);
    checkEffectActive(edges, edgesEnabled);
    checkEffectActive(vTiltShift, vTiltShiftEnabled);
    checkEffectActive(hTiltShift, hTiltShiftEnabled);
    checkEffectActive(godRay, godRayEnabled);
    checkEffectActive(toon, toonEnabled);
    checkEffectActive(bleachBypass, bleachBypassEnabled);
    checkEffectActive(convolve, convolveEnabled);
    checkEffectActive(fakeSSS, fakeSSSEnabled);
    checkEffectActive(limbDarken, limbDarkenEnabled);
    checkEffectActive(shift, shiftEnabled);
    checkEffectActive(rimHighlight, rimHighlightEnabled);
    checkEffectActive(ssao, ssaoEnabled);
    checkEffectActive(zoomBlur, zoomBlurEnabled);
    
    if (alias->getEnabled()) {
    }
    if (bloom->getEnabled()) {
    }
    if (dof->getEnabled()) {
        dof->setAperture(dofAperture);
        dof->setFocus(dofFocus);
        dof->setMaxBlur(dofMaxBlur);
    }
    if (kaleidoscope->getEnabled()) {
        kaleidoscope->setSegments(kaleidoscopeSegments);
    }
    if (noiseWarp->getEnabled()) {
        noiseWarp->setAmplitude(noiseWarpAmplitude);
        noiseWarp->setFrequency(noiseWarpFrequency);
        noiseWarp->setSpeed(noiseWarpSpeed);
    }
    if (pixelate->getEnabled()) {
        pixelate->setResolution(pixelateResolution);
    }
    if (edges->getEnabled()) {
        edges->setHue(edgesHue);
        edges->setSaturation(edgesSaturation);
    }
    if (vTiltShift->getEnabled()) {
        vTiltShift->setV(vTiltShiftV);
        vTiltShift->setR(vTiltShiftR);
    }
    if (hTiltShift->getEnabled()) {
        hTiltShift->setH(hTiltShiftH);
        hTiltShift->setR(hTiltShiftR);
    }
    if (godRay->getEnabled()) {
        godRay->setLightDirDOTviewDir(godRayLightDirDOTviewDir);
        godRay->setLightPositionOnScreen(godRayLightPoisitonOnScreen);
    }
    if (toon->getEnabled()) {
        toon->setSpecularColor(toonSpecularColor);
        toon->setShinyness(toonShinyness);
        toon->setLevel(toonLevel);
        toon->setEnableSpecular(toonEnableSpecular);
        toon->setEdgeThreshold(toonEdgeThreshold);
        toon->setDiffuseColor(toonDiffuseColor);
        toon->setAmbientColor(toonAmbientColor);
    }
    if (bleachBypass->getEnabled()) {
        bleachBypass->setOpacity(bleachBypassOpacity);
    }
    if (convolve->getEnabled()) {
    }
    if (fakeSSS->getEnabled()) {
        fakeSSS->setAttenuationOffset(fakeSSSAttenuationOffset);
        fakeSSS->setBaseColor(fakeSSSBaseColor);
        fakeSSS->setExtinctionCoefficient(fakeSSSExtinctionCoefficient);
        fakeSSS->setLightColor(fakeSSSLightColor);
        fakeSSS->setLightPosition(fakeSSSLightPosition);
        fakeSSS->setMaterialThickness(fakeSSSMaterialThickness);
        fakeSSS->setRimScale(fakeSSSRimScale);
        fakeSSS->setSpecular(fakeSSSSpecular);
        fakeSSS->setSpecularColor(fakeSSSSpecularColor);
    }
    if (limbDarken->getEnabled()) {
        limbDarken->setBrightness(limbDarkenBrightness);
        limbDarken->setEndColor(limbDarkenEndColor);
        limbDarken->setRadialScale(limbDarkenRadialScale);
        limbDarken->setStartColor(limbDarkenStartColor);
    }
    if (shift->getEnabled()) {
        shift->setAmount(shiftAmount);
        shift->setAngle(shiftAngle);
    }
    if (rimHighlight->getEnabled()) {
    }
    if (ssao->getEnabled()) {
        ssao->setCameraNear(ssaoCameraNear);
        ssao->setCameraFar(ssaoCameraFar);
        ssao->setFogNear(ssaoFogNear);
        ssao->setFogFar(ssaoFogFar);
        ssao->setFogEnabled(ssaoFogEnabled);
        ssao->setOnlyAO(ssaoOnlyAO);
        ssao->setAoClamp(ssaoAoClamp);
        ssao->setLumInfluence(ssaoLumInfluence);
    }
    if (zoomBlur->getEnabled()) {
        zoomBlur->setCenterX(zoomBlurCenterX);
        zoomBlur->setCenterY(zoomBlurCenterY);
        zoomBlur->setExposure(zoomBlurExposure);
        zoomBlur->setDecay(zoomBlurDecay);
        zoomBlur->setDensity(zoomBlurDensity);
        zoomBlur->setWeight(zoomBlurWeight);
        zoomBlur->setClamp(zoomBlurClamp);
    }
}

//-----------
void PostProcessingLayer::render() {
    update();
    
    fbo.begin();
    post.begin();
    ofPushMatrix();
    ofTranslate(texLayer->width, texLayer->height);
    ofRotate(180);
    texLayer->draw(0, 0);
    ofPopMatrix();
    post.end();
    fbo.end();
}

//-----------
PostProcessingLayer::~PostProcessingLayer() {
    control.clear();
    control.setActive(false);
    control.setVisible(false);
    gui.clear();
    gui.setActive(false);
    gui.setVisible(false);
}