#pragma once

#include "ofMain.h"
#include "Scene.h"
#include "TimeFunction.h"
#include "ofxBlurShader.h"

class Bubbles : public Scene
{
public:
    void setup();
    void update();
    void draw();
    
private:
    void fboParametersChanged(int & newMaxPasses);
    void setupBubblesFbo();
    void addNewBubble();
    
    ofxBlurShader blur;
    vector<ofFbo> bubbleFbo;

    // main parameters
    ofxParameter<float> speed;
    ofxParameter<int> numBubbles;
    ofxParameter<int> maxSizeInitial;
    ofxParameter<int> maxSize;
    ofxParameter<ofColor> baseColor;
    ofxParameter<ofVec3f> varColor;
    ofxParameter<int> maxPasses;
    ofxParameter<int> blurAmt;
    
    // for each bubble
    vector<TimeFunction<ofPoint> *> position;
    vector<ofVec3f> colorMargin;
    vector<float> blurLevel;
    vector<float> alpha;
    vector<float> size;
    
    float time;
};