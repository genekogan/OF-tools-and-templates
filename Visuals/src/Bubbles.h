#pragma once

#include "ofMain.h"
#include "Scene.h"
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
    ofParameter<float> speed;
    ofParameter<int> numBubbles;
    ofParameter<int> maxSizeInitial;
    ofParameter<int> maxSize;
    ofParameter<ofColor> baseColor;
    ofParameter<ofVec3f> varColor;
    ofParameter<int> maxPasses;
    ofParameter<int> blurAmt;
    
    // for each bubble
    vector<ofPoint> centerPosition;
    vector<ofPoint> position;
    vector<ofVec3f> colorMargin;
    vector<float> blurLevel;
    vector<float> alpha;
    vector<float> size;

    float time;
    
    
    
    float getFieldValue(ofVec2f p) {
        
        return sqrt(pow(p.x - width/2, 2) + pow(p.y - height/2, 2));
        
    }
};