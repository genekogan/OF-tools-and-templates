#pragma once

#include "ofMain.h"
#include "ofxManta.h"
#include "ofxConvexHull.h"
#include "Control.h"


// todo:
//  - shape summary features
//  - check face detection research
//


enum MantaType {
    PAD,
    SLIDER,
    BUTTON
};

class MantaElement {
public:
    MantaType type;
    int element;
    int selection;
    MantaElement(MantaType type, int element, int selection) {
        this->type = type;
        this->element = element;
        this->selection = selection;
    }
    MantaElement() {element = -1;}
};

bool operator <( MantaElement const& left, MantaElement const& right ) {
    return left.element < right.element || ( left.element == right.element && left.selection < right.selection );
}



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
    float * getPadVelocityRef(int row, int col) {return &padVelocity[row][col];}

    // get sliders
    float getSlider(int index) {return manta.getSlider(index);}
    float * getSliderRef(int index) {return manta.getSliderRef(index);}
    float * getSliderVelocityRef(int index) {return &sliderVelocity[index];}

    // get buttons
    float getButton(int index) {return manta.getButton(index);}
    float * getButtonRef(int index) {return manta.getButtonRef(index);}
    float * getButtonVelocityRef(int index) {return &buttonVelocity[index];}
    
    // get stats
    float & getNumPads() {return numPads;}
    float & getPadSum() {return padSum;}
    float & getPadAverage() {return padAverage;}
    float & getPerimeter() {return perimeter;}
    float & getAverageInterFingerDistance() {return averageInterFingerDistance;}
    float & getCentroidX() {return centroidX;}
    float & getCentroidY() {return centroidY;}
    float & getWeightedCentroidX() {return weightedCentroidX;}
    float & getWeightedCentroidY() {return weightedCentroidY;}
    float & getWidth() {return padWidth;}
    float & getHeight() {return padHeight;}
    float & getWidthHeightRatio() {return whRatio;}

    float & getNumPadsVelocity() {return numPadsVelocity;}
    float & getPadSumVelocity() {return padSumVelocity;}
    float & getPadAverageVelocity() {return padAverageVelocity;}
    float & getPerimeterVelocity() {return perimeterVelocity;}
    float & getAverageInterFingerDistanceVelocity() {return averageInterFingerDistanceVelocity;}
    float & getCentroidVelocityX() {return centroidVelocityX;}
    float & getCentroidVelocityY() {return centroidVelocityY;}
    float & getWeightedCentroidVelocityX() {return weightedCentroidVelocityX;}
    float & getWeightedCentroidVelocityY() {return weightedCentroidVelocityY;}
    float & getWidthVelocity() {return widthVelocity;}
    float & getHeightVelocity() {return heightVelocity;}
    float & getWidthHeightRatioVelocity() {return whRatioVelocity;}

    // mark manta
    void markPad(int row, int col, bool mark);
    void markSlider(int index, int column);
    void markButton(int index, bool mark);
    
    // returns gui element positions
    ofPoint getPositionAtPad(int row, int col);
    
    // get selection
    vector<int> getPadSelection() {return manta.getPadSelection(0);}
    vector<int> getPadVelocitySelection() {return manta.getPadSelection(1);}
    vector<int> getSliderSelection() {return manta.getSliderSelection(0);}
    vector<int> getSliderVelocitySelection() {return manta.getSliderSelection(1);}
    vector<int> getButtonSelection() {return manta.getButtonSelection(0);}
    vector<int> getButtonVelocitySelection() {return manta.getButtonSelection(1);}
    
    // set selection
    void addPadToSelection(int idx, int selection) {manta.addPadToSelection(idx, selection);}
    void addSliderToSelection(int idx, int selection) {manta.addSliderToSelection(idx, selection);}
    void addButtonToSelection(int idx, int selection) {manta.addButtonToSelection(idx, selection);}
    void setPadSelection(vector<int> idx, int selection);
    void setSliderSelection(vector<int> idx, int selection);
    void setButtonSelection(vector<int> idx, int selection);
    
    void clearSelection() {manta.clearSelection();}
    int getSizeSelection();

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
    template <typename L, typename M> void addClickListener(L *listener, M method) {ofAddListener(clickEvent, listener, method);}
    template <typename L, typename M> void removeClickListener(L *listener, M method) {ofRemoveListener(clickEvent, listener, method);}

    
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
    float buttonVelocity[4];
    float prevPad[8][6];
    float prevSlider[2];
    float prevButton[4];

    // finger statistics
    float numPads;
    float padSum, padAverage;
    float centroidX, centroidY;
    float weightedCentroidX, weightedCentroidY;
    float averageInterFingerDistance;
    float perimeter;
    float padWidth, padHeight, whRatio;
    
    // velocity statistics
    float numPadsVelocity;
    float padSumVelocity, padAverageVelocity;
    float centroidVelocityX, centroidVelocityY;
    float weightedCentroidVelocityX, weightedCentroidVelocityY;
    float averageInterFingerDistanceVelocity;
    float perimeterVelocity;
    float widthVelocity, heightVelocity, whRatioVelocity;

    // callbacks
    void mousePressed(ofMouseEventArgs &evt);
    void mouseDragged(ofMouseEventArgs &evt);
    void mouseReleased(ofMouseEventArgs &evt);
    void keyPressed(ofKeyEventArgs &e);
    void keyReleased(ofKeyEventArgs &e);
    void setMouseResponders();

    
    ofEvent<MantaElement> clickEvent;
    bool mouseActive;
    bool dragging;
    ofPoint dragPoint1, dragPoint2;
    ofRectangle padPositions[48], sliderPositions[2], buttonPositions[4];
    ofRectangle mainDrawRect, statsDrawRect;
    int x, y, width, height;
    int px, py, pwidth;
    bool visible, animated;
    int selection = 0;
    bool shift;
};
