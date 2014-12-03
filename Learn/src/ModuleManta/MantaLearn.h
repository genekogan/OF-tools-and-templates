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
    void addPerimterAsInput();
    void addAverageInterFingerDistanceAsInput();
    void addCentroidAsInput();
    void addWeightedCentroidAsInput();
    
    void addAllPadVelocitiesAsInput();
    void addSliderVelocitiesAsInput();
    void addPadSumVelocityAsInput();
    void addPadAverageVelocityAsInput();
    void addPerimterVelocityAsInput();
    void addAverageInterFingerDistanceVelocityAsInput();
    void addCentroidVelocityAsInput();
    void addWeightedCentroidVelocityAsInput();
    
    void setMantaVisible(bool mantaVisible);
    void toggleMantaVisible() {setMantaVisible(!mantaVisible);}
    
protected:

    void setupGuiInputs();
    void guiEvent(ofxUIEventArgs &e);
    void guiInputEvent(ofxUIEventArgs &e);
    void padClickedEvent(int &e);
    void sliderClickedEvent(int &e);
    void buttonClickedEvent(int &e);

    ofxUICanvas *gui, *guiInputs;
    ofxUIDropDownList *guiJointsFeatures, *guiGloalStatSelector, *guiJointSelector, *guiStatSelector;
    vector<string> rows, cols;
    vector<string> customPads;
    
    MantaController manta;
    bool mantaVisible;
};

