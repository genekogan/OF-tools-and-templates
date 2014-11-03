#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    kinect.setup();
    kinect.setTrackingBlobs(true);
    
    for (int i=0; i<n; i++) {
        ofFbo newFbo;
        newFbo.allocate(640, 480);
        fbo.push_back(newFbo);
    }
    
    osc.setup(8000);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    while(osc.hasWaitingMessages()){
        ofxOscMessage msg;
        osc.getNextMessage(&msg);
        string address = msg.getAddress();
        if (address == "/1/toggle1") {
            int val = msg.getArgAsInt32(0);
            recording = (val == 1) ? true : false;
        }
    }
    
    bool kinectNew = kinect.update();

    if (kinectNew) {
        if (recording) {
            fbo[idx].begin();
            ofClear(0, 0);
            kinect.beginMask(640, 480);
            kinect.drawRgb(0, 0, 640, 480);
            kinect.endMask();
            fbo[idx].end();
            idx = (idx+1)%n;
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    
    if (recording) {
        kinect.beginMask(640, 480);
        kinect.drawRgb(0, 0, 640, 480);
        kinect.endMask();
    }
    else {
        //ofEnableBlendMode(OF_BLENDMODE_ADD);
        for (int i=0; i<6; i++) {
            int j = (i * 40 + ofGetFrameNum()) % n;
            fbo[j].draw(0, 0, ofGetWidth(), ofGetHeight());
        }
        //ofDisableBlendMode();
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key==' ')   recording = !recording;
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
