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
    void addRightHandFingerTipsAsInput();
    
    void setGuiPosition(int x, int y);
    void setVisible(bool visible);
    void toggleVisible() {setVisible(!visible);}
    void toggleGuiSummaryView();

protected:
    
    LeapMotion leap;
    
    bool visible;
    int x, y;
};
