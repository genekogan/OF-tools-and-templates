#pragma once

#include "ofMain.h"
#include "Scene.h"


class PolarEq
{
public:
    PolarEq() {
        
        nx = 1;
        ny = 1;
       
        ang = 0;
        dAng = 0;
        a = 0;
        b = 0;
        da = 0;
        db = 0;
        dangmax = 0;
        damax = 0;
        dbmax = 0;
        w1 = 0;
        w2 = 0;
        w3 = 0;
        o1 = 0;
        o2 = 0;
        o3 = 0;
        age = 0;
        
        refresh();

    }
    
    
    void setA(float a) { this->a = a; }
    void setB(float a) { this->b = a; }
    void setDangMax(float a) { this->dangmax = a; }
    void setW1(float a) { this->w1 = a; }
    void setW2(float a) { this->w2 = a; }
    void setW3(float a) { this->w3 = a; }
    
    
    void refresh() {
        age = (int) ofRandom(100, 800);
        ang = 0;
        a = (float) ofGetWidth()/(3.0 * nx);
        dangmax = 1;
        b = ofRandom(10);
        w1= ofRandom(0.0005, 0.0015);
        w2= ofRandom(0.0005, 0.0015);
        w3= ofRandom(0.0005, 0.0015);
        o1 = ofRandom(100);
        o2 = ofRandom(100);
        o3 = ofRandom(100);
    }
    
    void update() {
        ang += dAng;
        a += da;
        b += db;
        
        damax = ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 1);
        dbmax = ofMap(ofGetMouseY(), 0, ofGetHeight(), 0, 1);
        
        dAng = ofMap(ofNoise(w1*ofGetFrameNum()+o1, 5), 0, 1, 0, dangmax);
        da = ofMap(ofNoise(w2*ofGetFrameNum()+o2, 10), 0, 1, -damax, damax);
        db = ofMap(ofNoise(w3*ofGetFrameNum()+o3, 20), 0, 1, -dbmax, dbmax);
        float r = a * cos(b * ang);
        float x = r * cos(ang);
        float y = r * sin(ang);
        
        pts.push_back(ofVec2f(x, y));
        
        if (pts.size() > 16) {
            //pts.pop_back();
            pts.erase(pts.begin());
        }
    }
    
    void draw() {
        
        //if (ofGetFrameNum() % 200 == 0) {refresh();}
        ofSetColor(255, 90);
        
        ofSetLineWidth(2);
        
        ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
        ofNoFill();
        ofBeginShape();
        for (int i=0; i<pts.size(); i++) {
            ofCurveVertex(pts[i].x, pts[i].y);
        }
        ofEndShape();

    }

    
    float ang, dAng;
    float a, b, da, db;
    vector<ofVec3f> pts;
    float dangmax, damax, dbmax;
    float w1, w2, w3;
    float o1, o2, o3;
    int age;

    int nx, ny;
    
    
};



class Polar : public Scene
{
public:
    void setup();
    void update();
    void draw();
    
    PolarEq *p;
private:
    ofxParameter<float> a, b, dangmax;
    ofxParameter<float> w1, w2, w3;
    
};