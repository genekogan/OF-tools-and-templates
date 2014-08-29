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
    contrast = post.createPass<ContrastPass>();
    convolve = post.createPass<ConvolutionPass>();
    fakeSSS = post.createPass<FakeSSSPass>();
    limbDarken = post.createPass<LimbDarkeningPass>();
    shift = post.createPass<RGBShiftPass>();
    rimHighlight = post.createPass<RimHighlightingPass>();
    ssao = post.createPass<SSAOPass>();
    zoomBlur = post.createPass<ZoomBlurPass>();
    
    control.setName("PostProcessing");
    control.registerLabel("Effects");
    control.registerParameter("alias", &aliasEnabled);
    control.registerParameter("bloom", &bloomEnabled);
    control.registerParameter("dof", &dofEnabled);
    control.registerParameter("kaleidoscope", &kaleidoscopeEnabled);
    control.registerParameter("noiseWarp", &noiseWarpEnabled);
    control.registerParameter("pixelate", &pixelateEnabled);
    control.registerParameter("edges", &edgesEnabled);
    control.registerParameter("vTiltShift", &vTiltShiftEnabled);
    control.registerParameter("hTiltShift", &hTiltShiftEnabled);
    control.registerParameter("godRay", &godRayEnabled);
    control.registerParameter("toon", &toonEnabled);
    control.registerParameter("bleachBypass", &bleachBypassEnabled);
    control.registerParameter("contrast", &contrastEnabled);
    control.registerParameter("convolve", &convolveEnabled);
    control.registerParameter("fakeSSS", &fakeSSSEnabled);
    control.registerParameter("limbDarken", &limbDarkenEnabled);
    control.registerParameter("shift", &shiftEnabled);
    control.registerParameter("rimHighlight", &rimHighlightEnabled);
    control.registerParameter("ssao", &ssaoEnabled);
    control.registerParameter("zoomBlur", &zoomBlurEnabled);
    
    aliasEnabled.addListener(this, &PostProcessingLayer::programChange);
    bloomEnabled.addListener(this, &PostProcessingLayer::programChange);
    dofEnabled.addListener(this, &PostProcessingLayer::programChange);
    kaleidoscopeEnabled.addListener(this, &PostProcessingLayer::programChange);
    noiseWarpEnabled.addListener(this, &PostProcessingLayer::programChange);
    pixelateEnabled.addListener(this, &PostProcessingLayer::programChange);
    edgesEnabled.addListener(this, &PostProcessingLayer::programChange);
    vTiltShiftEnabled.addListener(this, &PostProcessingLayer::programChange);
    hTiltShiftEnabled.addListener(this, &PostProcessingLayer::programChange);
    godRayEnabled.addListener(this, &PostProcessingLayer::programChange);
    toonEnabled.addListener(this, &PostProcessingLayer::programChange);
    bleachBypassEnabled.addListener(this, &PostProcessingLayer::programChange);
    contrastEnabled.addListener(this, &PostProcessingLayer::programChange);
    convolveEnabled.addListener(this, &PostProcessingLayer::programChange);
    fakeSSSEnabled.addListener(this, &PostProcessingLayer::programChange);
    limbDarkenEnabled.addListener(this, &PostProcessingLayer::programChange);
    shiftEnabled.addListener(this, &PostProcessingLayer::programChange);
    rimHighlightEnabled.addListener(this, &PostProcessingLayer::programChange);
    ssaoEnabled.addListener(this, &PostProcessingLayer::programChange);
    zoomBlurEnabled.addListener(this, &PostProcessingLayer::programChange);
    
    dofAperture.addListener(this, &PostProcessingLayer::dofApertureChanged);
    dofFocus.addListener(this, &PostProcessingLayer::dofFocusChanged);
    dofMaxBlur.addListener(this, &PostProcessingLayer::dofMaxBlurChanged);
    kaleidoscopeSegments.addListener(this, &PostProcessingLayer::kaleidoscopeSegmentsChanged);
    noiseWarpAmplitude.addListener(this, &PostProcessingLayer::noiseWarpAmplitudeChanged);
    noiseWarpFrequency.addListener(this, &PostProcessingLayer::noiseWarpFrequencyChanged);
    noiseWarpSpeed.addListener(this, &PostProcessingLayer::noiseWarpSpeedChanged);
    pixelateResolution.addListener(this, &PostProcessingLayer::pixelateResolutionChanged);
    edgesHue.addListener(this, &PostProcessingLayer::edgesHueChanged);
    edgesSaturation.addListener(this, &PostProcessingLayer::edgesSaturationChanged);
    vTiltShiftV.addListener(this, &PostProcessingLayer::vTiltShiftVChanged);
    vTiltShiftR.addListener(this, &PostProcessingLayer::vTiltShiftRChanged);
    hTiltShiftH.addListener(this, &PostProcessingLayer::hTiltShiftHChanged);
    hTiltShiftR.addListener(this, &PostProcessingLayer::hTiltShiftRChanged);
    godRayLightDirDOTviewDir.addListener(this, &PostProcessingLayer::godRayLightDirDOTviewDirChanged);
    godRayLightPoisitonOnScreen.addListener(this, &PostProcessingLayer::godRayLightPoisitonOnScreenChanged);
    toonSpecularColor.addListener(this, &PostProcessingLayer::toonSpecularColorChanged);
    toonShinyness.addListener(this, &PostProcessingLayer::toonShinynessChanged);
    toonLevel.addListener(this, &PostProcessingLayer::toonLevelChanged);
    toonEnableSpecular.addListener(this, &PostProcessingLayer::toonEnableSpecularChanged);
    toonEdgeThreshold.addListener(this, &PostProcessingLayer::toonEdgeThresholdChanged);
    toonDiffuseColor.addListener(this, &PostProcessingLayer::toonDiffuseColorChanged);
    toonAmbientColor.addListener(this, &PostProcessingLayer::toonAmbientColorChanged);
    bleachBypassOpacity.addListener(this, &PostProcessingLayer::bleachBypassOpacityChanged);
    contrastBrightness.addListener(this, &PostProcessingLayer::contrastBrightnessChanged);
    contrastContrast.addListener(this, &PostProcessingLayer::contrastContrastChanged);
    fakeSSSAttenuationOffset.addListener(this, &PostProcessingLayer::fakeSSSAttenuationOffsetChanged);
    fakeSSSBaseColor.addListener(this, &PostProcessingLayer::fakeSSSBaseColorChanged);
    fakeSSSExtinctionCoefficient.addListener(this, &PostProcessingLayer::fakeSSSExtinctionCoefficientChanged);
    fakeSSSLightColor.addListener(this, &PostProcessingLayer::fakeSSSLightColorChanged);
    fakeSSSLightPosition.addListener(this, &PostProcessingLayer::fakeSSSLightPositionChanged);
    fakeSSSMaterialThickness.addListener(this, &PostProcessingLayer::fakeSSSMaterialThicknessChanged);
    fakeSSSRimScale.addListener(this, &PostProcessingLayer::fakeSSSRimScaleChanged);
    fakeSSSSpecular.addListener(this, &PostProcessingLayer::fakeSSSSpecularChanged);
    fakeSSSSpecularColor.addListener(this, &PostProcessingLayer::fakeSSSSpecularColorChanged);
    limbDarkenBrightness.addListener(this, &PostProcessingLayer::limbDarkenBrightnessChanged);
    limbDarkenEndColor.addListener(this, &PostProcessingLayer::limbDarkenEndColorChanged);
    limbDarkenRadialScale.addListener(this, &PostProcessingLayer::limbDarkenRadialScaleChanged);
    limbDarkenStartColor.addListener(this, &PostProcessingLayer::limbDarkenStartColorChanged);
    shiftAmount.addListener(this, &PostProcessingLayer::shiftAmountChanged);
    shiftAngle.addListener(this, &PostProcessingLayer::shiftAngleChanged);
    ssaoCameraNear.addListener(this, &PostProcessingLayer::ssaoCameraNearChanged);
    ssaoCameraFar.addListener(this, &PostProcessingLayer::ssaoCameraFarChanged);
    ssaoFogNear.addListener(this, &PostProcessingLayer::ssaoFogNearChanged);
    ssaoFogFar.addListener(this, &PostProcessingLayer::ssaoFogFarChanged);
    ssaoFogEnabled.addListener(this, &PostProcessingLayer::ssaoFogEnabledChanged);
    ssaoOnlyAO.addListener(this, &PostProcessingLayer::ssaoOnlyAOChanged);
    ssaoAoClamp.addListener(this, &PostProcessingLayer::ssaoAoClampChanged);
    ssaoLumInfluence.addListener(this, &PostProcessingLayer::ssaoLumInfluenceChanged);
    zoomBlurCenterX.addListener(this, &PostProcessingLayer::zoomBlurCenterXChanged);
    zoomBlurCenterY.addListener(this, &PostProcessingLayer::zoomBlurCenterYChanged);
    zoomBlurExposure.addListener(this, &PostProcessingLayer::zoomBlurExposureChanged);
    zoomBlurDecay.addListener(this, &PostProcessingLayer::zoomBlurDecayChanged);
    zoomBlurDensity.addListener(this, &PostProcessingLayer::zoomBlurDensityChanged);
    zoomBlurWeight.addListener(this, &PostProcessingLayer::zoomBlurWeightChanged);
    zoomBlurClamp.addListener(this, &PostProcessingLayer::zoomBlurClampChanged);

    buildGui();
}

