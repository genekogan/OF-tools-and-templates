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
    
    float * getPadRef(int x, int y);
    float getPad(int x, int y);
    
    int   & getNumFingers() {return numFingers;}
    float & getPadSum() {return padSum;}
    float & getPadAverage() {return padAverage;}
    float & getPerimeter() {return perimeter;}
    float & getAverageInterFingerDistance() {return averageInterFingerDistance;}
    float & getCentroidX() {return centroidX;}
    float & getCentroidY() {return centroidY;}
    float & getWeightedCentroidX() {return weightedCentroidX;}
    float & getWeightedCentroidY() {return weightedCentroidY;}
    
    // add event listeners
    template <typename L, typename M> void addPadListener(L *listener, M method) {
        manta.addPadListener(listener, method);
    }
    template <typename L, typename M> void addSliderListener(L *listener, M method) {
        manta.addSliderListener(listener, method);
    }
    template <typename L, typename M> void addButtonListener(L *listener, M method) {
        manta.addButtonListener(listener, method);
    }
    template <typename L, typename M> void addPadVelocityListener(L *listener, M method) {
        manta.addPadVelocityListener(listener, method);
    }
    template <typename L, typename M> void addButtonVelocityListener(L *listener, M method) {
        manta.addButtonVelocityListener(listener, method);
    }
    
protected:
    
    ofPoint getPositionAtPad(int row, int col);
    
    ofxManta manta;
    bool isConnected;
    
    ofxConvexHull convexHull;
    
    // finger trackers
    vector<ofPoint> fingers, fingersHull;
    vector<float> fingerValues;
    
    // finger statistics
    int numFingers;
    float padSum, padAverage;
    float centroidX, centroidY;
    float weightedCentroidX, weightedCentroidY;
    float averageInterFingerDistance;
    float perimeter;

};
