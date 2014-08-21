#pragma once

#include "ofMain.h"
#include "Scene.h"


struct ShaderParameterBase {
    virtual string getName() { }
    virtual void update(ofShader *shader) {};
};

template<typename T>
struct ShaderParameter : public ShaderParameterBase {
    ShaderParameter(ofxParameter<T> *val) {
        value = val;
        name = value->getName();
    }
    void update(ofShader *shader);
    string getName() { return name; }
    string name;
    ofParameter<T> *value;
};

template<> inline
void ShaderParameter<int>::update(ofShader *shader) {
    shader->setUniform1f(name, *value);
}
template<> inline
void ShaderParameter<float>::update(ofShader *shader) {
    shader->setUniform1f(name, *value);
}
template<> inline
void ShaderParameter<ofVec2f>::update(ofShader *shader) {
    shader->setUniform2f(name, value->get().x, value->get().y);
}
template<> inline
void ShaderParameter<ofVec3f>::update(ofShader *shader) {
    shader->setUniform3f(name, value->get().x, value->get().y, value->get().z);
}
template<> inline
void ShaderParameter<ofColor>::update(ofShader *shader) {
    shader->setUniform3f(name, value->get().r / 255.0, value->get().g / 255.0, value->get().b / 255.0);
}



class Shader : public Scene
{
public:
    void setup();
    void setShader(string vert, string frag);
    
    void update(ofFbo *fboTex);
    void update() { update(NULL); }
    void draw();
    
    void addParameter(string name, float min, float max);
    void addParameter(string name, ofVec2f min, ofVec2f max);
    void addParameter(string name, ofVec3f min, ofVec3f max);
    void addParameter(string name, ofColor min, ofColor max);
    
    ofShader shader;
    ofFbo fbo;
    vector<ShaderParameterBase *> shaderParameters;
    ofxParameter<bool> clearFbo;
    
    /* color presets */
    void setupBlobby();
    void setupBits();
    void setupBitsExperimental();
    void setupElectro();
    void setupEye();
    void setupHerokuBubbles();
    void setupLandscape();
    void setupMonjori();
    void setupNebula();
    void setupNoisy();
    void setupRain();
    void setupSinewave();
    void setupSinewaveExperimental();
    void setupWave();
    void setupFractalScope();
    void setupFractalFlower();
    void setupCurtains();
    
    /* texture presets */
    void setupBrCoSa();
    void setupPixelate();
    void setupBilateralFilter();
    void setupBlur();
    void setupChannels();
    void setupDeform();
    void setupEdges();
    void setupHalftoneCmyk();
    void setupHalftone();
    void setupHue();
    void setupInvert();
    void setupNeon();
    void setupPatches();
    void setupPixelRolls();
    void setupThreshold();
    void setupWrap();
};
