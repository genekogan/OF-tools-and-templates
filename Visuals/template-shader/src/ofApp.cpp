#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    shader = new Shader();
    ((Scene *) shader)->setup(1024, 768);
    
    shader->setShader("shaders_color/standard.vert",
                      "shaders_color/blobby.frag");
    shader->addParameter("depth", 0, 2);
    shader->addParameter("rate", 0, 2);
    shader->addParameter("speed", 0, 2);
    shader->addParameter("density", 0, 40);
    shader->addParameter("zoomout", 0, 200);

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ((Scene *) shader)->draw(0, 0);

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
