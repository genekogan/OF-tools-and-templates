#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    aalto.setup();
    
}

//--------------------------------------------------------------
void ofApp::update(){
    aalto.update();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    aalto.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    /*
    if      (key=='q')   aalto.toggleNote(60, 90);
    else if (key=='w')   aalto.toggleNote(62, 90);
    else if (key=='e')   aalto.toggleNote(64, 90);
    else if (key=='r')   aalto.toggleNote(65, 90);
    else if (key=='t')   aalto.toggleNote(67, 90);
    else if (key=='y')   aalto.toggleNote(69, 90);
    else if (key=='u')   aalto.toggleNote(71, 90);
    else if (key=='i')   aalto.toggleNote(72, 90);
    else if (key=='o')   aalto.toggleNote(74, 90);
    else if (key=='p')   aalto.toggleNote(76, 90);
    else if (key=='[')   aalto.toggleNote(77, 90);
    else if (key==']')   aalto.toggleNote(79, 90);
    */
    
    /*
    if      (key=='q')   aalto.noteOn(60, 90);
    else if (key=='w')   aalto.noteOn(62, 90);
    else if (key=='e')   aalto.noteOn(64, 90);
    else if (key=='r')   aalto.noteOn(65, 90);
    else if (key=='t')   aalto.noteOn(67, 90);
    else if (key=='y')   aalto.noteOn(69, 90);
    else if (key=='u')   aalto.noteOn(71, 90);
    else if (key=='i')   aalto.noteOn(72, 90);
    else if (key=='o')   aalto.noteOn(74, 90);
    else if (key=='p')   aalto.noteOn(76, 90);
    else if (key=='[')   aalto.noteOn(77, 90);
    else if (key==']')   aalto.noteOn(79, 90);

    else if (key=='a')   aalto.noteOff(60, 90);
    else if (key=='s')   aalto.noteOff(62, 90);
    else if (key=='d')   aalto.noteOff(64, 90);
    else if (key=='f')   aalto.noteOff(65, 90);
    else if (key=='g')   aalto.noteOff(67, 90);
    else if (key=='h')   aalto.noteOff(69, 90);
    else if (key=='j')   aalto.noteOff(71, 90);
    else if (key=='k')   aalto.noteOff(72, 90);
    else if (key=='l')   aalto.noteOff(74, 90);
    else if (key==';')   aalto.noteOff(76, 90);
    else if (key=='\'')  aalto.noteOff(77, 90);
    else if (key=='z')   aalto.noteOff(79, 90);
     */
    
    
    /*
    if      (key=='q')   aalto.noteEvent(60, 90);
    else if (key=='w')   aalto.noteEvent(62, 90);
    else if (key=='e')   aalto.noteEvent(64, 90);
    else if (key=='r')   aalto.noteEvent(65, 90);
    else if (key=='t')   aalto.noteEvent(67, 90);
    else if (key=='y')   aalto.noteEvent(69, 90);
    else if (key=='u')   aalto.noteEvent(71, 90);
    else if (key=='i')   aalto.noteEvent(72, 90);
    else if (key=='o')   aalto.noteEvent(74, 90);
    else if (key=='p')   aalto.noteEvent(76, 90);
    else if (key=='[')   aalto.noteEvent(77, 90);
    else if (key==']')   aalto.noteEvent(79, 90);
     */

    if (key==' ') {
        aalto.randomizeSequencer();
    }
    
    if      (key=='q')   aalto.noteEvent2(60, 90);
    else if (key=='w')   aalto.noteEvent2(62, 90);
    else if (key=='e')   aalto.noteEvent2(64, 90);
    else if (key=='r')   aalto.noteEvent2(65, 90);
    else if (key=='t')   aalto.noteEvent2(67, 90);
    else if (key=='y')   aalto.noteEvent2(69, 90);
    else if (key=='u')   aalto.noteEvent2(71, 90);
    else if (key=='i')   aalto.noteEvent2(72, 90);
    else if (key=='o')   aalto.noteEvent2(74, 90);
    else if (key=='p')   aalto.noteEvent2(76, 90);
    else if (key=='[')   aalto.noteEvent2(77, 90);
    else if (key==']')   aalto.noteEvent2(79, 90);

    
    
    if (key=='1') {
        aalto.savePreset();
    }
    /*
    if (key=='2') {
        aalto.loadPreset("thistestpr");
    }
     */
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
