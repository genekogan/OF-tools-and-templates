#pragma once

#include "Canvas.h"
#include "ofxPostProcessing.h"
#include "Control.h"


class PostProcessingLayer : public CanvasLayer
{
public:
    ~PostProcessingLayer();
    void setup();
    void setGuiPosition(int x, int y);
    void toggleVisible();

private:

    void programChange(bool &b);
    void buildGui();
    void render();

    void dofApertureChanged(float &v) {dof->setAperture(dofAperture);}
    void dofFocusChanged(float &v) {dof->setFocus(dofFocus);}
    void dofMaxBlurChanged(float &v) {dof->setMaxBlur(dofMaxBlur);}
    void kaleidoscopeSegmentsChanged(float &v) {kaleidoscope->setSegments(kaleidoscopeSegments);}
    void noiseWarpAmplitudeChanged(float &v) {noiseWarp->setAmplitude(noiseWarpAmplitude);}
    void noiseWarpFrequencyChanged(float &v) {noiseWarp->setFrequency(noiseWarpFrequency);}
    void noiseWarpSpeedChanged(float &v) {noiseWarp->setSpeed(noiseWarpSpeed);}
    void pixelateResolutionChanged(ofVec2f &v) {pixelate->setResolution(pixelateResolution);}
    void edgesHueChanged(float &v) {edges->setHue(edgesHue);}
    void edgesSaturationChanged(float &v) {edges->setSaturation(edgesSaturation);}
    void vTiltShiftVChanged(float &v) {vTiltShift->setV(vTiltShiftV);}
    void vTiltShiftRChanged(float &v) {vTiltShift->setR(vTiltShiftR);}
    void hTiltShiftHChanged(float &v) {hTiltShift->setH(hTiltShiftH);}
    void hTiltShiftRChanged(float &v) {hTiltShift->setR(hTiltShiftR);}
    void godRayLightDirDOTviewDirChanged(float &v) {godRay->setLightDirDOTviewDir(godRayLightDirDOTviewDir);}
    void godRayLightPoisitonOnScreenChanged(ofVec3f &v) {godRay->setLightPositionOnScreen(godRayLightPoisitonOnScreen);}
    void toonSpecularColorChanged(ofVec4f &v) {toon->setSpecularColor(toonSpecularColor);}
    void toonShinynessChanged(float &v) {toon->setShinyness(toonShinyness);}
    void toonLevelChanged(float &v) {toon->setLevel(toonLevel);}
    void toonEnableSpecularChanged(bool &v) {toon->setEnableSpecular(toonEnableSpecular);}
    void toonEdgeThresholdChanged(float &v) {toon->setEdgeThreshold(toonEdgeThreshold);}
    void toonDiffuseColorChanged(ofVec4f &v) {toon->setDiffuseColor(toonDiffuseColor);}
    void toonAmbientColorChanged(ofVec4f &v) {toon->setAmbientColor(toonAmbientColor);}
    void bleachBypassOpacityChanged(float &v) {bleachBypass->setOpacity(bleachBypassOpacity);}
    void contrastBrightnessChanged(float &v) {contrast->setBrightness(contrastBrightness);}
    void contrastContrastChanged(float &v) {contrast->setContrast(contrastContrast);}
    void fakeSSSAttenuationOffsetChanged(float &v) {fakeSSS->setAttenuationOffset(fakeSSSAttenuationOffset);}
    void fakeSSSBaseColorChanged(ofVec4f &v) {fakeSSS->setBaseColor(fakeSSSBaseColor);}
    void fakeSSSExtinctionCoefficientChanged(ofVec4f &v) {fakeSSS->setExtinctionCoefficient(fakeSSSExtinctionCoefficient);}
    void fakeSSSLightColorChanged(ofVec4f &v) {fakeSSS->setLightColor(fakeSSSLightColor);}
    void fakeSSSLightPositionChanged(ofPoint &v) {fakeSSS->setLightPosition(fakeSSSLightPosition);}
    void fakeSSSMaterialThicknessChanged(float &v) {fakeSSS->setMaterialThickness(fakeSSSMaterialThickness);}
    void fakeSSSRimScaleChanged(float &v) {fakeSSS->setRimScale(fakeSSSRimScale);}
    void fakeSSSSpecularChanged(float &v) {fakeSSS->setSpecular(fakeSSSSpecular);}
    void fakeSSSSpecularColorChanged(ofVec4f &v) {fakeSSS->setSpecularColor(fakeSSSSpecularColor);}
    void limbDarkenBrightnessChanged(float &v) {limbDarken->setBrightness(limbDarkenBrightness);}
    void limbDarkenEndColorChanged(ofVec3f &v) {limbDarken->setEndColor(limbDarkenEndColor);}
    void limbDarkenRadialScaleChanged(float &v) {limbDarken->setRadialScale(limbDarkenRadialScale);}
    void limbDarkenStartColorChanged(ofVec3f &v) {limbDarken->setStartColor(limbDarkenStartColor);}
    void shiftAmountChanged(float &v) {shift->setAmount(shiftAmount);}
    void shiftAngleChanged(float &v) {shift->setAngle(shiftAngle);}
    void ssaoCameraNearChanged(float &v) {ssao->setCameraNear(ssaoCameraNear);}
    void ssaoCameraFarChanged(float &v) {ssao->setCameraFar(ssaoCameraFar);}
    void ssaoFogNearChanged(float &v) {ssao->setFogNear(ssaoFogNear);}
    void ssaoFogFarChanged(float &v) {ssao->setFogFar(ssaoFogFar);}
    void ssaoFogEnabledChanged(bool &v) {ssao->setFogEnabled(ssaoFogEnabled);}
    void ssaoOnlyAOChanged(bool &v) {ssao->setOnlyAO(ssaoOnlyAO);}
    void ssaoAoClampChanged(float &v) {ssao->setAoClamp(ssaoAoClamp);}
    void ssaoLumInfluenceChanged(float &v) {ssao->setLumInfluence(ssaoLumInfluence);}
    void zoomBlurCenterXChanged(float &v) {zoomBlur->setCenterX(zoomBlurCenterX);}
    void zoomBlurCenterYChanged(float &v) {zoomBlur->setCenterY(zoomBlurCenterY);}
    void zoomBlurExposureChanged(float &v) {zoomBlur->setExposure(zoomBlurExposure);}
    void zoomBlurDecayChanged(float &v) {zoomBlur->setDecay(zoomBlurDecay);}
    void zoomBlurDensityChanged(float &v) {zoomBlur->setDensity(zoomBlurDensity);}
    void zoomBlurWeightChanged(float &v) {zoomBlur->setWeight(zoomBlurWeight);}
    void zoomBlurClampChanged(float &v) {zoomBlur->setClamp(zoomBlurClamp);}
    
    
    Control gui;

