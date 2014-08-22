#include "Amoeba.h"

//--------
void Amoeba::setup() {
    setName("Amoeba");
    
    control.registerParameter("center", &center, ofVec2f(0, 0), ofVec2f(width, height));
    control.registerParameter("numVertices", &numVertices, 3, 1000);
    control.registerParameter("radRange", &radRange, ofVec2f(-width, -width), ofVec2f(width, width));
    control.registerParameter("speed", &speed, 0.0f, 0.1f);
    control.registerParameter("noiseFactor", &noiseFactor, 0.0f, 0.1f);
    control.registerParameter("noiseRegion", &noiseRegion, -5.0f, 5.0f);
    control.registerParameter("offset", &offset, -4.0f, 4.0f);
    control.registerParameter("colorFill", &colorFill, ofColor(0, 0), ofColor(255, 255));
    control.registerParameter("colorStroke", &colorStroke, ofColor(0, 0), ofColor(255, 255));
    control.registerParameter("lineWidth", &lineWidth, 0.0f, 16.0f);
    control.registerParameter("filled", &filled);
    control.registerParameter("filledCycle", &cycleFill, 4, 200 );
    control.registerParameter("filledDensity", &densityFill, 0.0f, 1.0f);
    control.registerParameter("stroked", &stroked);
    control.registerParameter("strokedCycle", &cycleStroke, 4, 200);
    control.registerParameter("strokedDensity", &densityStroke, 0.0f, 1.0f);
    control.registerParameter("curvedVertices", &curvedVertices);
    
    numVertices = 200;
    center = ofVec2f(width/2, height/2);
    radRange = ofVec2f(0, width/3);
    speed = 0.01;
    noiseFactor = 0.07;
    offset = 0.0;
    noiseRegion = 0.0;
    time = 0.0;
    colorFill = ofColor(255, 100);
    colorStroke = ofColor(255, 100);
    filled = false;
    stroked = true;
    curvedVertices = false;
    lineWidth = 1;
}

//--------
void Amoeba::update() {
    time += speed;
}

//--------
void Amoeba::draw() {
    if (filled) {
        float t = (float) (ofGetFrameNum() % cycleFill) / cycleFill;
        if (t < densityFill) {
            ofFill();
            ofSetColor(colorFill);
            ofSetLineWidth(0);
            drawAmoeba();
        }
    }
    if (stroked) {
        float t = (float) (ofGetFrameNum() % cycleStroke) / cycleStroke;
        if (t < densityStroke) {
            ofNoFill();
            ofSetColor(colorStroke);
            ofSetLineWidth(lineWidth);
            drawAmoeba();
        }
    }
}

//--------
void Amoeba::drawAmoeba() {
    ofBeginShape();
    float ang, rad0, rad, x, y;
    for (int i=0; i<numVertices; i++) {
        ang = ofMap(i, 0, numVertices, 0, TWO_PI);
        rad0 = ofNoise(offset + noiseFactor * i, noiseRegion, time);
        rad = ofMap(rad0, 0, 1, radRange->x, radRange->y);
        x = center->x + rad * cos(ang);
        y = center->y + rad * sin(ang);
        if (curvedVertices) ofCurveVertex(x, y);
        else                ofVertex(x, y);
    }
    ofEndShape(true);
}