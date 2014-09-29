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
               int &numBubbles,
               int width, int height) {
        this->position = &position;
        this->colorMargin = &colorMargin;
        this->blurLevel = &blurLevel;
        this->alpha = &alpha;
        this->size = &size;
        this->numBubbles = &numBubbles;
        this->width = width;
        this->height = height;
    }
    
    void setRunning(bool b) {
        if (b != isThreadRunning()) {
            isThreadRunning() ? stopThread() : startThread();
        }
    }
    
    void threadedFunction() {
        while(isThreadRunning()) {
            if(lock()) {
                while (position->size() < *numBubbles) {
                    TimeFunction<ofVec2f> *newPosition = new TimeFunction<ofVec2f>(false);
                    newPosition->setConstant(ofVec2f(ofRandom(width), ofRandom(height)));
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
    int *numBubbles;
    int width, height;
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
    float speed;
    int numBubbles;
    int maxSizeInitial;
    int maxSize;
    ofColor baseColor;
    ofVec3f varColor;
    ofVec2f positionNoiseMargin;
    ofVec2f positionNoiseFactor;
    ofVec2f positionSineMargin;
    ofVec2f positionSineFreq;
    ofVec2f positionTimeConstant;
    int maxPasses;
    int blurAmt;
    
    // for each bubble
    vector<TimeFunction<ofVec2f> *> position;
    vector<ofVec3f> colorMargin;
    vector<float> blurLevel;
    vector<float> alpha;
    vector<float> size;
    
    float time;
    BubbleCreator bubbleCreator;
};