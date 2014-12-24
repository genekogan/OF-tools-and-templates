#pragma once

#include "ofMain.h"
#include "Learn.h"


#include "ofxCv.h"
#include "ofxOpenCv.h"
#include "ofxHistogram.h"


/*

 inputs
  - image, movie
  - camera feed
  - webcam
 
 features
  - 1d histograms (r, g, b, h, s, v) (GMM?)
  - 2d/3d histograms (rgb, hsv)
  - framediff -> motion quantity
  - optical flow -> motion vectors/direction movement
  - color flux
  - contour/edge/keypoint info: avg blob size, circularity (https://github.com/ofZach/circularityMetric/)
  - edges/saliency map -> downpixel
  - contrast/saturation

 ideas
  - use MoviePlayer scene as input
  - use Canvas as input
  - MoviePlayer class: webcam option
 
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
    
    // input sources
    ofVideoGrabber cam;
    ofVideoPlayer player;
    ofxCvColorImage image;
    
    // analysis
    ofPixels previous;
	ofImage diff;
	cv::Scalar diffMean;
    cv::Mat columnMean;
    ofxCv::FlowFarneback flow;
	ofMesh mesh;
	int stepSize, xSteps, ySteps;

    // stats
    float frameDiffTotal;
    float frameDiffTotalRed, frameDiffTotalGreen, frameDiffTotalBlue;
    vector<float> frameDiffCol, frameDiffRow;

    // parameters
    int mode=0;
    int width, height;
    int numBins;
    float vLerpRate;

    // gui
    Control control;
    int x, y;
};
