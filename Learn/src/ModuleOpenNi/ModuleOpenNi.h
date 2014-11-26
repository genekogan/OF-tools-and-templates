#pragma once

#include "ofMain.h"
#include "Learn.h"
#include "OpenNi.h"


class ModuleOpenNi : public Learn
{
public:
    ~ModuleOpenNi() {openNi.close();}
    ModuleOpenNi();
    
    void setup(string oni="");
    void update();
    void draw();
    
    void addJointsAsInput();
    void addNormalizedJointsAsInput();
    void addRelativeJointsAsInput();
    void addVelocityJointsAsInput();
    void addAccelerationJointsAsInput();
    
    void addRelativeDistanceJointsAsInput();
    void addVelocityMagnitudeJointsAsInput();
    void addVelocityMeanJointsAsInput();
    void addAccelerationMagnitudeJointsAsInput();
    void addAccelerationMeanJointsAsInput();
    void addAccelerationTrajectoryJointsAsInput();

    void addSymmetryAsInput();
    void addQomAsInput();
    void addCiAsInput();
    void addDepthAsInput();
    void addYMaxHandsAsInput();

    void setGuiPosition(int x, int y);
    void setVisible(bool visible);
    void toggleVisible() {setVisible(!visible);}
    void setOpenNiVisible(bool v) {openNi.setVisible(v);}
    void toggleOpenNiVisible() {openNi.toggleVisible();}
    void toggleGuiSummaryView();
    
protected:
        
    void addVectorVec3AsInput(string descriptor, string prefix, vector<ofVec3f *> *vec, float min, float max);
    void addVectorFloatAsInput(string descriptor, string prefix, vector<float *> *vec, float min, float max);
    void addIndividualAsInput(string descriptor, float *value, float min, float max);

    OpenNi openNi;
    
    int x, y;
};
