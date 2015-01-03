#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    kinect.setup();
    kinect.setTrackingBlobs(true);
    
    for (int i=0; i<800; i++) {
        ofFbo newFbo;
        newFbo.allocate(640, 480);
        fbo.push_back(newFbo);
    }
    
    osc.setup(8000);
    
    window.setup("projection", ofGetScreenWidth(), 0, 1280, 800, true);
    mapping.addQuad(1280, 800);
    mapping.setMouseResolution(window.getWidth(), window.getHeight());

    control.addParameter("numFramesTotal", &numTotalFrames, 200, 800);
    control.addParameter("numFramesSkip", &numFramesSkip, 1, 30);
    control.addParameter("Margin", &margin, 5, 100);

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
            idx = (idx+1)%numTotalFrames;
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    
    kinect.beginMask(640, 480);
    kinect.drawRgb(0, 0, 640, 480);
    kinect.endMask();
    
    window.begin();
    mapping.begin(0);
    ofBackground(255);
    for (int i=0; i<numFramesSkip; i++) {
        int j = (i * margin + ofGetFrameNum()) % numTotalFrames;
        fbo[j].draw(0, 0, ofGetWidth(), ofGetHeight());
    }
    mapping.end(0);
    window.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key==' ')   recording = !recording;
    if (key=='c')   mapping.toggleDebug();

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
