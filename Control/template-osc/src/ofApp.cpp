#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    /* setup ofxUI layout */
    control.setName("control");
    control.addParameter("position", &position, ofVec2f(0, 0), ofVec2f(ofGetWidth(), ofGetHeight()));
    control.addParameter("radius", &radius, 20.0f, 200.0f);
    control.addParameter("filled", &filled);
    control.addParameter("point", &point, ofPoint(0,0,0), ofPoint(1,1,1));
    control.addParameter("resolution", &resolution, 4, 24);
    
    /* receive parameters */
    osc.setupReceiver(8000);
    osc.registerToOscReceiver(control.getParameters());

    /* send parameters  */
    osc.setupSender("localhost", 12345);
    osc.registerToOscSender(control.getParameters());

    /* convert control parameters into touchOsc layout */
    osc.saveTouchOscLayout("myLayout", control.getParameters());
    
    radius = 100;
    color = ofColor(0, 255, 0);
    position = ofVec2f(345, 298);
    resolution = 12;
    filled = true;
}

//---------
void ofApp::update(){
    osc.update();
}

//---------
void ofApp::draw(){
    if (filled) ofFill();
    else        ofNoFill();
    ofSetCircleResolution(resolution);
    ofSetColor(color.r, color.g, color.b);
    ofCircle(position.x, position.y, radius);
}

//---------
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
