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
    
    void setMantaVisible(bool mantaVisible);
    void toggleMantaVisible() {setMantaVisible(!mantaVisible);}
    void setOutputOptionVisible(bool v) {setupGuiOutputs(v);}
    
    MantaController * getMantaControllerRef() {return &manta;}
    
protected:
    
    void loadPreset(string path);
    
    void setupGuiInputs();
    void setupGuiOutputs(bool showAddOutputOption=true);
    void guiEvent(ofxUIEventArgs &e);
    void guiInputEvent(ofxUIEventArgs &e);
    
    void resetManta();
    void resetMantaInputSelector();

    // all feature sets
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

    // input selector
    bool allPads, allSliders, allButtons,
        numPads, padSum, padAvg, perimeter,
        width, height, whRatio,
        avgInterDist, centroid, wCentroid;
    bool vAllPads, vAllSliders, vAllButtons,
        vNumPads, vPadSum, vPadAvg, vPerimeter,
        vWidth, vHeight, vWhRatio,
        vAvgInterDist, vCentroid, vWCentroid;
    bool padVal[6][8], padVel[6][8];
    bool sliderVal[2], sliderVel[2];
    bool buttonVal[4], buttonVel[4];
    
    // gui
    ofxUICanvas *gui, *guiInputs;
    ofxUIDropDownList *guiJointsFeatures, *guiGloalStatSelector, *guiJointSelector, *guiStatSelector;
    vector<string> rows, cols;
    vector<string> customPads;
    int guiRow, guiCol;
    
    // manta
    MantaController manta;
    bool mantaVisible, inputsVisible;
};

