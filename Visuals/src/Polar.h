#pragma once

#include "ofMain.h"
#include "Scene.h"
#include "ofxTwistedRibbon.h"


class PolarEq
{
public:
    PolarEq(ofxParameter<bool> *is3d, ofxParameter<bool> *isRibbon, ofxParameter<ofVec3f> *speedRotation);
    
    void refresh();
    void update();
    void draw();
    
    void setNumPoints(int n) { this->numPoints = n; }
    
    void setColor(ofColor a) { this->color = a; }
    void setLineWidth(float a) { this->lineWidth = a; }
    
    void setRad(float rad) { this->rad = rad; }
    void setDRadMax(float dRadMax) { this->dRadMax = dRadMax; }
    void setDRateMax(float dRateMax) { this->dRateMax = dRateMax; }
    void setDAngMax(float dAngMax) { this->dAngMax = dAngMax; }
    void setRadNoiseFactor(float radNoiseFactor) { this->radNoiseFactor = radNoiseFactor; }
    void setRadNoiseMargin(float radNoiseMargin) { this->radNoiseMargin = radNoiseMargin; }
    void setNoiseFactor(ofVec3f noiseFactor) { this->noiseFactor = noiseFactor; }
    
private:
    
    ofColor color;
    float lineWidth;
    
    int numPoints;
    float ang, rad, rate;
    float dAng, dRad, dRate;
    float dAngMax, dRadMax, dRateMax;
    float radNoiseFactor, radNoiseMargin;
    ofVec3f noiseFactor;
    ofVec3f noiseOffset;
    ofVec3f rotAngle;
    
    vector<ofVec3f> pts;
    int age;
    
    ofxParameter<bool> *is3d, *isRibbon;
    ofxParameter<ofVec3f> *speedRotation;
    
    ofxTwistedRibbon *ribbon;
    ofEasyCam cam;
};



class Polar : public Scene
{
public:
    void setup();
    void update();
    void draw();
    
private:
    void refresh();
    
    vector<PolarEq *> polars;
    
    ofxParameter<int> nx, ny;
    ofxParameter<ofColor> color;
    ofxParameter<float> lineWidth;
    ofxParameter<int> numPoints;
    ofxParameter<float> rad;
    ofxParameter<float> dRadMax, dRateMax, dAngMax;
    ofxParameter<float> radNoiseFactor, radNoiseMargin;
    ofxParameter<ofVec3f> noiseFactor;
    ofxParameter<bool> is3d;
    ofxParameter<bool> isRibbon;
    ofxParameter<ofVec3f> speedRotation;
};
