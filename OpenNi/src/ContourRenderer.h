#pragma once

#include "OpenNi.h"

class OpenNi;


class Contour {
public:
    Contour(vector<ofVec2f> & points, ofPoint center, int label);
    void setPoints(vector<ofVec2f> & points, ofPoint center);
    void draw();
    
    vector<ofVec2f> points;
    ofPoint center;
    int label;
    int age;
    ofColor color;
};




class Ribbon {
public:
    Ribbon(Contour *contour,
           int maxAge, int speed, int length, int skip,
           int margin, float noiseFactor, float ageFactor,
           float lineWidth, int maxAlpha, int updateRate,
           float lerpRate, float dilate, bool curved, bool match);
    
    void update();
    void addPoint(int p);
    void draw();
    
    Contour *getContour() {return contour;}
    bool getActive() {return active;}

private:
    
    int idx, idxMatched;
    int age, maxAge;
    int speed;
    int length;
    int skip;
    bool active;
    int margin;
    float lineWidth;
    int maxAlpha;
    float noiseFactor, ageFactor;
    int updateRate;
    float lerpRate;
    float dilate;
    bool curved, match;
    
    Contour *contour;
    vector<ofVec2f> points;
    vector<int> lookup;
    vector<float> lookupMatched;
};


class ContourRenderer {
public:
    void setup(OpenNi *openNi, int width, int height);
    void setCalibration(ofxKinectProjectorToolkit *kpt);
    void update();
    void draw();
    
    void setDrawRibbons(bool drawRibbons) {this->drawRibbons = drawRibbons;}
    void setDrawOutwardLines(bool drawOutwardLines) {this->drawOutwardLines = drawOutwardLines;}
    void setDrawPhysics(bool drawPhysics) {this->drawPhysics = drawPhysics;}
    
    void setGuiPosition(int x, int y) {control.setGuiPosition(x, y);}
    void setGuiVisible(bool visible);
    void toggleGuiVisible() {setGuiVisible(!visible);}
    
private:
    
    void checkChanges();
    void setupControl();
    
    void recordContours();
    void manageContours();
    
    void renderContours();
    
    void manageRibbons();
    void renderRibbons();
    
    void renderOutwardLines();
    
    void setupPhysics();
    void updatePhysics();
    void renderPhysics();
    void clearCircles(string &s) {circles.clear(); }

    // tracking
    vector<Contour *> contours;
    vector<vector<ofVec2f> > currentContours;
    OpenNi *openNi;
    ofxKinectProjectorToolkit *kpt;
    bool calibrated = false;
    int width, height;
    
    // ribbons
    vector<Ribbon *> ribbons;
    vector<int> labels;
    
    // drawing contours
    ofColor contourColor;
    int contourSmoothness;
    
    // physics
    ofxBox2d box2d;
	vector<ofPtr<ofxBox2dCircle> > circles;
	vector<ofPolyline> lines;
    vector<ofPtr<ofxBox2dEdge> > edges;
    int rate;
    float tolerance;
    float circleDensity, circleBounce, circleFriction;
    ofImage img;
    
    // parameters
    Control control;
    
    int smooth;
    float offset;
    float lineWidth;
    int length;
    ofColor color;
    bool centered;
    
    int maxAgeMin, maxAgeMax;
    int speedMin, speedMax;
    int lengthMin, lengthMax;
    int skipMin, skipMax;
    int marginMin, marginMax;
    float noiseFactorMin, noiseFactorMax;
    float ageFactorMin, ageFactorMax;
    float lineWidthMin, lineWidthMax;
    int maxAlphaMin, maxAlphaMax;
    float lerpRateMin, lerpRateMax;
    int updateRateMin, updateRateMax;
    float dilate;
    bool curved, match;
    int numNew;
    int threshold;
    int frameSkip;
    
    // drawing modes
    bool drawRibbons, pDrawRibbons, drawPhysics, drawContours;
    bool drawOutwardLines, pDrawOutwardLines, pDrawPhysics, pDrawContours;
    
    bool visible;
};