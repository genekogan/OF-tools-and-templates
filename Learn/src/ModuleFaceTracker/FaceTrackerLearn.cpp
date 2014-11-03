#include "FaceTrackerLearn.h"


//-----------
FaceTrackerLearn::FaceTrackerLearn() : Learn() {
    setGuiInputsVisible(false);
    
    tracker.setup();
    cam.initGrabber(640, 480);
    
    faceMeshVertices = new vector<ofVec3f>();
    faceMeshVertices->resize(66);
    rotation.resize(9);
}

//-----------
void FaceTrackerLearn::update() {
    cam.update();
	if(cam.isFrameNew()) {
		tracker.update(toCv(cam));
        
		position = tracker.getPosition();
		scale = tracker.getScale();
		orientation = tracker.getOrientation();
		rotationMatrix = tracker.getRotationMatrix();
        faceMesh = tracker.getObjectMesh();
        *faceMeshVertices = faceMesh.getVertices();
        for (int r=0; r<3; r++) {
            rotation[r*3  ] = rotationMatrix.getRowAsVec3f(r).x;
            rotation[r*3+1] = rotationMatrix.getRowAsVec3f(r).y;
            rotation[r*3+2] = rotationMatrix.getRowAsVec3f(r).z;
        }
        
        if (visible) {
            leftEye = tracker.getImageFeature(ofxFaceTracker::LEFT_EYE);
            rightEye = tracker.getImageFeature(ofxFaceTracker::RIGHT_EYE);
            leftEyebrow = tracker.getImageFeature(ofxFaceTracker::LEFT_EYEBROW);
            rightEyebrow = tracker.getImageFeature(ofxFaceTracker::RIGHT_EYEBROW);
            noseBridge = tracker.getImageFeature(ofxFaceTracker::NOSE_BRIDGE);
            noseBase = tracker.getImageFeature(ofxFaceTracker::NOSE_BASE);
            innerMouth = tracker.getImageFeature(ofxFaceTracker::INNER_MOUTH);
            outerMouth = tracker.getImageFeature(ofxFaceTracker::OUTER_MOUTH);
            jaw = tracker.getImageFeature(ofxFaceTracker::JAW);
        }
    }
    
    Learn::update();
}

//-----------
void FaceTrackerLearn::draw() {
    if (visible) {
        Learn::draw();
        
        ofPushMatrix();
        ofTranslate(5, 80);
        ofScale(0.6, 0.6);
        cam.draw(0, 0);
        if(tracker.getFound()) {
            tracker.draw();
        }
        ofPopMatrix();
    }
}

//-----------
void FaceTrackerLearn::addPositionAsInput() {
    vector<LearnInputParameter*> newInputs;
    newInputs.push_back(addInput("position-x", &position.x, 0.0, 640.0));
    newInputs.push_back(addInput("position-y", &position.y, 0.0, 480.0));
    addParametersAsInput("position", newInputs);
}

//-----------
void FaceTrackerLearn::addScaleAsInput() {
    addInput("scale", &scale, 0.0, 10.0);
}

//-----------
void FaceTrackerLearn::addOrientationAsInput() {
    vector<LearnInputParameter*> newInputs;
    newInputs.push_back(addInput("orientation-x", &orientation.x, -1.0, 1.0));
    newInputs.push_back(addInput("orientation-y", &orientation.y, -1.0, 1.0));
    newInputs.push_back(addInput("orientation-z", &orientation.z, -1.0, 1.0));
    addParametersAsInput("orientation", newInputs);
}

//-----------
void FaceTrackerLearn::addRotationAsInput() {
    vector<LearnInputParameter*> newInputs;
    for (int i=0; i<9; i++) {
        newInputs.push_back(addInput("rotation-"+ofToString(i), &rotation[i], -1.0, 1.0));
    }
    addParametersAsInput("rotation", newInputs);
}

//-----------
void FaceTrackerLearn::addFaceMeshAsInput() {
    vector<LearnInputParameter*> newInputs;
    for (int i=0; i<faceMeshVertices->size(); i++) {
        newInputs.push_back(addInput("faceMesh-"+ofToString(i)+"-x", &faceMeshVertices->at(i).x, -20.0, 20.0));
        newInputs.push_back(addInput("faceMesh-"+ofToString(i)+"-y", &faceMeshVertices->at(i).y, -20.0, 20.0));
    }
    addParametersAsInput("face mesh", newInputs);
}

//-----------
FaceTrackerLearn::~FaceTrackerLearn() {
    cam.close();
}