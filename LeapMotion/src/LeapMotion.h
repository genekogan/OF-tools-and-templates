#pragma once

#include "ofxLeapMotion2.h"
#include "Control.h"


enum Handedness { LEFT, RIGHT };


class LeapMotion
{
public:
    ~LeapMotion();
    void            setup();
    void            setGuiPosition(int x, int y);
    void            close() { leap.close(); }
    void            setVelocityTracking(bool trackVelocity, int numFrames=10);
    void            update();
    void            draw(int x, int y, int w, int h);
    void            drawVelocityGraph(Handedness hand, int x, int y, int w, int h);
    
    vector<ofPoint> getFingerTips(Handedness hand, bool isNormalized=false);
    ofPoint         getFingerTip(Handedness hand, fingerType finger);
    ofPoint         getHandVelocity(Handedness hand);
    float           getOpenHandSize(Handedness hand);
    ofPoint         getHandNormal(Handedness hand);
    ofPoint         getHandDirection(Handedness hand);
    float           getHandRoll(Handedness hand);
    float           getHandPitch(Handedness hand);
    float           getHandYaw(Handedness hand);
    
    
    vector<ofPoint> * getLeftHandFingerTipsRef() { return &leftHandTips; }
    vector<ofPoint> * getRightHandFingerTipsRef() { return &rightHandTips; }
    
private:
    
    void            updateFingerTips(bool isNormalized=false);

    vector<ofPoint> leftHandTips, rightHandTips;
    
    

    ofEasyCam       cam;
    ofFbo           fbo;
    
    ofxLeapMotion   leap;
	vector          <ofxLeapMotionSimpleHand> simpleHands;
    
    float           frameWeight;
    ofPoint         handVelocity[2];
    vector<ofPoint> handVelocityList[2];
    ofPoint         prevHandPosition[2];
    int             idxFrame;
    
    /* parameters */
    Control         control;
    bool            trackVelocity, pTrackVelocity;
    int             numFrames, pNumFrames;
};
