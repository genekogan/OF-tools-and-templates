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
    
    /*
    void addAllPadsAsInput();
    void addSlidersAsInput();
    void addButtonsAsInput();
    void addNumFingersAsInput();
    void addPadSumAsInput();
    void addPadAverageAsInput();
    void addPerimterAsInput();
    void addAverageInterFingerDistanceAsInput();
    void addCentroidAsInput();
    void addWeightedCentroidAsInput();
     */
    
protected:
    
    LeapMotion leap;
};