//-----------
void PostProcessingLayer::setGuiPosition(int x, int y) {
    this->guiPosition = ofPoint(x, y);
    control.setGuiPosition(x, y);
    gui.setGuiPosition(x+208, y);
}

//-----------
void PostProcessingLayer::toggleVisible() {
    control.toggleVisible();
    gui.toggleVisible();
}

//-----------
void PostProcessingLayer::setVisible(bool visible) {
    control.setVisible(visible);
    gui.setVisible(visible);
}

//-----------
void PostProcessingLayer::programChange(bool &b) {
    buildGui();
}

//-----------
void PostProcessingLayer::buildGui() {
    alias->setEnabled(aliasEnabled);
    bloom->setEnabled(bloomEnabled);
    dof->setEnabled(dofEnabled);
    kaleidoscope->setEnabled(kaleidoscopeEnabled);
    noiseWarp->setEnabled(noiseWarpEnabled);
    pixelate->setEnabled(pixelateEnabled);
    edges->setEnabled(edgesEnabled);
    vTiltShift->setEnabled(vTiltShiftEnabled);
    hTiltShift->setEnabled(hTiltShiftEnabled);
    godRay->setEnabled(godRayEnabled);
    toon->setEnabled(toonEnabled);
    bleachBypass->setEnabled(bleachBypassEnabled);
    contrast->setEnabled(contrastEnabled);
    convolve->setEnabled(convolveEnabled);
    fakeSSS->setEnabled(fakeSSSEnabled);
    limbDarken->setEnabled(limbDarkenEnabled);
    shift->setEnabled(shiftEnabled);
    rimHighlight->setEnabled(rimHighlightEnabled);
    ssao->setEnabled(ssaoEnabled);
    zoomBlur->setEnabled(zoomBlurEnabled);
    
    gui.clearParameters();
    
    if (alias->getEnabled()) {
        gui.registerLabel("alias");
    }
    if (bloom->getEnabled()) {
        gui.registerLabel("bloom");
    }
    if (dof->getEnabled()) {
        gui.registerLabel("dof");
        gui.registerParameter("aperture", &dofAperture, 0.0f, 1.0f);
        gui.registerParameter("focus", &dofFocus, 0.95f, 1.0f);
        gui.registerParameter("maxBlur", &dofMaxBlur, 0.0f, 1.0f);
    }
    if (kaleidoscope->getEnabled()) {
        gui.registerLabel("kaleidoscope");
        gui.registerParameter("segments", &kaleidoscopeSegments, 0.0f, 16.0f);
    }
    if (noiseWarp->getEnabled()) {
        gui.registerLabel("noiseWarp");
        gui.registerParameter("amplitude", &noiseWarpAmplitude, 0.0f, 1.0f);
        gui.registerParameter("frequency", &noiseWarpFrequency, 0.0f, 1.0f);
        gui.registerParameter("speed", &noiseWarpSpeed, 0.0f, 1.0f);
    }
    if (pixelate->getEnabled()) {
        gui.registerLabel("pixelate");
        gui.registerParameter("resolution", &pixelateResolution, ofVec2f(0, 0), ofVec2f(100, 100));
    }
    if (edges->getEnabled()) {
        gui.registerLabel("edges");
        gui.registerParameter("hue", &edgesHue, 0.0f, 1.0f);
        gui.registerParameter("saturation", &edgesSaturation, 0.0f, 1.0f);
    }
    if (vTiltShift->getEnabled()) {
        gui.registerLabel("vTiltShift");
        gui.registerParameter("V", &vTiltShiftV, 0.0f, 1.0f);
        gui.registerParameter("R", &vTiltShiftR, 0.0f, 1.0f);
    }
    if (hTiltShift->getEnabled()) {
        gui.registerLabel("hTiltShift");
        gui.registerParameter("H", &hTiltShiftH, 0.0f, 0.1f);
        gui.registerParameter("R", &hTiltShiftR, 0.0f, 0.1f);
    }
    if (godRay->getEnabled()) {
        gui.registerLabel("godRay");
        gui.registerParameter("lightDirDOTviewDir", &godRayLightDirDOTviewDir, 0.0f, 1.0f);
        gui.registerParameter("lightPoisitonOnScreen", &godRayLightPoisitonOnScreen, ofVec3f(-1, -1, -1), ofVec3f(1, 1, 1));
    }
    if (toon->getEnabled()) {
        gui.registerLabel("toon");
        gui.registerParameter("specularColor", &toonSpecularColor, ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1));
        gui.registerParameter("shinyness", &toonShinyness, 0.0f, 1.0f);
        gui.registerParameter("level", &toonLevel, 0.0f, 1.0f);
        gui.registerParameter("enableSpecular", &toonEnableSpecular);
        gui.registerParameter("edgeThreshold", &toonEdgeThreshold, 0.0f, 1.0f);
        gui.registerParameter("diffuseColor", &toonDiffuseColor, ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1));
        gui.registerParameter("ambientColor", &toonAmbientColor, ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1));
    }
    if (bleachBypass->getEnabled()) {
        gui.registerLabel("bleachBypass");
        gui.registerParameter("bypassOpacity", &bleachBypassOpacity, 0.0f, 1.0f);
    }
    if (contrast->getEnabled()) {
        gui.registerLabel("contrast");
        gui.registerParameter("brightness", &contrastBrightness, 0.0f, 1.0f);
        gui.registerParameter("contrast", &contrastContrast, 0.0f, 1.0f);
    }
    if (convolve->getEnabled()) {
        gui.registerLabel("convolve");
    }
    if (fakeSSS->getEnabled()) {
        gui.registerLabel("fakeSSS");
        gui.registerParameter("attenuationOffset", &fakeSSSAttenuationOffset, 0.0f, 1.0f);
        gui.registerParameter("baseColor", &fakeSSSBaseColor, ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1));
        gui.registerParameter("extinctionCoefficient", &fakeSSSExtinctionCoefficient, ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1));
        gui.registerParameter("lightColor", &fakeSSSLightColor, ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1));
        gui.registerParameter("lightPosition", &fakeSSSLightPosition, ofPoint(0, 0), ofPoint(1, 1));
        gui.registerParameter("materialThickness", &fakeSSSMaterialThickness, 0.0f, 1.0f);
        gui.registerParameter("rimScale", &fakeSSSRimScale, 0.0f, 1.0f);
        gui.registerParameter("specular", &fakeSSSSpecular, 0.0f, 1.0f);
        gui.registerParameter("specularColor", &fakeSSSSpecularColor, ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1));
    }
    if (limbDarken->getEnabled()) {
        gui.registerLabel("limbDarken");
        gui.registerParameter("brightness", &limbDarkenBrightness, 0.0f, 1.0f);
        gui.registerParameter("endColor", &limbDarkenEndColor, ofVec3f(0, 0, 0), ofVec3f(1, 1, 1));
        gui.registerParameter("radialScale", &limbDarkenRadialScale, 0.0f, 1.0f);
        gui.registerParameter("startColor", &limbDarkenStartColor, ofVec3f(0, 0, 0), ofVec3f(1, 1, 1));
    }
    if (shift->getEnabled()) {
        gui.registerLabel("shift");
        gui.registerParameter("amount", &shiftAmount, 0.0f, 1.0f);
        gui.registerParameter("angle", &shiftAngle, 0.0f, 1.0f);
    }
    if (rimHighlight->getEnabled()) {
        gui.registerLabel("rimHighlight");
    }
    if (ssao->getEnabled()) {
        gui.registerLabel("ssao");
        gui.registerParameter("cameraNear", &ssaoCameraNear, 0.0f, 1.0f);
        gui.registerParameter("cameraFar", &ssaoCameraFar, 0.0f, 1.0f);
        gui.registerParameter("fogNear", &ssaoFogNear, 0.0f, 1.0f);
        gui.registerParameter("fogFar", &ssaoFogFar, 0.0f, 1.0f);
        gui.registerParameter("fogEnabled", &ssaoFogEnabled);
        gui.registerParameter("onlyAO", &ssaoOnlyAO);
        gui.registerParameter("aoClamp", &ssaoAoClamp, 0.0f, 1.0f);
        gui.registerParameter("lumInfluence", &ssaoLumInfluence, 0.0f, 1.0f);
    }
    if (zoomBlur->getEnabled()) {
        gui.registerLabel("zoomBlur");
        gui.registerParameter("centerX", &zoomBlurCenterX, 0.0f, 1.0f);
        gui.registerParameter("centerY", &zoomBlurCenterY, 0.0f, 1.0f);
        gui.registerParameter("exposure", &zoomBlurExposure, 0.0f, 1.0f);
        gui.registerParameter("decay", &zoomBlurDecay, 0.0f, 1.0f);
        gui.registerParameter("density", &zoomBlurDensity, 0.0f, 1.0f);
        gui.registerParameter("weight", &zoomBlurWeight, 0.0f, 1.0f);
        gui.registerParameter("clamp", &zoomBlurClamp, 0.0f, 1.0f);
    }
}

