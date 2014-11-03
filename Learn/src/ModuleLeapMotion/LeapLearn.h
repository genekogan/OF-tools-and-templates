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
    
protected:
    
    LeapMotion leap;
};
