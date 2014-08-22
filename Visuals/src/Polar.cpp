#include "Polar.h"

//----------
PolarEq::PolarEq(ofxParameter<bool> *is3d, ofxParameter<bool> *isRibbon, ofxParameter<ofVec3f> *speedRotation) {
    this->is3d = is3d;
    this->isRibbon = isRibbon;
    this->speedRotation = speedRotation;
    ribbon = new ofxTwistedRibbon();
    refresh();
}

//----------
void PolarEq::refresh() {
    numPoints = 16;
    age = (int) ofRandom(100, 800);
    ang = 0;
    noiseOffset = ofVec3f(ofRandom(100), ofRandom(100), ofRandom(100));
}

//----------
void PolarEq::update() {
    dAng = ofMap(ofNoise(noiseFactor.x * ofGetFrameNum() + noiseOffset.x, 5), 0, 1, 0, dAngMax);
    dRad = ofMap(ofNoise(noiseFactor.y * ofGetFrameNum() + noiseOffset.y, 10), 0, 1, -dRadMax, dRadMax);
    dRate = ofMap(ofNoise(noiseFactor.z * ofGetFrameNum() + noiseOffset.z, 20), 0, 1, -dRateMax, dRateMax);
    
    ang  += dAng;
    rad  += dRad;
    rate += dRate;
    
    float r, x, y, z;
    r = rad * cos(ang * rate);
    r += radNoiseMargin * ofMap(ofNoise(radNoiseFactor * ofGetFrameNum(), 5), 0, 1, -rad, rad);
    x = r * cos(ang);
    y = r * sin(ang);
    z = *is3d ? r * sin(ang+1.0) * cos(ang+2.0) : 0;

    if (*is3d) {
        rotAngle.x = ofLerp(rotAngle.x, speedRotation->get().x * ofGetFrameNum(), 0.05);
        rotAngle.y = ofLerp(rotAngle.y, speedRotation->get().y * ofGetFrameNum(), 0.05);
        rotAngle.z = ofLerp(rotAngle.z, speedRotation->get().z * ofGetFrameNum(), 0.05);
    }
    else {
        rotAngle.x = ofLerp(rotAngle.x, 0, 0.03);
        rotAngle.y = ofLerp(rotAngle.y, 0, 0.03);
        rotAngle.z = ofLerp(rotAngle.z, 0, 0.03);
    }
    

    pts.push_back(ofVec3f(x, y, z));
    while (pts.size() > numPoints) {
        pts.erase(pts.begin());
    }
    if (*isRibbon) {
        ribbon->length = numPoints;
        ribbon->color = color;
        ribbon->thickness = lineWidth;
        ribbon->update(ofVec3f(x, y, z));
    }
}

//----------
void PolarEq::draw() {
    
    ofSetColor(color);
    ofSetLineWidth(lineWidth);
    
    ofRotateX(rotAngle.x);
    ofRotateX(rotAngle.y);
    ofRotateZ(rotAngle.z);

    if (*isRibbon) {
        ribbon->draw();
    }
    else {
        ofNoFill();
        ofBeginShape();
        for (int i=0; i<pts.size(); i++) {
            ofCurveVertex(pts[i].x, pts[i].y, pts[i].z);
        }
        ofEndShape();
    }
}

//----------
void Polar::setup() {
    setName("Polar");
    
    control.registerParameter("color", &color, ofColor(0, 0), ofColor(255, 255));
    control.registerParameter("lineWidth", &lineWidth, 0.0f, 5.0f);
    control.registerParameter("nx", &nx, 1, 12);
    control.registerParameter("ny", &ny, 1, 12);
    control.registerParameter("numPoints", &numPoints, 3, 100);
    control.registerParameter("rad", &rad, 0.0f, 500.0f);
    control.registerParameter("dRadMax", &dRadMax, 0.0f, 0.5f);
    control.registerParameter("dRateMax", &dRateMax, 0.0f, 0.05f);
    control.registerParameter("dAngMax", &dAngMax, 0.0f, 0.5f);
    control.registerParameter("noiseFactor", &noiseFactor, ofVec3f(0,0,0), ofVec3f(0.015,0.015,0.015));
    control.registerParameter("radNoiseFactor", &radNoiseFactor, 0.0f, 1.0f);
    control.registerParameter("radNoiseMargin", &radNoiseMargin, 0.0f, 1.0f);
    control.registerParameter("3d", &is3d);
    control.registerParameter("ribbons", &isRibbon);
    control.registerParameter("speedRotation", &speedRotation, ofVec3f(0, 0, 0), ofVec3f(1, 1, 1));
    control.registerEvent("refresh", this, &Polar::refresh);
    
    nx = 3;
    ny = 3;
    numPoints = 12;
    color = ofColor(255, 255, 255, 150);
    lineWidth = 2;
    rad = 120;
    radNoiseFactor = 0.01;
    radNoiseMargin = 0.1;
    is3d = false;
    isRibbon = false;
}

//----------
void Polar::refresh() {
    for (int i=0; i<polars.size(); i++) {
        polars[i]->refresh();
    }
}

//----------
void Polar::update() {
    
    while (polars.size() > nx * ny) {
        polars.erase(polars.begin());
    }
    while (polars.size() < nx * ny) {
        polars.push_back(new PolarEq(&is3d, &isRibbon, &speedRotation));
    }
    
    for (int i=0; i<nx*ny; i++) {
        polars[i]->setColor(color);
        polars[i]->setLineWidth(lineWidth);
        polars[i]->setNumPoints(numPoints);
        polars[i]->setRad(rad);
        polars[i]->setDRadMax(dRadMax);
        polars[i]->setDRateMax(dRateMax);
        polars[i]->setDAngMax(dAngMax);
        polars[i]->setNoiseFactor(noiseFactor);
        polars[i]->setRadNoiseFactor(radNoiseFactor);
        polars[i]->setRadNoiseMargin(radNoiseMargin);
        polars[i]->update();
    }
}

//----------
void Polar::draw() {
    for (int i=0; i<nx; i++) {
        for (int j=0; j<ny; j++) {
            float x = ofMap(i+0.5, 0, nx, 0, width);
            float y = ofMap(j+0.5, 0, ny, 0, height);
            ofPushMatrix();
            ofTranslate(x, y);
            polars[i*ny + j]->draw();
            ofPopMatrix();
        }
    }
}

