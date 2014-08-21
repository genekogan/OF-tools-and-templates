#include "ShapeSpace.h"


//--------
void ShapeSpace::setup() {
    setName("ShapeSpace");
 
}

//--------
void ShapeSpace::update() {
    
}

//--------
void ShapeSpace::draw() {
    cam.begin();
    
    for (int i=0; i<100; i++) {
        ofSetColor(ofMap(ofNoise(0.001*ofGetFrameNum(), 5*i, 45), 0, 1, 0, 255),
                   ofMap(ofNoise(0.001*ofGetFrameNum(), 5*i, 55), 0, 1, 0, 255),
                   ofMap(ofNoise(0.001*ofGetFrameNum(), 5*i, 65), 0, 1, 0, 255), 200);
        
        float x = ofMap(ofNoise(0.001*ofGetFrameNum(), 5*i, 15), 0, 1, -300, 300);
        float y = ofMap(ofNoise(0.001*ofGetFrameNum(), 5*i, 25), 0, 1, -300, 300);
        float z = ofMap(ofNoise(0.001*ofGetFrameNum(), 5*i, 35), 0, 1, -300, 0);
        
//        ofDrawBox(x, y, z, 50, 50, 50);
        ofDrawCylinder(x, y, z, 50, 90);
    }
    
    cam.end();
}

