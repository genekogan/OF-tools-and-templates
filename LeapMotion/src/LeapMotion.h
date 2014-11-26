#pragma once

#include "ofxLeapMotion2.h"
#include "Control.h"


enum Handedness { LEFT, RIGHT };


class LeapMotion
{
public:
    ~LeapMotion();
    void              setup();
    void              setGuiPosition(int x, int y);
    void              close() { leap.close(); }
    void              setVelocityTracking(bool trackVelocity, int numFrames=10);
    void              update();
    void              draw(int x, int y, int w, int h);
    void              drawVelocityGraph(Handedness hand, int x, int y, int w, int h);
    
    // left hand getters
    vector<ofPoint> * getLeftHandFingerTipsRef() {return &leftHandTips;}
    ofPoint *         getLeftHandIndexFingerTipRef() {return &leftHandTips[1];}
    ofPoint *         getLeftHandVelocityRef() {return &handVelocity[0];}
    float *           getLeftOpenHandSizeRef() {return &lOpenHandSize;}
    ofPoint *         getLeftHandNormalRef() {return &lHandNormal;}
    ofPoint *         getLeftHandDirectionRef() {return &lHandDirection;}
    float *           getLeftHandRollRef() {return &lHandRoll;}
    float *           getLeftHandPitchRef() {return &lHandPitch;}
    float *           getLeftHandYawRef() {return &lHandYaw;}

    // right hand getters
    vector<ofPoint> * getRightHandFingerTipsRef() {return &rightHandTips;}
    ofPoint *         getRightHandIndexFingerTipRef() {return &rightHandTips[1];}
    ofPoint *         getRightHandVelocityRef() {return &handVelocity[1];}
    float *           getRightOpenHandSizeRef() {return &rOpenHandSize;}
    ofPoint *         getRightHandNormalRef() {return &rHandNormal;}
    ofPoint *         getRightHandDirectionRef() {return &rHandDirection;}
    float *           getRightHandRollRef() {return &rHandRoll;}
    float *           getRightHandPitchRef() {return &rHandPitch;}
    float *           getRightHandYawRef() {return &rHandYaw;}

private:
    
    void              updateFingerTips(bool isNormalized=false);
    void              updateHandStats();

    // stats
    vector<ofPoint>   leftHandTips, rightHandTips;
    ofPoint           lHandNormal, lHandDirection;
    ofPoint           rHandNormal, rHandDirection;
    float             lOpenHandSize, lHandRoll, lHandPitch, lHandYaw;
    float             rOpenHandSize, rHandRoll, rHandPitch, rHandYaw;

    // velocity stats
    vector<ofPoint>   vLeftHandTips, vRightHandTips;
    ofPoint           vLHandNormal, vLHandDirection;
    ofPoint           vRHandNormal, vRHandDirection;
    float             vLOpenHandSize, vLHandRoll, vLHandPitch, vLHandYaw;
    float             vROpenHandSize, vRHandRoll, vRHandPitch, vRHandYaw;
    ofPoint           handVelocity[2];

    // internals
    ofxLeapMotion     leap;
	vector            <ofxLeapMotionSimpleHand> simpleHands;
    float             frameWeight;
    vector<ofPoint>   handVelocityList[2];
    ofPoint           prevHandPosition[2];
    int               idxFrame;
    ofEasyCam         cam;
    ofFbo             fbo;
    fingerType        finger[5];
    
    // parameters
    Control           control;
    bool              trackVelocity, pTrackVelocity;
    bool              normalizeTips;
    int               numFrames, pNumFrames;
    float             vLerpRate;
};
