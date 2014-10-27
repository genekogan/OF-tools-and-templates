#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    kinect.setup();
    kinect.setCalibration("/Users/Gene/Desktop/calibration.xml");
    kinect.setTrackingBlobs(true);
    
    gfx.setup("render", ofGetScreenWidth(), 0, 1280, 800, true);
}

//--------------------------------------------------------------
void ofApp::update(){
    kinect.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    kinect.drawDebug(0, 0);
    
    
    float angOffset = 0.01 * ofGetFrameNum();
    float alph = 50;
    float lineWidth = ofMap(sin(0.001*ofGetFrameNum()), -1, 1, 0.1, 2);
    
    gfx.begin();
    
    ofClear(0, 0);
    ofNoFill();
    ofSetColor(255, alph);
    ofSetLineWidth(lineWidth);
    
    ofxCv::ContourFinder contourFinder = kinect.getContourFinder();
    for(int i = 0; i < contourFinder.size(); i++) {
        vector<cv::Point> points = contourFinder.getContour(i);
        for (int j=0; j<points.size(); j++) {

            ofVec2f pp = kinect.getCalibratedPoint(ofVec2f(points[j].x, points[j].y));
            ofPoint mp(ofMap(pp.x, 0, 1, 0, gfx.getWidth()),
                       ofMap(pp.y, 0, 1, 0, gfx.getHeight()));

            float ang = ofMap(j, 0, points.size(), 0, TWO_PI) + angOffset;
            ofPoint e(ofGetWidth()*0.5 + 800*cos(ang), ofGetHeight()*0.5 + 800*sin(ang));
            ofLine(mp.x, mp.y, e.x, e.y);
        }
    }
    
    gfx.end();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