//-----------
void PostProcessingLayer::render() {
    fbo.begin();
        post.begin();
            texLayer->draw(0, 0);
        post.end();
    fbo.end();
}

//-----------
PostProcessingLayer::~PostProcessingLayer() {
    // remove toggle listeners
    aliasEnabled.removeListener(this, &PostProcessingLayer::programChange);
    bloomEnabled.removeListener(this, &PostProcessingLayer::programChange);
    dofEnabled.removeListener(this, &PostProcessingLayer::programChange);
    kaleidoscopeEnabled.removeListener(this, &PostProcessingLayer::programChange);
    noiseWarpEnabled.removeListener(this, &PostProcessingLayer::programChange);
    pixelateEnabled.removeListener(this, &PostProcessingLayer::programChange);
    edgesEnabled.removeListener(this, &PostProcessingLayer::programChange);
    vTiltShiftEnabled.removeListener(this, &PostProcessingLayer::programChange);
    hTiltShiftEnabled.removeListener(this, &PostProcessingLayer::programChange);
    godRayEnabled.removeListener(this, &PostProcessingLayer::programChange);
    toonEnabled.removeListener(this, &PostProcessingLayer::programChange);
    bleachBypassEnabled.removeListener(this, &PostProcessingLayer::programChange);
    contrastEnabled.removeListener(this, &PostProcessingLayer::programChange);
    convolveEnabled.removeListener(this, &PostProcessingLayer::programChange);
    fakeSSSEnabled.removeListener(this, &PostProcessingLayer::programChange);
    limbDarkenEnabled.removeListener(this, &PostProcessingLayer::programChange);
    shiftEnabled.removeListener(this, &PostProcessingLayer::programChange);
    rimHighlightEnabled.removeListener(this, &PostProcessingLayer::programChange);
    ssaoEnabled.removeListener(this, &PostProcessingLayer::programChange);
    zoomBlurEnabled.removeListener(this, &PostProcessingLayer::programChange);
    // remove parameter listeners
    dofAperture.removeListener(this, &PostProcessingLayer::dofApertureChanged);
    dofFocus.removeListener(this, &PostProcessingLayer::dofFocusChanged);
    dofMaxBlur.removeListener(this, &PostProcessingLayer::dofMaxBlurChanged);
    kaleidoscopeSegments.removeListener(this, &PostProcessingLayer::kaleidoscopeSegmentsChanged);
    noiseWarpAmplitude.removeListener(this, &PostProcessingLayer::noiseWarpAmplitudeChanged);
    noiseWarpFrequency.removeListener(this, &PostProcessingLayer::noiseWarpFrequencyChanged);
    noiseWarpSpeed.removeListener(this, &PostProcessingLayer::noiseWarpSpeedChanged);
    pixelateResolution.removeListener(this, &PostProcessingLayer::pixelateResolutionChanged);
    edgesHue.removeListener(this, &PostProcessingLayer::edgesHueChanged);
    edgesSaturation.removeListener(this, &PostProcessingLayer::edgesSaturationChanged);
    vTiltShiftV.removeListener(this, &PostProcessingLayer::vTiltShiftVChanged);
    vTiltShiftR.removeListener(this, &PostProcessingLayer::vTiltShiftRChanged);
    hTiltShiftH.removeListener(this, &PostProcessingLayer::hTiltShiftHChanged);
    hTiltShiftR.removeListener(this, &PostProcessingLayer::hTiltShiftRChanged);
    godRayLightDirDOTviewDir.removeListener(this, &PostProcessingLayer::godRayLightDirDOTviewDirChanged);
    godRayLightPoisitonOnScreen.removeListener(this, &PostProcessingLayer::godRayLightPoisitonOnScreenChanged);
    toonSpecularColor.removeListener(this, &PostProcessingLayer::toonSpecularColorChanged);
    toonShinyness.removeListener(this, &PostProcessingLayer::toonShinynessChanged);
    toonLevel.removeListener(this, &PostProcessingLayer::toonLevelChanged);
    toonEnableSpecular.removeListener(this, &PostProcessingLayer::toonEnableSpecularChanged);
    toonEdgeThreshold.removeListener(this, &PostProcessingLayer::toonEdgeThresholdChanged);
    toonDiffuseColor.removeListener(this, &PostProcessingLayer::toonDiffuseColorChanged);
    toonAmbientColor.removeListener(this, &PostProcessingLayer::toonAmbientColorChanged);
    bleachBypassOpacity.removeListener(this, &PostProcessingLayer::bleachBypassOpacityChanged);
    contrastBrightness.removeListener(this, &PostProcessingLayer::contrastBrightnessChanged);
    contrastContrast.removeListener(this, &PostProcessingLayer::contrastContrastChanged);
    fakeSSSAttenuationOffset.removeListener(this, &PostProcessingLayer::fakeSSSAttenuationOffsetChanged);
    fakeSSSBaseColor.removeListener(this, &PostProcessingLayer::fakeSSSBaseColorChanged);
    fakeSSSExtinctionCoefficient.removeListener(this, &PostProcessingLayer::fakeSSSExtinctionCoefficientChanged);
    fakeSSSLightColor.removeListener(this, &PostProcessingLayer::fakeSSSLightColorChanged);
    fakeSSSLightPosition.removeListener(this, &PostProcessingLayer::fakeSSSLightPositionChanged);
    fakeSSSMaterialThickness.removeListener(this, &PostProcessingLayer::fakeSSSMaterialThicknessChanged);
    fakeSSSRimScale.removeListener(this, &PostProcessingLayer::fakeSSSRimScaleChanged);
    fakeSSSSpecular.removeListener(this, &PostProcessingLayer::fakeSSSSpecularChanged);
    fakeSSSSpecularColor.removeListener(this, &PostProcessingLayer::fakeSSSSpecularColorChanged);
    limbDarkenBrightness.removeListener(this, &PostProcessingLayer::limbDarkenBrightnessChanged);
    limbDarkenEndColor.removeListener(this, &PostProcessingLayer::limbDarkenEndColorChanged);
    limbDarkenRadialScale.removeListener(this, &PostProcessingLayer::limbDarkenRadialScaleChanged);
    limbDarkenStartColor.removeListener(this, &PostProcessingLayer::limbDarkenStartColorChanged);
    shiftAmount.removeListener(this, &PostProcessingLayer::shiftAmountChanged);
    shiftAngle.removeListener(this, &PostProcessingLayer::shiftAngleChanged);
    ssaoCameraNear.removeListener(this, &PostProcessingLayer::ssaoCameraNearChanged);
    ssaoCameraFar.removeListener(this, &PostProcessingLayer::ssaoCameraFarChanged);
    ssaoFogNear.removeListener(this, &PostProcessingLayer::ssaoFogNearChanged);
    ssaoFogFar.removeListener(this, &PostProcessingLayer::ssaoFogFarChanged);
    ssaoFogEnabled.removeListener(this, &PostProcessingLayer::ssaoFogEnabledChanged);
    ssaoOnlyAO.removeListener(this, &PostProcessingLayer::ssaoOnlyAOChanged);
    ssaoAoClamp.removeListener(this, &PostProcessingLayer::ssaoAoClampChanged);
    ssaoLumInfluence.removeListener(this, &PostProcessingLayer::ssaoLumInfluenceChanged);
    zoomBlurCenterX.removeListener(this, &PostProcessingLayer::zoomBlurCenterXChanged);
    zoomBlurCenterY.removeListener(this, &PostProcessingLayer::zoomBlurCenterYChanged);
    zoomBlurExposure.removeListener(this, &PostProcessingLayer::zoomBlurExposureChanged);
    zoomBlurDecay.removeListener(this, &PostProcessingLayer::zoomBlurDecayChanged);
    zoomBlurDensity.removeListener(this, &PostProcessingLayer::zoomBlurDensityChanged);
    zoomBlurWeight.removeListener(this, &PostProcessingLayer::zoomBlurWeightChanged);
    zoomBlurClamp.removeListener(this, &PostProcessingLayer::zoomBlurClampChanged);
}
