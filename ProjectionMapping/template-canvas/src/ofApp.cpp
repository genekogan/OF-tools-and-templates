#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    mapping.addQuad(800, 600);
    mapping.addQuad(800, 600);
    
    canvas1.setup(800, 600);
    canvas1.addLayer(CANVAS_CREATOR);
    canvas1.addLayer(CANVAS_POST_FX);
    
    canvas2.setup(800, 600);
    canvas2.addLayer(CANVAS_CREATOR);
    canvas2.addLayer(CANVAS_POST_GLITCH);
    
    canvas1.setVisible(true);
    canvas2.setVisible(false);
}

//--------------------------------------------------------------
void ofApp::update(){
    if (ofGetFrameNum() % 60 == 0)
        cout << "fps: "<< ofGetFrameRate() << endl;
    
    canvas1.update();
    canvas2.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    mapping.begin(0);
    canvas1.draw(0, 0);
    mapping.end(0);

    mapping.begin(1);
    canvas2.draw(0, 0);
    mapping.end(1);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key=='1') {
        canvas1.setVisible(true);
        canvas2.setVisible(false);
    }
    else if (key=='2') {
        canvas1.setVisible(false);
        canvas2.setVisible(true);
    }
    else if (key=='3') {
        canvas1.setVisible(false);
        canvas2.setVisible(false);
    }

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
