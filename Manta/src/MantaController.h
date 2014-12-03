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
    
    void setMouseActive(bool active);
    void setVisible(bool visible);
    void toggleVisible() {setVisible(!visible);}
    
    float & getVelocityLerpRate() {return velocityLerpRate;}
    void setVelocityLerpRate(float v) {velocityLerpRate=v;}
    
    void update();
    void draw(int x, int y, int width);
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
    
    // get shape
    // getNormalizedConvexHull (get shape)
    // getBoundingBox
    // get width/height ratio
    
    float * getPadVelocityRef(int row, int col) {return &padVelocity[row][col];}
    float * getSliderVelocityRef(int index) {return &sliderVelocity[index];}

    float & getPadSumVelocity() {return padSumVelocity;}
    float & getPadAverageVelocity() {return padAverageVelocity;}
    float & getPerimeterVelocity() {return perimeterVelocity;}
    float & getAverageInterFingerDistanceVelocity() {return averageInterFingerDistanceVelocity;}
    float & getCentroidVelocityX() {return centroidVelocityX;}
    float & getCentroidVelocityY() {return centroidVelocityY;}
    float & getWeightedCentroidVelocityX() {return weightedCentroidVelocityX;}
    float & getWeightedCentroidVelocityY() {return weightedCentroidVelocityY;}
    
    // mark manta
    void markPad(int row, int col, bool mark);
    void markSlider(int index, int column);
    void markButton(int index, bool mark);
    
    // returns gui element positions
    ofPoint getPositionAtPad(int row, int col);
    
    // wrap manta event listeners
    template <typename L, typename M> void addPadListener(L *listener, M method) {manta.addPadListener(listener, method);}
    template <typename L, typename M> void addSliderListener(L *listener, M method) {manta.addSliderListener(listener, method);}
    template <typename L, typename M> void addButtonListener(L *listener, M method) {manta.addButtonListener(listener, method);}
    template <typename L, typename M> void addPadVelocityListener(L *listener, M method) {manta.addPadVelocityListener(listener, method);}
    template <typename L, typename M> void addButtonVelocityListener(L *listener, M method) {manta.addButtonVelocityListener(listener, method);}
    template <typename L, typename M> void removePadListener(L *listener, M method) {manta.removePadListener(listener, method);}
    template <typename L, typename M> void removeSliderListener(L *listener, M method) {manta.removeSliderListener(listener, method);}
    template <typename L, typename M> void removeButtonListener(L *listener, M method) {manta.removeButtonListener(listener, method);}
    template <typename L, typename M> void removePadVelocityListener(L *listener, M method) {manta.removePadVelocityListener(listener, method);}
    template <typename L, typename M> void removeButtonVelocityListener(L *listener, M method) {manta.removeButtonVelocityListener(listener, method);}
    
    // add gui interaction listeners
    template <typename L, typename M> void addPadClickListener(L *listener, M method) {ofAddListener(padClickEvent, listener, method);}
    template <typename L, typename M> void addSliderClickListener(L *listener, M method) {ofAddListener(sliderClickEvent, listener, method);}
    template <typename L, typename M> void addButtonClickListener(L *listener, M method) {ofAddListener(buttonClickEvent, listener, method);}
    template <typename L, typename M> void removePadClickListener(L *listener, M method) {ofRemoveListener(padClickEvent, listener, method);}
    template <typename L, typename M> void removeSliderClickListener(L *listener, M method) {ofRemoveListener(sliderClickEvent, listener, method);}
    template <typename L, typename M> void removeButtonClickListener(L *listener, M method) {ofRemoveListener(buttonClickEvent, listener, method);}
    
protected:
    
    ofxManta manta;
    bool isConnected;
    ofxConvexHull convexHull;
    
    // parameters
    float velocityLerpRate;
    
    // finger trackers
    vector<ofPoint> fingers, fingersHull, fingersHullNormalized;
    vector<float> fingerValues;
    
    // tracking pads and sliders (for velocity)
    float padVelocity[8][6];
    float sliderVelocity[2];
    float prevPad[8][6];
    float prevSlider[2];

    // finger statistics
    float numFingers;
    float padSum, padAverage;
    float centroidX, centroidY;
    float weightedCentroidX, weightedCentroidY;
    float averageInterFingerDistance;
    float perimeter;
    
    // velocity statistics
    float padSumVelocity, padAverageVelocity;
    float centroidVelocityX, centroidVelocityY;
    float weightedCentroidVelocityX, weightedCentroidVelocityY;
    float averageInterFingerDistanceVelocity;
    float perimeterVelocity;

    // mouse callbacks
    void mousePressed(ofMouseEventArgs &evt);
    
    ofEvent<int> padClickEvent, sliderClickEvent, buttonClickEvent;
    bool mouseActive;
    void setMouseResponders();
    ofRectangle padPositions[48], sliderPositions[2], buttonPositions[4];
    int x, y, width;
    int px, py, pwidth;
    bool visible;
};
