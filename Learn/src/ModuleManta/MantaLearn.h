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
    void guiEntryHelper(string b1, string b2);
    
    // manta
    void createCustomFeatureSetFromMantaSelection();
    void resetManta();
    
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
    
    // gui
    ofxUICanvas *gui, *guiInputs;
    ofxUIDropDownList *guiJointsFeatures, *guiGloalStatSelector, *guiJointSelector, *guiStatSelector;
    vector<string> rows, cols;
    vector<string> customPads;
    int guiRow, guiCol;
    int numCustomFeatureSets;
    
    // manta
    MantaController manta;
    bool mantaVisible, inputsVisible;
};

