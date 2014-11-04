#include "ofApp.h"

//--------
void ofApp::setup() {
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    //kinect.setup("/Users/Gene/Downloads/hometest_single.oni");
    kinect.setup();
    kinect.setCalibration("/Users/Gene/Desktop/calibration.xml");
    kinect.setMaxUsers(2);

    gfx.setup("main", ofGetScreenWidth(), 0, 1280, 800, false);
}

//--------
void ofApp::update(){
    kinect.update();
}

//--------
void ofApp::draw(){
    kinect.draw();
    
    gfx.begin();
    
    ofSetColor(0, 150);
    ofRect(0, 0, gfx.getWidth(), gfx.getHeight());
    
    //kinect.drawCalibratedSkeleton(0, gfx.getWidth(), gfx.getHeight());
    drawSkeletonLines(gfx.getWidth(), gfx.getHeight(), 3, 10);
    
    gfx.end();
}

//--------
void ofApp::drawSkeletonLines(int width, int height, int iterations, float randomOffset){
    int numUsers = kinect.getNumUsers();
    for (int user=0; user<numUsers; user++) {
        vector<ofVec2f> joints = kinect.getProjectedJoints(user);
        ofSetColor(255, 50);
        ofSetLineWidth(2);
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
void ofApp::drawSkeletonLines2(int width, int height, int iterations, float randomOffset){
    vector<ofVec2f> joints = kinect.getProjectedJoints(0);
    
    
    ofSetColor(255, 50);
    ofSetLineWidth(2);
    
    float radius = 800;
    int numLines = ofMap(ofGetMouseX(), 0, ofGetWidth(), 5, 1000);
    
    for (int j=0; j<joints.size(); j++) {
    
        for (int i=0; i<numLines; i++) {
            float ang = ofMap(i, 0, numLines, 0, TWO_PI);
            
            //float x = ofGetWidth()*0.5 + radius * cos(ang);
            //float y = ofGetHeight()*0.5 + radius * sin(ang);
            float x = width * joints[j].x + radius * cos(ang);
            float y = height* joints[j].y + radius * sin(ang);
            
            ofLine(width * joints[j].x, height * joints[j].y, x, y);
            
            
        }
        
    }
}


//--------
void ofApp::exit(){
    kinect.close();
}

//--------
void ofApp::keyPressed(int key){
    
}