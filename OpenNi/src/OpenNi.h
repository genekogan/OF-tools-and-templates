#pragma once

#include "ofMain.h"
#include "ofxOpenNi.h"
#include "ofxCv.h"
#include "ofxOpenCv.h"
#include "ofxKinectProjectorToolkit.h"
#include "Control.h"


using namespace ofxCv;
using namespace cv;

enum ContourStrategy { BLOBS, SEGMENTATION };




class Contour {
public:
    vector<cv::Point> points;
    int label;
    int age;
    bool active;
    
    ofColor color;
    
    Contour(vector<cv::Point> & points, int label) {
        this->points = points;
        this->label = label;
        age = 0;
        active = true;
        color = ofColor(ofRandom(60,255), ofRandom(60,255), ofRandom(60,255));
    }
    
    void setPoints(vector<cv::Point> & points) {
        this->points = points;
    }
    
    void draw() {
        ofPushStyle();
        ofNoFill();
        ofSetLineWidth(2);
        ofSetColor(color);
        ofBeginShape();
        for (int j=0; j<points.size(); j++) {
            ofVertex(points[j].x, points[j].y);
        }
        ofEndShape();
        ofPopStyle();
    }
    
};



class OpenNi
{
public:
    void setup(string oni="");
    
    
    
    void close();
    
    void setCalibration(string path);
    
    void setGuiPosition(int x, int y);
    void toggleGuiVisible();

    bool update();
    
    void updateContours();
    void updateJoints();
    
    void recordContours();
    
    void draw(int x=0, int y=0, int w=640, int h=480);
    void drawCalibrated(int x=0, int y=0, int width=640, int height=480);
    void drawCalibratedSkeleton(int width, int height);

    vector<ofVec3f*> & getJoints(bool normalized=false);
    vector<ofVec2f>  & getProjectedJoints();
    string getJointName(int idx) {return jointNames[idx];}
    
    
    ContourFinder&          getContourFinder();
    RectTracker&            getContourTracker();
    
    vector<Contour *> contours;
    //vector<Ribbon *> ribbons;

    
private:
    
    void userEvent(ofxOpenNIUserEvent & event);

    // kinect
    ofxOpenNI kinect;
    ofxKinectProjectorToolkit kpt;

    // kinect data
    vector<string> jointNames;
    vector<ofVec3f*> joints;
    vector<ofVec2f> projectedJoints;
    ofVec3f userBoundingBoxMin, userBoundingBoxMax;
    ofVec3f jointTorso, jointNeck, jointHead,
        jointLeftShoulder, jointLeftElbow, jointLeftHand,
        jointRightShoulder, jointRightElbow, jointRightHand,
        jointLeftHip, jointLeftKnee, jointLeftFoot,
        jointRightHip, jointRightKnee, jointRightFoot;

    // parameters
    Control control;
    
    
    
    
    /* blob tracking */
    ContourFinder           contourFinder;

    bool                    trackingBlobs;
    bool                    trackingKeypoints;
    bool                    trackingJoints;

    
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
    
    
    ofxCvGrayscaleImage grayImage;

};


