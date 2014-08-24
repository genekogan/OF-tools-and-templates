#pragma once

#include "ofMain.h"
#include "Scene.h"
#include "TimeFunction.h"
#include "ofxBlurShader.h"


class BubbleCreator : public ofThread
{
public:

    void setup(vector<TimeFunction<ofVec2f> *> &position,
               vector<ofVec3f> &colorMargin,
               vector<float> &blurLevel,
               vector<float> &alpha,
               vector<float> &size,
               ofxParameter<int> &numBubbles) {
        this->position = &position;
        this->colorMargin = &colorMargin;
        this->blurLevel = &blurLevel;
        this->alpha = &alpha;
        this->size = &size;
        this->numBubbles = &numBubbles;
    }
    
    void setRunning(bool b) {
        if (b != isThreadRunning()) {
            isThreadRunning() ? stopThread() : startThread();
        }
    }
    
    void threadedFunction() {
        while(isThreadRunning()) {
            if(lock()) {
                while (position->size() < numBubbles->get()) {
                    TimeFunction<ofVec2f> *newPosition = new TimeFunction<ofVec2f>(false);
                    newPosition->setConstant(ofVec2f(ofRandom(ofGetWidth()), ofRandom(ofGetHeight())));
                    position->push_back(newPosition);
                    blurLevel->push_back(ofRandom(1));
                    alpha->push_back(0);
                    size->push_back(0);
                    colorMargin->push_back(ofVec3f(ofRandom(-1,1), ofRandom(-1,1), ofRandom(-1,1)));
                }
                unlock();
            }
        }
    }
    
protected:
    
    vector<TimeFunction<ofVec2f> *> *position;
    vector<ofVec3f> *colorMargin;
    vector<float> *blurLevel;
    vector<float> *alpha;
    vector<float> *size;
    ofxParameter<int> *numBubbles;

};




class Bubbles : public Scene
{
public:
    ~Bubbles();
    void setup();
    void update();
    void draw();
    
private:
    void fboParametersChanged(int & newMaxPasses);
    void setupBubblesFbo();
    
    ofxBlurShader blur;
    vector<ofFbo> bubbleFbo;

    // main parameters
    ofxParameter<float> speed;
    ofxParameter<int> numBubbles;
    ofxParameter<int> maxSizeInitial;
    ofxParameter<int> maxSize;
    ofxParameter<ofColor> baseColor;
    ofxParameter<ofVec3f> varColor;
    ofxParameter<ofVec2f> positionNoiseMargin;
    ofxParameter<ofVec2f> positionNoiseFactor;
    ofxParameter<ofVec2f> positionSineMargin;
    ofxParameter<ofVec2f> positionSineFreq;
    ofxParameter<ofVec2f> positionTimeConstant;
    ofxParameter<int> maxPasses;
    ofxParameter<int> blurAmt;
    
    // for each bubble
    vector<TimeFunction<ofVec2f> *> position;
    vector<ofVec3f> colorMargin;
    vector<float> blurLevel;
    vector<float> alpha;
    vector<float> size;
    
    float time;
    BubbleCreator bubbleCreator;
};