    ofxPostProcessing post;
    
    FxaaPass::Ptr alias;
    BloomPass::Ptr bloom;
    DofPass::Ptr dof;
    KaleidoscopePass::Ptr kaleidoscope;
    NoiseWarpPass::Ptr noiseWarp;
    PixelatePass::Ptr pixelate;
    EdgePass::Ptr edges;
    VerticalTiltShifPass::Ptr vTiltShift;
    HorizontalTiltShifPass::Ptr hTiltShift;
    GodRaysPass::Ptr godRay;
    ToonPass::Ptr toon;
    BleachBypassPass::Ptr bleachBypass;
    ContrastPass::Ptr contrast;
    ConvolutionPass::Ptr convolve;
    FakeSSSPass::Ptr fakeSSS;
    LimbDarkeningPass::Ptr limbDarken;
    RGBShiftPass::Ptr shift;
    RimHighlightingPass::Ptr rimHighlight;
    SSAOPass::Ptr ssao;
    ZoomBlurPass::Ptr zoomBlur;
    
    
    /* toggles */
    
    ofxParameter<bool> aliasEnabled;
    ofxParameter<bool> bloomEnabled;
    ofxParameter<bool> dofEnabled;
    ofxParameter<bool> kaleidoscopeEnabled;
    ofxParameter<bool> noiseWarpEnabled;
    ofxParameter<bool> pixelateEnabled;
    ofxParameter<bool> edgesEnabled;
    ofxParameter<bool> vTiltShiftEnabled;
    ofxParameter<bool> hTiltShiftEnabled;
    ofxParameter<bool> godRayEnabled;
    ofxParameter<bool> toonEnabled;
    ofxParameter<bool> bleachBypassEnabled;
    ofxParameter<bool> contrastEnabled;
    ofxParameter<bool> convolveEnabled;
    ofxParameter<bool> fakeSSSEnabled;
    ofxParameter<bool> limbDarkenEnabled;
    ofxParameter<bool> shiftEnabled;
    ofxParameter<bool> rimHighlightEnabled;
    ofxParameter<bool> ssaoEnabled;
    ofxParameter<bool> zoomBlurEnabled;
    

