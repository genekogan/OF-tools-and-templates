#pragma once

#include "OpenNi.h"


class ContourRenderer;
class SkeletonRenderer;


class OpenNi
{
public:
    OpenNi();
    void setup(string oni="");
    void close();
    
    void enableUserTracking(int maxUsers=2);
    void enableContourTracking();
    
    void disableUserTracking();
    void disableContourTracking();
    
    void setCalibration(string path);
    
    void setupContourVisuals(int width, int height);
    void setupSkeletonVisuals(int width, int height);
    
    void start() {kinect.start();}
    void stop() {kinect.stop();}
    
    bool update();
    
    void draw();
    void drawCalibratedContours(int width=640, int height=480);
    void drawCalibratedSkeleton(int idx, int width=640, int height=480);
    
    void drawVisuals();
    
    void setMaxUsers(int maxUsers);
    void setNormalizeJoints(bool normalizeJoints) {this->normalizeJoints = normalizeJoints;}
    
    void setGuiPosition(int guiX, int guiY);
    void setVisible(bool visible);
    void toggleVisible();

    ofPoint getCalibratedPoint(int x, int y);
    void getCalibratedContour(vector<cv::Point> &points, vector<ofVec2f> &calibratedPoints, int width, int height);

    ContourFinder & getContourFinder() {return contourFinder;}
    RectTracker & getContourTracker() {return contourFinder.getTracker();}
    
    ContourRenderer * getContourRenderer() {return contourRenderer;}
    SkeletonRenderer * getSkeletonRenderer() {return skeletonRenderer;}
    
    int getNumUsers() {return kinect.getNumTrackedUsers();}

    // individual getters
    ofVec3f * getJoint(int idxUser, int joint);
    ofVec3f * getNormalizedJoint(int idxUser, int joint);
    ofVec3f * getRelativeJoint(int idxUser, int joint);
    ofVec3f * getVelocityJoint(int idxUser, int joint);
    ofVec3f * getAccelerationJoint(int idxUser, int joint);
    float * getRelativeDistanceJoint(int idxUser, int joint);
    float * getVelocityMagnitudeJoint(int idxUser, int joint);
    float * getVelocityMeanJoint(int idxUser, int joint);
    float * getAccelerationMagnitudeJoint(int idxUser, int joint);
    float * getAccelerationMeanJoint(int idxUser, int joint);
    float * getAccelerationTrajectoryJoint(int idxUser, int joint);

    // collective getters
    vector<ofVec3f*> & getJoints(int idxUser);
    vector<ofVec3f*> & getNormalizedJoints(int idxUser);
    vector<ofVec3f*> & getRelativeJoints(int idxUser);
    vector<ofVec3f*> & getVelocityJoints(int idxUser);
    vector<ofVec3f*> & getAccelerationJoints(int idxUser);
    vector<float*> & getRelativeDistanceJoints(int idxUser);
    vector<float*> & getVelocityMagnitudeJoints(int idxUser);
    vector<float*> & getVelocityMeanJoints(int idxUser);
    vector<float*> & getAccelerationMagnitudeJoints(int idxUser);
    vector<float*> & getAccelerationMeanJoints(int idxUser);
    vector<float*> & getAccelerationTrajectoryJoints(int idxUser);
    
    // stat getters
    float * getSymmetry(int idxUser);
    float * getQom(int idxUser);
    float * getCi(int idxUser);
    float * getDepth(int idxUser);
    float * getYMaxHands(int idxUser);
    
    vector<ofVec2f> & getProjectedJoints(int idxUser);
    
    string getJointName(int idx) {return jointNames[idx];}
    vector<string> getJointNames() {return jointNames;}
    
    void toggleActive() {active = !active;}
    
private:
    
    void setupControl();
    
    void updateContours();
    void updateUsers();
    void updateOpticalFlow();
    
    void applyDepthMask();
    void mousePressed(ofMouseEventArgs &evt);
    void clearMask(string &s);
    
    void checkTrackingOptions();
    void userEvent(ofxOpenNIUserEvent & event);
    
    // kinect
    ofxOpenNI kinect;
    ofxKinectProjectorToolkit kpt;
    ContourFinder contourFinder;
    ofxKinectFeatures featExtractor;
    bool calibrationLoaded;
    int kinectWidth, kinectHeight;
    
    // kinect data
    vector<string> jointNames;
    vector<vector<ofVec3f*> > pJoints, rJoints, vJoints, aJoints, normalizedJoints;
    vector<vector<float*> > dJoints, vJointsMag, vJointsMean, aJointsMag, aJointsMean, aJointsTrajectory;
    vector<float*> symmetry, qom, ci, depth, ymaxHands;
    vector<ofVec2f> projectedJoints;
    vector<ofVec3f> userBoundingBoxMin, userBoundingBoxMax;
    ofVec3f jointTorso, jointNeck, jointHead,
        jointLeftShoulder, jointLeftElbow, jointLeftHand,
        jointRightShoulder, jointRightElbow, jointRightHand,
        jointLeftHip, jointLeftKnee, jointLeftFoot,
        jointRightHip, jointRightKnee, jointRightFoot;
    
    Control control;
    int guiX, guiY;
    float guiScale;
    
    bool trackingContours, trackingUsers, trackingKeypoints;
    bool pTrackingContours, pTrackingUsers, pTrackingKeypoints;
    
    float minArea;
    float maxArea;
    float threshold;
    float persistence;
    float maxDistance;
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
    ofShortPixels depthPixels;
    
    // depth mask
    ofxCvColorImage cvMask;
    ofxCvGrayscaleImage cvGrayMask;
    ofFbo maskFBO;
    ofImage maskImage;
    ofPixels maskPixels;
    bool depthMaskEnabled, editingDepthMask;
    vector<ofPoint> depthMaskPoints, depthMaskVertices;

    // drawing
    ContourRenderer *contourRenderer;
    SkeletonRenderer *skeletonRenderer;
    bool contourVisuals, skeletonVisuals;
    
    bool active;
    bool visible;
};


