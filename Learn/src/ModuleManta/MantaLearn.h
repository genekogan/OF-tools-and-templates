#pragma once

#include "ofMain.h"
#include "MantaController.h"
#include "Learn.h"


class MantaLearn : public Learn
{
public:
    ~MantaLearn();
    MantaLearn();
    
    void update();
    void draw();
    
    void addAllPadsAsInput();
    void addSlidersAsInput();
    void addButtonsAsInput();
    void addNumFingersAsInput();
    void addPadSumAsInput();
    void addPadAverageAsInput();
    void addPerimeterAsInput();
    void addAverageInterFingerDistanceAsInput();
    void addCentroidAsInput();
    void addWeightedCentroidAsInput();
    
    void addAllPadVelocitiesAsInput();
    void addSliderVelocitiesAsInput();
    void addPadSumVelocityAsInput();
    void addPadAverageVelocityAsInput();
    void addPerimeterVelocityAsInput();
    void addAverageInterFingerDistanceVelocityAsInput();
    void addCentroidVelocityAsInput();
    void addWeightedCentroidVelocityAsInput();
    
    void setMantaVisible(bool mantaVisible);
    void toggleMantaVisible() {setMantaVisible(!mantaVisible);}
    
protected:
    
    void setupInputs();
    void setupGuiInputs();
    void guiEvent(ofxUIEventArgs &e);
    void guiInputEvent(ofxUIEventArgs &e);

    bool allPads, allSliders, allButtons,
        numFingers, padSum, padAvg, perimeter,
        avgInterDist, centroid, wCentroid;
    bool vAllPads, vAllSliders,
        vPadSum, vPadAvg, vPerimeter,
        vAvgInterDist, vCentroid, vWCentroid;
    bool padVal[8][6], padVel[8][6];
    bool sliderVal[2], sliderVel[2];
    
    ofxUICanvas *gui, *guiInputs;
    ofxUIDropDownList *guiJointsFeatures, *guiGloalStatSelector, *guiJointSelector, *guiStatSelector;
    vector<string> rows, cols;
    vector<string> customPads;
    int guiRow, guiCol;
    
    MantaController manta;
    bool mantaVisible;
};

