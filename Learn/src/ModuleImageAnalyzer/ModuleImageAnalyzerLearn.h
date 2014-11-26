#pragma once

#include "ofMain.h"
#include "Learn.h"
#include "ofxFaceTracker.h"


using namespace ofxCv;


class FaceTrackerLearn : public Learn
{
public:
    ~FaceTrackerLearn();
    FaceTrackerLearn();
    
    void update();
    void draw();
    
    void addPositionAsInput();
    void addVelocityAsInput();
    void addScaleAsInput();
    void addScaleVelocityAsInput();
    void addOrientationAsInput();
    void addOrientationVelocityAsInput();
    void addRotationAsInput();
    void addRotationVelocityAsInput();
    void addFaceMeshAsInput();
    
    void setGuiPosition(int x, int y);
    void setVisible(bool visible);
    void toggleVisible() {setVisible(!visible);}
    void toggleGuiSummaryView();
    
protected:
    
    Control control;
    
    ofxFaceTracker tracker;
    ofVideoGrabber cam;
    
    // stats
    float vLerpRate;
    ofVec2f position, velocity;
	float scale, vScale;
	ofVec3f orientation, vOrientation;
	ofMatrix4x4 rotationMatrix;
    vector<float> rotation, vRotation;
    ofMesh faceMesh;
    vector<ofVec3f> *faceMeshVertices;
    
    ofPolyline leftEye, rightEye,
        leftEyebrow, rightEyebrow,
        noseBridge, noseBase,
        innerMouth, outerMouth, jaw;
    
    int x, y;
};
