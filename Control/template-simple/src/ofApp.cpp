#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    control.setName("hello world");
    control.addParameter("position", &position, ofVec2f(0, 0), ofVec2f(1280, 720));
    control.addParameter("radius", &radius, 20.0f, 200.0f);
    control.addParameter("filled", &filled);
    control.addParameter("point", &point, ofPoint(0,0,0), ofPoint(1,1,1));
    control.addParameter("vec3", &vec3, ofVec3f(0,0,0), ofVec3f(1,1,1));
    control.addColor("color", &color);
    control.addParameter("resolution", &resolution, 10, 30);
    control.addParameter("string", &myString);
    
    control.addEvent("event1", this, &ofApp::event1);
    control.addEvent("event2", this, &ofApp::event2);
    
    vector<string> items1;
    items1.push_back("menu1 item 1");
    items1.push_back("menu1 item 2");
    items1.push_back("menu1 item 3");
    items1.push_back("menu1 item 4");

    vector<string> items2;
    items2.push_back("menu2 item 1");
    items2.push_back("menu2 item 2");
    items2.push_back("menu2 item 3");
    items2.push_back("menu2 item 4");

    control.addMenu("dropdown1", items1, this, &ofApp::menuEvent1);
    control.addMenu("dropdown2", items2, this, &ofApp::menuEvent2);
    
    radius = 100;
    color = ofColor(0, 255, 0);
    position = ofVec2f(345, 298);
    resolution = 16;
    filled = true;
}

//---------
void ofApp::update(){
    
}

//---------
void ofApp::event1(string & s) {
    cout<<"event: ("+s+") happened"<<endl;
}

//---------
void ofApp::event2(string & s) {
    cout<<"event: ("+s+") happened"<<endl;
}

//---------
void ofApp::menuEvent1(string & s) {
    cout<<"menu1 selection : "<<s<<endl;
}

//---------
void ofApp::menuEvent2(string & s) {
    cout<<"menu2 selection : "<<s<<endl;
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
