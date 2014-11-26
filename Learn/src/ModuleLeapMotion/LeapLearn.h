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
    
    void setGuiPosition(int x, int y);
    void setVisible(bool visible);
    void toggleVisible() {setVisible(!visible);}
    void toggleGuiSummaryView();

protected:
    
    LeapMotion leap;
    
    bool visible;
    int x, y;
};
