#include "SkeletonRenderer.h"

//--------
void SkeletonRenderer::renderJointLines(){
    ofSetColor(color);
    ofSetLineWidth(lineWidth);
    int numUsers = openNi->getNumUsers();
    for (int user=0; user<numUsers; user++) {
        vector<ofVec2f> joints = openNi->getProjectedJoints(user);
        for (int i=0; i<joints.size(); i++) {
            for (int j=i+1; j<joints.size(); j++) {
                ofPoint p1(joints[i].x * width, joints[i].y * height);
                ofPoint p2(joints[j].x * width, joints[j].y * height);
                for (int k=0; k<iterations; k++) {
                    ofLine(p1.x + ofRandom(-randomOffset, randomOffset),
                           p1.y + ofRandom(-randomOffset, randomOffset),
                           p2.x + ofRandom(-randomOffset, randomOffset),
                           p2.y + ofRandom(-randomOffset, randomOffset));
                }
            }
        }
    }
}

//--------
void SkeletonRenderer::renderOutwardLines(){
    ofSetColor(color);
    ofSetLineWidth(lineWidth);
    vector<ofVec2f> joints = openNi->getProjectedJoints(0);
    for (int i=0; i<numLines; i++) {
        float ang = ofMap(i, 0, numLines, 0, TWO_PI);
        float x = width * 0.5 + radius * cos(ang);
        float y = height * 0.5 + radius * sin(ang);
        ofLine(width * joints[3].x, height * joints[3].y, x, y);
    }
}

//--------
void SkeletonRenderer::setupControl() {
    control.setName("skeleton render");
    
    control.addParameter("joint lines", &drawJointLines);
    control.addParameter("outward lines", &drawOutwardLines);
    
    control.addParameter("iterations", &iterations, 1, 10);
    control.addParameter("randomOffset", &randomOffset, 0.0f, 100.0f);
    control.addColor("color", &color);
    control.addParameter("lineWidth", &lineWidth, 0.5f, 4.0f);
    control.addParameter("numLines", &numLines, 5, 500);
    control.addParameter("radius", &radius, 0, 800);
}

