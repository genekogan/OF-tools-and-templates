#pragma once

#include "ofMain.h"
#include "Learn.h"
#include "ofxCv.h"


/*
 stub for image analysis module
 metrics
  - color histogram => GMM
  - color voxel set
  - color motion, movement
  - high-level info from contours, edges, keypoints
  - frame differencing, motion
  - flux
 inputs
  - image, movie
  - camera feed
  - webcam
 */


using namespace ofxCv;


class ModuleImageAnalyzer : public Learn
{
public:
    ~ModuleImageAnalyzer();
    ModuleImageAnalyzer();
    
    void update();
    void draw();
    
    //void addPositionAsInput();
    
    void setGuiPosition(int x, int y);
    void setVisible(bool visible);
    void toggleVisible() {setVisible(!visible);}
    void toggleGuiSummaryView();
    
protected:
    
    Control control;
    
    //ofxCv cv;
    //ofVideoGrabber cam;
    
    int x, y;
};
