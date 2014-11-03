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
    void setMaxUsers(int maxUsers);
    void setNormalizeJoints(bool normalizeJoints) {this->normalizeJoints = normalizeJoints;}
    void setTrackingBlobs(bool trackingBlobs) {this->trackingBlobs = trackingBlobs; }
    void setCalibration(string path);
    void close();
    
    void setGuiPosition(int x, int y);
    void toggleGuiVisible();

    bool update();
    
    void updateContours();
    void updateJoints();
    void recordContours();
    
    void draw(int x=0, int y=0, int w=1280, int h=960);
    void drawCalibratedContours(int width=640, int height=480);
    void drawCalibratedSkeleton(int idx, int width=640, int height=480);

    vector<ofVec3f*> & getJoints(int idxUser);
    vector<ofVec3f*> & getNormalizedJoints(int idxUser);
    vector<ofVec2f>  & getProjectedJoints(int idxUser);
    string getJointName(int idx) {return jointNames[idx];}
    
    int getNumUsers() {return kinect.getNumTrackedUsers();}
    
    ContourFinder & getContourFinder() {return contourFinder;}
    RectTracker & getContourTracker() {return contourFinder.getTracker();}
    
private:
    
    void userEvent(ofxOpenNIUserEvent & event);

    // kinect
    ofxOpenNI kinect;
    ofxKinectProjectorToolkit kpt;
    ContourFinder contourFinder;

    // kinect data
    vector<string> jointNames;
    vector<vector<ofVec3f*> > joints, normalizedJoints;
    vector<ofVec2f> projectedJoints;
    ofVec3f userBoundingBoxMin, userBoundingBoxMax;
    ofVec3f jointTorso, jointNeck, jointHead,
        jointLeftShoulder, jointLeftElbow, jointLeftHand,
        jointRightShoulder, jointRightElbow, jointRightHand,
        jointLeftHip, jointLeftKnee, jointLeftFoot,
        jointRightHip, jointRightKnee, jointRightFoot;

    vector<Contour *> contours;
    //vector<Ribbon *> ribbons;

    Control control;

    bool trackingBlobs;
    bool trackingKeypoints;
    bool trackingJoints;

    
    float fade;
    float minArea;
    float maxArea;
    float threshold;
    float persistence;
    float maxDistance;
    int numDilate;
    int numErode;
    float nearThreshold;
    float farThreshold;
    int smoothness;
    bool curved;
    float smoothingRate;
    int maxUsers;
    bool normalizeJoints;
    
    ofxCvGrayscaleImage grayImage;
	ofxCvGrayscaleImage grayThreshNear;
	ofxCvGrayscaleImage grayThreshFar;
    
};


