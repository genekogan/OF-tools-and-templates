#pragma once

#include "ofMain.h"
#include "Control.h"

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
    
    void event1(string & s);
    void event2(string & s);
    void menuEvent1(string & s);
    void menuEvent2(string & s);
    
    Control control;
    
    /* parameters */
    bool filled;
    int resolution;
    float radius;
    ofPoint point;
    ofVec2f position;
    ofVec3f vec3;
    string myString;
    ofColor color;
};
