#pragma once

#include "ofMain.h"
#include "Control.h"
#include "OscManager.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    Control control;
    OscManager osc;
    
    /* parameters */
    bool filled;
    int resolution;
    string myString;
    float radius;
    ofPoint point;
    ofVec2f position;
    ofColor color;

};
