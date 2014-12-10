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
    
    
    void addInputFeatureSet(string name);
    void removeInputFeatureSet(string name);
    
    void addAllPadsAsInput();
    void addSlidersAsInput();
    void addButtonsAsInput();
    void addNumPadsAsInput();
    void addPadSumAsInput();
    void addPadAverageAsInput();
    void addPerimeterAsInput();
    void addPadWidthAsInput();
    void addPadHeightAsInput();
    void addPadWHRatioAsInput();
    void addAverageInterFingerDistanceAsInput();
    void addCentroidAsInput();
    void addWeightedCentroidAsInput();
    void addAllPadVelocitiesAsInput();
    void addSliderVelocitiesAsInput();
    void addButtonVelocitiesAsInput();
    void addNumPadsVelocityAsInput();
    void addPadSumVelocityAsInput();
    void addPadAverageVelocityAsInput();
    void addPerimeterVelocityAsInput();
    void addPadWidthVelocityAsInput();
    void addPadHeightVelocityAsInput();
    void addPadWHRatioVelocityAsInput();
    void addAverageInterFingerDistanceVelocityAsInput();
    void addCentroidVelocityAsInput();
    void addWeightedCentroidVelocityAsInput();
    
    
//    void addWeightedCentroidVelocityAsInput2();
    
    
    
    //LearnOutputParameter * addOutput(string name, float min, float max);
    LearnInputParameter * addInput(string name, float min, float max);
    
    void setMantaVisible(bool mantaVisible);
    void toggleMantaVisible() {setMantaVisible(!mantaVisible);}

protected:
    
    //void setupInputs();
    void setupGuiInputs();
    void guiEvent(ofxUIEventArgs &e);
    void guiInputEvent(ofxUIEventArgs &e);
    void resetManta();

    // input selector
    bool allPads, allSliders, allButtons,
        numPads, padSum, padAvg, perimeter,
        width, height, whRatio,
        avgInterDist, centroid, wCentroid;
    bool vAllPads, vAllSliders, vAllButtons,
        vNumPads, vPadSum, vPadAvg, vPerimeter,
        vWidth, vHeight, vWhRatio,
        vAvgInterDist, vCentroid, vWCentroid;
    bool padVal[8][6], padVel[8][6];
    bool sliderVal[2], sliderVel[2];
    bool buttonVal[4], buttonVel[4];
    
    ofxUICanvas *gui, *guiInputs;
    ofxUIDropDownList *guiJointsFeatures, *guiGloalStatSelector, *guiJointSelector, *guiStatSelector;
    vector<string> rows, cols;
    vector<string> customPads;
    int guiRow, guiCol;
    
    MantaController manta;
    bool mantaVisible, inputsVisible;
};

