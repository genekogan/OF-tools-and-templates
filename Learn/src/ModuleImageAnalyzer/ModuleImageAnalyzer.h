#pragma once

#include "ofMain.h"
#include "Learn.h"


#include "ofxCv.h"
#include "ofxOpenCv.h"
#include "ofxHistogram.h"


/*
 stub for image analysis module
 metrics
  - color histogram/distribution => GMM
  - color voxel set
  - color motion, movement
  - high-level info from contours, edges, keypoints
  - frame differencing (by rows/cols), motion quantity
  - flux
  - optical flow -> quantify motion
  - edges/saliency map -> downpixel
  - brightness/contrast/saturation/

 inputs
  - image, movie
  - camera feed
  - webcam
 */




using namespace ofxCv;
using namespace cv;

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
    
    void getHistograms();
    void drawHistograms();
    void drawHistogram(vector<float> & h);
    ofxHistogram histogram;
    ofxCvColorImage rgb, hsv;
    ofxCvGrayscaleImage r, g, b;
    ofxCvGrayscaleImage h, s, v;
    vector<float> histogramR, histogramG, histogramB;
    vector<float> histogramH, histogramS, histogramV;

    
    
    int width, height;
    
    Control control;
    
    ofVideoGrabber cam;
    ofVideoPlayer player;

    
    ofPixels previous;
	ofImage diff;
	cv::Scalar diffMean;
    cv::Mat columnMean;
    
    
    
    
    
    ofxCv::FlowFarneback flow;
	ofMesh mesh;
	int stepSize, xSteps, ySteps;

    
    
    float frameDiffTotal;
    float frameDiffTotalRed, frameDiffTotalGreen, frameDiffTotalBlue;
    vector<float> frameDiffCol, frameDiffRow;

    
    int x, y;
};
