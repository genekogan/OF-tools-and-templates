#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // setup sc3
    string synthFile = "/Users/Gene/Code/openFrameworks/tools/SuperCollider/synths.scd";
    SuperColliderLayer *layer1 = sc3.addLayer("source", synthFile);
    layer1->setBusOutSelf();
    sc3.setGuiPosition(200, 200);

    // add inputs
    learn.addJointsAsInput();
    
    // add outputs
    /*
    learn.addOutput("h1x/", 1, 30);
    learn.addOutput("h1y/", 0.001, 0.99);
    learn.addOutput("h1z/", 1, 16);
    */
    
    //1
    learn.addOutput("h1x/", 0, 0.8);
    learn.addOutput("h1y/", 0, 0.35);
    learn.addOutput("h1z/", 0, 0.05);
    //learn.addOutput("h1w/", 0, 0.3);
    learn.addOutput("h1a/", 0, 1);
    
    /*
    //2
    learn.addOutput("h1x/", 0, 1);
    learn.addOutput("h1y/", 0, 1);
    learn.addOutput("h1z/", 0, 1);
    learn.addOutput("h1w/", 0, 1);
    learn.addOutput("h1a/", 0, 1);
*/
    
    // osc
    learn.setupOscSender("localhost", 57120);
    learn.setupOscReceiver(8000);
}

//--------------------------------------------------------------
void ofApp::update(){
    sc3.update();
    learn.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    learn.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key=='g')
        learn.toggleVisible();
    
    if (key=='s')
        learn.savePreset();
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
