#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "Learn.h"
#include "OpenNi.h"


class ModuleOpenNi : public Learn
{
public:
    ~ModuleOpenNi() {openNi.close();}
    ModuleOpenNi();
    
    void setup(string oniFile="");
    void update();
    void draw();
    
    // gui
    void setGuiPosition(int x, int y);
    void setVisible(bool visible);
    void toggleVisible() {setVisible(!visible);}
    void setOpenNiVisible(bool v) {openNi.setVisible(v);}
    void toggleOpenNiVisible() {openNi.toggleVisible();}
    void toggleGuiSummaryView();
    
protected:
    
    void setupGuiInputs();
    void guiInputEvent(ofxUIEventArgs &e);
    
    // collective joints
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
    
    // individual joints
    void addJointAsInput(int joint);
    void addNormalizedJointAsInput(int joint);
    void addRelativeJointAsInput(int joint);
    void addVelocityJointAsInput(int joint);
    void addAccelerationJointAsInput(int joint);
    
    void addRelativeDistanceJointAsInput(int joint);
    void addVelocityMagnitudeJointAsInput(int joint);
    void addVelocityMeanJointAsInput(int joint);
    void addAccelerationMagnitudeJointAsInput(int joint);
    void addAccelerationMeanJointAsInput(int joint);
    void addAccelerationTrajectoryJointAsInput(int joint);
    
    // stats
    void addSymmetryAsInput();
    void addQomAsInput();
    void addCiAsInput();
    void addDepthAsInput();
    void addYMaxHandsAsInput();
    
    // helpers for input features
    void addVec3AsInput(string descriptor, ofVec3f *vec, float min, float max);
    void addVectorVec3AsInput(string descriptor, string prefix, vector<ofVec3f *> *vec, float min, float max);
    void addVectorFloatAsInput(string descriptor, string prefix, vector<float *> *vec, float min, float max);
    void addIndividualAsInput(string descriptor, float *value, float min, float max);
    
    OpenNi openNi;
    
    ofxUICanvas *guiInputs;
    ofxUIDropDownList *guiJointsFeatures, *guiGloalStatSelector, *guiJointSelector, *guiStatSelector;
    int x, y;
};
