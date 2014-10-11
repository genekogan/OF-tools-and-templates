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
    void update();
    void draw(int x, int y, int w);
    
    
    float getPad(int x, int y);
    
    float getPadSum();
    float getPadAverage();
    
    float getPerimeter();
    float getAverageInterFingerDistance();

    float getCentroid();
    float getWeightedCentroid();
    int getNumFingers();
    
    
    void close();
    

    float * getPadRef(int x, int y);

    void setGuiPosition(int x, int y);
    
    
    
    /* add event listeners */
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

    ofxManta manta;
    ofxConvexHull convexHull;
    
//    Control control;
};
