#pragma once

#include "ofMain.h"
#include "Control.h"
#include "ofxKinect.h"
#include "ofxKinectProjectorToolkit.h"
#include "ofxCv.h"
#include "ofxOpenCv.h"



using namespace ofxCv;
using namespace cv;

enum ContourStrategy { BLOBS, SEGMENTATION };


class Kinect
{
public:
    void                    setup();
    void                    close();
    void                    setGuiPosition(int x, int y);
    void                    toggleGuiView();
    void                    setCalibration(string path);
    void                    setContourStrategy(ContourStrategy contourStrategy);
    //    ContourFinder*          getContourFinder();
    
    void                    update();
    void                    draw();
    void                    drawDebug();
    void                    drawMask();
    
private:
    void                    updateSegmentation();
    void                    updateBlobs();
    void                    drawDebugSegmentation();
    void                    drawDebugBlobs();
    
    ofxKinect               kinect;
    ofxKinectProjectorToolkit kpt;

    
//    ofxUICanvas             *gui;
    
	ofxCvColorImage         colorImage;
	ofxCvGrayscaleImage     grayImage;
	ofxCvGrayscaleImage     grayThreshNear;
	ofxCvGrayscaleImage     grayThreshFar;
    
    ContourStrategy         contourStrategy;
    ContourFinder           contourFinder;
    ofColor                 blobColors[11];

    ofShader                edgeShader;
    ofFbo                   fboKinect, fboEdges;
    ofPixels                pixels;

    Control                 control;

    ofxParameter<float>     fade;
    ofxParameter<float>     minArea;
    ofxParameter<float>     maxArea;
    ofxParameter<float>     threshold;
    ofxParameter<float>     persistence;
    ofxParameter<float>     maxDistance;
    ofxParameter<int>       numDilate;
    ofxParameter<int>       numErode;
    ofxParameter<float>     nearThreshold;
    ofxParameter<float>     farThreshold;
    ofxParameter<int>       smoothness;
    ofxParameter<bool>      curved;
};