    /* parameters */
    
    ofxParameter<float> dofAperture;
    ofxParameter<float> dofFocus;
    ofxParameter<float> dofMaxBlur;
    
    ofxParameter<float> kaleidoscopeSegments;
    
    ofxParameter<float> noiseWarpAmplitude;
    ofxParameter<float> noiseWarpFrequency;
    ofxParameter<float> noiseWarpSpeed;
    
    ofxParameter<ofVec2f> pixelateResolution;
    
    ofxParameter<float> edgesHue;
    ofxParameter<float> edgesSaturation;
    
    ofxParameter<float> vTiltShiftV;
    ofxParameter<float> vTiltShiftR;
    
    ofxParameter<float> hTiltShiftH;
    ofxParameter<float> hTiltShiftR;
    
    ofxParameter<float> godRayLightDirDOTviewDir;
    ofxParameter<ofVec3f> godRayLightPoisitonOnScreen;
    
    ofxParameter<ofVec4f> toonSpecularColor;
    ofxParameter<float> toonShinyness;
    ofxParameter<float> toonLevel;
    ofxParameter<bool> toonEnableSpecular;
    ofxParameter<float> toonEdgeThreshold;
    ofxParameter<ofVec4f> toonDiffuseColor;
    ofxParameter<ofVec4f> toonAmbientColor;
    
    ofxParameter<float> bleachBypassOpacity;
    
    ofxParameter<float> contrastBrightness;
    ofxParameter<float> contrastContrast;
    
    ofxParameter<float> fakeSSSAttenuationOffset;
    ofxParameter<ofVec4f> fakeSSSBaseColor;
    ofxParameter<ofVec4f> fakeSSSExtinctionCoefficient;
    ofxParameter<ofVec4f> fakeSSSLightColor;
    ofxParameter<ofPoint> fakeSSSLightPosition;
    ofxParameter<float> fakeSSSMaterialThickness;
    ofxParameter<float> fakeSSSRimScale;
    ofxParameter<float> fakeSSSSpecular;
    ofxParameter<ofVec4f> fakeSSSSpecularColor;
    
    ofxParameter<float> limbDarkenBrightness;
    ofxParameter<ofVec3f> limbDarkenEndColor;
    ofxParameter<float> limbDarkenRadialScale;
    ofxParameter<ofVec3f> limbDarkenStartColor;
    
    ofxParameter<float> shiftAmount;
    ofxParameter<float> shiftAngle;
    
    ofxParameter<float> ssaoCameraNear;
    ofxParameter<float> ssaoCameraFar;
    ofxParameter<float> ssaoFogNear;
    ofxParameter<float> ssaoFogFar;
    ofxParameter<bool> ssaoFogEnabled;
    ofxParameter<bool> ssaoOnlyAO;
    ofxParameter<float> ssaoAoClamp;
    ofxParameter<float> ssaoLumInfluence;
    
    ofxParameter<float> zoomBlurCenterX;
    ofxParameter<float> zoomBlurCenterY;
    ofxParameter<float> zoomBlurExposure;
    ofxParameter<float> zoomBlurDecay;
    ofxParameter<float> zoomBlurDensity;
    ofxParameter<float> zoomBlurWeight;
    ofxParameter<float> zoomBlurClamp;
};