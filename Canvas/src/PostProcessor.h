#pragma once

#include "Canvas.h"
#include "ofxPostProcessing.h"
#include "Control.h"


class PostProcessingLayer : public CanvasLayer
{
public:
    ~PostProcessingLayer();
    void setup();
    string getType() {return "PostProcessingLayer";}
    void render();
    
private:
    void setupParameterGui();
    void checkEffectActive(shared_ptr<RenderPass> pass, bool isEnabled);
    void update();

    void checkGuiCalls() {
        if (gui.headerSelected) {
            gui.headerSelected = false;
            gui.setVisible(false);
            control.setVisible(true);
        }
        if (control.headerSelected) {
            control.headerSelected = false;
            control.setVisible(false);
            gui.setVisible(true);
        }
    }
    
    void setGuiPosition(int x, int y) {
        this->guiPosition = ofPoint(x, y);
        control.setGuiPosition(x, y);
        gui.setGuiPosition(x, y);
    }
    
    void toggleVisible() {
        control.toggleVisible();
        gui.toggleVisible();
    }
    
    void setVisible(bool visible) {
        control.setVisible(visible);
        gui.setVisible(false);
    }
    
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
    ConvolutionPass::Ptr convolve;
    FakeSSSPass::Ptr fakeSSS;
    LimbDarkeningPass::Ptr limbDarken;
    RGBShiftPass::Ptr shift;
    RimHighlightingPass::Ptr rimHighlight;
    SSAOPass::Ptr ssao;
    ZoomBlurPass::Ptr zoomBlur;
    
    Control gui;
    
    /* toggles */
    bool aliasEnabled;
    bool bloomEnabled;
    bool dofEnabled;
    bool kaleidoscopeEnabled;
    bool noiseWarpEnabled;
    bool pixelateEnabled;
    bool edgesEnabled;
    bool vTiltShiftEnabled;
    bool hTiltShiftEnabled;
    bool godRayEnabled;
    bool toonEnabled;
    bool bleachBypassEnabled;
    bool convolveEnabled;
    bool fakeSSSEnabled;
    bool limbDarkenEnabled;
    bool shiftEnabled;
    bool rimHighlightEnabled;
    bool ssaoEnabled;
    bool zoomBlurEnabled;
    
    
    /* parameters */
    
    float dofAperture;
    float dofFocus;
    float dofMaxBlur;
    
    float kaleidoscopeSegments;
    
    float noiseWarpAmplitude;
    float noiseWarpFrequency;
    float noiseWarpSpeed;
    
    ofVec2f pixelateResolution;
    
    float edgesHue;
    float edgesSaturation;
    
    float vTiltShiftV;
    float vTiltShiftR;
    
    float hTiltShiftH;
    float hTiltShiftR;
    
    float godRayLightDirDOTviewDir;
    ofVec3f godRayLightPoisitonOnScreen;
    
    ofVec4f toonSpecularColor;
    float toonShinyness;
    float toonLevel;
    bool toonEnableSpecular;
    float toonEdgeThreshold;
    ofVec4f toonDiffuseColor;
    ofVec4f toonAmbientColor;
    
    float bleachBypassOpacity;
    
    float fakeSSSAttenuationOffset;
    ofVec4f fakeSSSBaseColor;
    ofVec4f fakeSSSExtinctionCoefficient;
    ofVec4f fakeSSSLightColor;
    ofPoint fakeSSSLightPosition;
    float fakeSSSMaterialThickness;
    float fakeSSSRimScale;
    float fakeSSSSpecular;
    ofVec4f fakeSSSSpecularColor;
    
    float limbDarkenBrightness;
    ofVec3f limbDarkenEndColor;
    float limbDarkenRadialScale;
    ofVec3f limbDarkenStartColor;
    
    float shiftAmount;
    float shiftAngle;
    
    float ssaoCameraNear;
    float ssaoCameraFar;
    float ssaoFogNear;
    float ssaoFogFar;
    bool ssaoFogEnabled;
    bool ssaoOnlyAO;
    float ssaoAoClamp;
    float ssaoLumInfluence;
    
    float zoomBlurCenterX;
    float zoomBlurCenterY;
    float zoomBlurExposure;
    float zoomBlurDecay;
    float zoomBlurDensity;
    float zoomBlurWeight;
    float zoomBlurClamp;

};
