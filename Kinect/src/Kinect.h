#pragma once

#include "ofMain.h"
#include "Control.h"
#include "Mask.h"
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
    ~Kinect();
    void                    setup();
    void                    close();
    
    void                    setTrackingBlobs(bool t) {trackingBlobs = t;}
    void                    setTrackingKeypoints(bool t) {trackingKeypoints = t;}
    
    void                    setGuiPosition(int x, int y);
    void                    toggleGuiVisible();

    void                    setCalibration(string path);
    
    void                    setContourStrategy(ContourStrategy contourStrategy);
    ContourFinder&          getContourFinder();
    RectTracker&            getContourTracker();
    
    bool                    update();
    void                    draw();
    
    ofVec3f                 getWorldCoordinateAt(int x, int y);
    
    void                    drawDebug(int x, int y, int w=1280, int h=960);
    void                    drawDepth(int x, int y, int w=640, int h=480);
    void                    drawRgb(int x, int y, int w=640, int h=480);
    
    void                    drawMask(ofBaseDraws &tex, bool useCalibration=false);
    void                    beginMask(int w, int h, bool useCalibration=false);
    void                    endMask();
    
    void                    drawCalibratedContours(int width, int height);
    ofVec2f                 getCalibratedPoint(ofVec2f depthPoint);
    
    void                    setKeypointROI(ofPoint topLeft, ofPoint bottomRight);
    ofxKinect *             getKinect() {return &kinect;}
    
private:
    
    void                    selectStrategy(string &s);

    void                    updateSegmentation();
    void                    updateBlobs();
    void                    updateContours();
    
    
    /* main */
    ofxKinect               kinect;
    ofxKinectProjectorToolkit kpt;
    
    /* blob tracking */
	ofxCvColorImage         colorImage;
	ofxCvGrayscaleImage     grayImage;
	ofxCvGrayscaleImage     grayThreshNear;
	ofxCvGrayscaleImage     grayThreshFar;
    ContourStrategy         contourStrategy;
    ContourFinder           contourFinder;
    ofColor                 blobColors[12];
    ofShader                edgeShader;
    ofFbo                   fboKinect, fboEdges;
    ofPixels                pixels;

    /* generating masks */
    KMask                   kmask;
    
    /* keypoint tracking */
    cv::Mat                 kinectGray;
    ofxCv::FlowPyrLK        flow;
    ofVec2f                 p1;
    ofRectangle             rect;
    
    /* parameters */
    Control                 control;
    bool                    trackingBlobs;
    bool                    trackingKeypoints;
    
    float                   fade;
    float                   minArea;
    float                   maxArea;
    float                   threshold;
    float                   persistence;
    float                   maxDistance;
    int                     numDilate;
    int                     numErode;
    float                   nearThreshold;
    float                   farThreshold;
    int                     smoothness;
    bool                    curved;
    float                   smoothingRate;
    
};
