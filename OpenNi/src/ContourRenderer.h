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


class LineSet {
public:
    LineSet(Contour *contour) {
        this->contour = contour;
        active = true;
    }
    
    void update() {
        if (lines.size() < 50) {
            addLine();
        }
        for (int i=0; i<lines.size(); i++) {
            float t = (float) i / 50;
            float width = 640;
            float height = 480;
            int idx = floor(contour->points.size() * t);
            float ang = t * TWO_PI;
            int x1 = width * 0.5 + width * cos(ang);
            int y1 = height * 0.5 + height * sin(ang);
            int x2 = contour->points[idx].x;
            int y2 = contour->points[idx].y;
            lines[i].z = ofLerp(lines[i].z, x2, 0.1);
            lines[i].w = ofLerp(lines[i].w, y2, 0.1);
        }
    }
    
    void draw() {
        for (int i=0; i<lines.size(); i++) {
            ofLine(lines[i].x, lines[i].y, lines[i].z, lines[i].w);
        }
    }
    
    void addLine() {
        float t = (float) lines.size() / 50;
        float width = 640;
        float height = 480;
        int idx = floor(contour->points.size() * t);
        float ang = t * TWO_PI;
        int x1 = width * 0.5 + width * cos(ang);
        int y1 = height * 0.5 + height * sin(ang);
        int x2 = contour->points[idx].x;
        int y2 = contour->points[idx].y;
        ofVec4f newLine = ofVec4f(x1, y1, x2, y2);
        lines.push_back(newLine);
    }
    
    void removeLine() {
        if (lines.size() != 0) {
            lines.erase(lines.begin());
        }
        if (lines.size() == 0) {
            active = false;
        }
    }
    
    Contour *getContour() {return contour;}
    bool getActive() {return active;}
    
private:
    
    Contour *contour;
    vector<ofVec4f> lines;
    bool active;
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
    void setGuiPosition(int x, int y) {control.setGuiPosition(x, y);}
    void setCalibration(ofxKinectProjectorToolkit *kpt);
    void update();
    void draw();

private:

    void setupControl();
    
    void recordContours();
    
    void manageContours();
    void manageRibbons();
    
    void manageOutwardLines();
    
    void renderRibbons();
    void renderOutwardLines();
    
    // tracking
    vector<Contour *> contours;
    OpenNi *openNi;
    ofxKinectProjectorToolkit *kpt;
    bool calibrated = false;
    int width, height;
    
    // ribbons
    vector<Ribbon *> ribbons;
    vector<int> labels;
    
    // outward lines
    vector<LineSet *> lines;
    
    // parameters
    Control control;
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
    bool drawRibbons;
    bool drawOutwardLines;
};