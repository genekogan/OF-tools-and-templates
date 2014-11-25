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
    void addScaleAsInput();
    void addOrientationAsInput();
    void addRotationAsInput();
    void addFaceMeshAsInput();
    
    void setGuiPosition(int x, int y);
    void setVisible(bool visible);
    void toggleVisible() {setVisible(!visible);}
    void toggleGuiSummaryView();

    
protected:
    
    ofxFaceTracker tracker;
    ofVideoGrabber cam;
    
    // stats
    ofVec2f position;
	float scale;
	ofVec3f orientation;
	ofMatrix4x4 rotationMatrix;
    vector<float> rotation;
    ofMesh faceMesh;
    vector<ofVec3f> *faceMeshVertices;
    
    ofPolyline leftEye, rightEye,
        leftEyebrow, rightEyebrow,
        noseBridge, noseBase,
        innerMouth, outerMouth, jaw;
    
    int x, y;
};
