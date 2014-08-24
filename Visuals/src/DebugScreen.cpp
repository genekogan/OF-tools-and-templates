#include "DebugScreen.h"


//--------
void DebugScreen::setup() {
    setName("DebugScreen");

    vector<string> debugModes;
    debugModes.push_back("Full");
    debugModes.push_back("Gradient");
    debugModes.push_back("Checkerboard");
    debugModes.push_back("Frames");

    vector<string> gradientModes;
    gradientModes.push_back("Circular");
    gradientModes.push_back("Linear");
    gradientModes.push_back("Bar");

    color1.addListener(this, &DebugScreen::changedColor);
    color2.addListener(this, &DebugScreen::changedColor);

    control.registerMenu("mode", this, &DebugScreen::setType, debugModes);
    control.registerParameter("color1", &color1, ofColor(0, 0), ofColor(255, 255));
    control.registerParameter("color2", &color2, ofColor(0, 0), ofColor(255, 255));
    control.registerParameter("numRects", &numRects, 1, 100);
    control.registerParameter("speed", &speed, 0, 100);
    control.registerMenu("gradient type", this, &DebugScreen::setupGradient, gradientModes);
    
    color1 = ofColor(ofRandom(255), ofRandom(255), ofRandom(255), 255);
    color2 = ofColor(ofRandom(255), ofRandom(255), ofRandom(255), 255);
    numRects = 8;
    speed = 60;
    
    setupGradient((string &) gradientMode);
}

//--------
void DebugScreen::setType(string &s) {
    if      (s == "Full")           type = FULL;
    else if (s == "Gradient")       type = GRADIENT;
    else if (s == "Checkerboard")   type = CHECKERBOARD;
    else if (s == "Frames")         type = FRAMES;
}

//--------
void DebugScreen::update() {

}

//--------
void DebugScreen::draw() {
    if (type == FULL) {
        drawFull();
    } else if (type == CHECKERBOARD) {
        drawCheckerboard();
    } else if (type == FRAMES) {
        drawFrames();
    } else if (type == GRADIENT) {
        drawGradient();
    }
}

//--------
void DebugScreen::drawFull() {
    ofSetColor(color1);
    ofFill();
    ofRect(0, 0, width, height);
}

//--------
void DebugScreen::drawCheckerboard() {
    ofSetColor(color2);
    ofRect(0, 0, width, height);
    
    float x, y, w, h;
    w = width / numRects;
    h = height / numRects;
    
    ofSetColor(color1);
    for (int i=0; i<numRects; i++){
        for (int j=0; j<numRects; j++) {
            if ((i+j)%2==0) continue;
            x = ofMap(i, 0, numRects, 0, width);
            y = ofMap(j, 0, numRects, 0, height);
            ofRect(x, y, w, h);
        }
    }
}

//--------
void DebugScreen::drawFrames() {
    ofSetColor(color1);
    ofNoFill();
    ofSetLineWidth((float) width/ numRects);
    
    float x, y, w, h, di;
    if (speed == 0) di = 0;
    else    di = (ofGetFrameNum() % speed) / (float)speed;
    for (int i=0; i<numRects; i++) {
        x = ofMap(i + di, 0, numRects, 0, width/2.0);
        y = ofMap(i + di, 0, numRects, 0, height/2.0);
        w = ofMap(i + di, 0, numRects, width, 0);
        h = ofMap(i + di, 0, numRects, height, 0);
        ofRect(x, y, w, h);
    }

    ofSetLineWidth(12);
    ofSetColor(color2);
    ofRect(6, 6, width-12, height-12);
}

//--------
void DebugScreen::drawGradient() {
    GLboolean depthMaskEnabled;
    glGetBooleanv(GL_DEPTH_WRITEMASK, &depthMaskEnabled);
    glDepthMask(GL_FALSE);
    gradientMesh.draw();
    if (depthMaskEnabled){
        glDepthMask(GL_TRUE);
    }
}

//--------
void DebugScreen::changedColor(ofColor & c) {
    if (type == GRADIENT) {
        setupGradient((string &) gradientMode);
    }
}

//--------
void DebugScreen::setupGradient(string &gradientMode) {
    this->gradientMode = ofToString(gradientMode);

    gradientMesh.clear();
    gradientMesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
    gradientMesh.setUsage(GL_STREAM_DRAW);
    
    if (gradientMode == "Circular") {
        ofVec2f center(width / 2, height / 2);
        gradientMesh.addVertex(center);
        gradientMesh.addColor(ofColor(color1));
        int n = 64;
        float angleBisector = TWO_PI / (n * 2);
        float smallRadius = ofDist(0, 0, width / 2, height / 2);
        float bigRadius = smallRadius / cos(angleBisector);
        for(int i = 0; i <= n; i++) {
            float theta = i * TWO_PI / n;
            gradientMesh.addVertex(center + ofVec2f(sin(theta), cos(theta)) * bigRadius);
            gradientMesh.addColor(ofColor(color2));
        }
    }
    else if (gradientMode == "Linear") {
        gradientMesh.addVertex(ofVec2f(0, 0));
        gradientMesh.addVertex(ofVec2f(width, 0));
        gradientMesh.addVertex(ofVec2f(width, height));
        gradientMesh.addVertex(ofVec2f(0, height));
        gradientMesh.addColor(ofColor(color1));
        gradientMesh.addColor(ofColor(color1));
        gradientMesh.addColor(ofColor(color2));
        gradientMesh.addColor(ofColor(color2));
    }
    else if (gradientMode == "Bar") {
        gradientMesh.addVertex(ofVec2f(width / 2, height / 2));
        gradientMesh.addVertex(ofVec2f(0, height / 2));
        gradientMesh.addVertex(ofVec2f(0, 0));
        gradientMesh.addVertex(ofVec2f(width, 0));
        gradientMesh.addVertex(ofVec2f(width, height / 2));
        gradientMesh.addVertex(ofVec2f(width, height));
        gradientMesh.addVertex(ofVec2f(0, height));
        gradientMesh.addVertex(ofVec2f(0, height / 2));
        gradientMesh.addColor(ofColor(color1));
        gradientMesh.addColor(ofColor(color1));
        gradientMesh.addColor(ofColor(color2));
        gradientMesh.addColor(ofColor(color2));
        gradientMesh.addColor(ofColor(color1));
        gradientMesh.addColor(ofColor(color2));
        gradientMesh.addColor(ofColor(color2));
        gradientMesh.addColor(ofColor(color1));
    }
}