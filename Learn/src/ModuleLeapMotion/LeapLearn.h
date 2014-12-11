#pragma once

#include "ofMain.h"
#include "LeapMotion.h"
#include "Learn.h"


class LeapLearn : public Learn
{
public:
    ~LeapLearn();
    LeapLearn();
    
    void update();
    void draw();
        
    void setGuiPosition(int x, int y);
    void setVisible(bool visible);
    void toggleVisible() {setVisible(!visible);}
    void toggleGuiSummaryView();

protected:
    
    void setupGuiInputs();
    void guiInputEvent(ofxUIEventArgs &e);

    void addLeftHandFingerTipsAsInput();
    void addLeftHandIndexFingerTipAsInput();
    void addLeftHandVelocityAsInput();
    void addLeftOpenHandSizeAsInput();
    void addLeftHandNormalAsInput();
    void addLeftHandDirectionAsInput();
    void addLeftHandRollAsInput();
    void addLeftHandPitchAsInput();
    void addLeftHandYawAsInput();
    
    void addRightHandFingerTipsAsInput();
    void addRightHandIndexFingerTipAsInput();
    void addRightHandVelocityAsInput();
    void addRightOpenHandSizeAsInput();
    void addRightHandNormalAsInput();
    void addRightHandDirectionAsInput();
    void addRightHandRollAsInput();
    void addRightHandPitchAsInput();
    void addRightHandYawAsInput();
    
    LeapMotion leap;
    
    bool visible;
    int x, y;
    
    ofxUICanvas *guiInputs;
    
    bool lTips, rTips;
    bool lIndex, rIndex;
    bool lVel, rVel;
    bool lSize, rSize;
    bool lNormal, rNormal;
    bool lDir, rDir;
    bool lRoll, rRoll;
    bool lPitch, rPitch;
    bool lYaw, rYaw;

};
