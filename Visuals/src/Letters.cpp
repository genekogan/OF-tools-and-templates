#include "Letters.h"

void Letters::setup(int width, int height) {
    Scene::setup(width, height);
    
    control.registerParameter("rows", &nrows, 2, 200);
    control.registerParameter("cols", &ncols, 2, 200);
    control.registerParameter("rate", &rate, 1, 10);
    control.registerParameter("color", &color, ofColor(0), ofColor(255));
    
    nrows = 30;
    ncols = 24;
    rate = 1;
    color = ofColor(0, 255, 20);
    
    font.loadFont("AndaleMono.ttf", 24);
    
    chars.resize(nrows * ncols);
}

void Letters::update() {
    if (chars.size() != nrows * ncols) {
        chars.resize(nrows * ncols);
    }
    
    if (ofGetFrameNum() % rate == 0) {
        
        static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
        
        int idx = 0;
        for (int i=0; i<nrows; i++) {
            for (int j=0; j<ncols; j++) {
                chars[idx++] = alphanum[ (int) ofRandom(62) ];
            }
        }
    }
    
}

void Letters::draw() {
    int idx = 0;
    float x, y;
    for (int i=0; i<nrows; i++) {
        for (int j=0; j<ncols; j++) {
            x = ofMap(i, 0, nrows, 0, width);
            y = ofMap(j, 0, ncols, 0, height);
            ofSetColor(color);
            font.drawString(ofToString(chars[idx++]), x, y);
        }
    }
}