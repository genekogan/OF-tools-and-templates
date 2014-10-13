#pragma once

#include "ofMain.h"
#include "ofxManta.h"
#include "ofxConvexHull.h"
#include "Control.h"


class MantaController
{
public:
    ~MantaController();
    void setup();
    void close();

    void update();
    void draw(int x, int y, int w);
    void drawStats(int x, int y, int w);
    
    // get pads
    float getPad(int row, int col) {return manta.getPad(row, col);}
    float * getPadRef(int row, int col) {return manta.getPadRef(row, col);}

    // get sliders
    float getSlider(int index) {return manta.getSlider(index);}
    float * getSliderRef(int index) {return manta.getSliderRef(index);}

    // get buttons
    float getButton(int index) {return manta.getButton(index);}
    float * getButtonRef(int index) {return manta.getButtonRef(index);}

    // get stats
    float & getNumFingers() {return numFingers;}
    float & getPadSum() {return padSum;}
    float & getPadAverage() {return padAverage;}
    float & getPerimeter() {return perimeter;}
    float & getAverageInterFingerDistance() {return averageInterFingerDistance;}
    float & getCentroidX() {return centroidX;}
    float & getCentroidY() {return centroidY;}
    float & getWeightedCentroidX() {return weightedCentroidX;}
    float & getWeightedCentroidY() {return weightedCentroidY;}
    
    // returns [0,1]x[0,1] position of (row,col)
    ofPoint getPositionAtPad(int row, int col);
    
    // add event listeners
    template <typename L, typename M> void addPadListener(L *listener, M method) {manta.addPadListener(listener, method);}
    template <typename L, typename M> void addSliderListener(L *listener, M method) {manta.addSliderListener(listener, method);}
    template <typename L, typename M> void addButtonListener(L *listener, M method) {manta.addButtonListener(listener, method);}
    template <typename L, typename M> void addPadVelocityListener(L *listener, M method) {manta.addPadVelocityListener(listener, method);}
    template <typename L, typename M> void addButtonVelocityListener(L *listener, M method) {manta.addButtonVelocityListener(listener, method);}
    
protected:
    
    ofxManta manta;
    bool isConnected;
    ofxConvexHull convexHull;
    
    // finger trackers
    vector<ofPoint> fingers, fingersHull;
    vector<float> fingerValues;
    
    // finger statistics
    float numFingers;
    float padSum, padAverage;
    float centroidX, centroidY;
    float weightedCentroidX, weightedCentroidY;
    float averageInterFingerDistance;
    float perimeter;

};
