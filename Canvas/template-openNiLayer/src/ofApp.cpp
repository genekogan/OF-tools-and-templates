#include "ofApp.h"

//-----------
void ofApp::setup(){
    ofSetBackgroundAuto(false);
    
    //window.setup("projection", ofGetScreenWidth()-500, 0, 1280, 800, false);
    window.setup("projection", ofGetScreenWidth()-500, 0, 800, 600, false);
    
    canvas.setup(window.getWidth(), window.getHeight());
    canvas.addLayer(CANVAS_CREATOR);
    canvas.setVisible(false);
    
    kinect.setup("/Users/Gene/Code/openFrameworks/templates/Kinect/openni_oniRecording/bin/data/alecsroom.oni");
    kinect.enableContourTracking();
    kinect.setCalibration("/Users/Gene/Desktop/calibration.xml");
    kinect.setupContourVisuals(window.getWidth(), window.getHeight());
    kinect.start();
    
//    kinect.getContourRenderer()->setDrawFluids(true);

}

//-----------
void ofApp::update(){
    kinect.update();
    canvas.update();
}

//-----------
void ofApp::draw() {
    ofBackground(0);
    kinect.draw();
    
    window.begin();

    ofClear(0, 0);
    canvas.draw(0, 0);
    kinect.drawVisuals();

    window.end();
}

//-----------
void ofApp::exit(){
    kinect.stop();
    kinect.close();
}

//-----------
void ofApp::keyPressed(int key){
    if (key=='g') {
        canvas.toggleGuiVisible();
        kinect.toggleVisible();
    }
}