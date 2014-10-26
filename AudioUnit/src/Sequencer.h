#pragma once

#include "ofMain.h"
#include "ofxSequencer.h"
#include "Control.h"


class Sequencer
{
public:
    void setup(int rows, int cols);
    
    void setPosition(int x, int y, int w=400, int h=200);
    void setVisible(bool visible);
    void toggleVisible() {setVisible(!visible);}

    void setSmooth(bool smooth) {
        this->smooth = smooth;
        if (smooth) sequencer.addBeatListener(this, &Sequencer::playBeat);
        else        sequencer.removeBeatListener(this, &Sequencer::playBeat);
    }
    
    bool getSmooth() {return smooth;}
    
    void update();
    void draw();
    
    void setSize(int rows, int cols);
    int getNumberOfRows() {return rows;}
    int getNumberOfCols() {return cols;}
    
    void setValue(int row, int col, float value);
    float getValue(int row, int col);
    float getValueInterpolated(int row, int col);
    
    void setActive(bool active) {this->active = active;}
    void setDiscrete(bool discrete) {this->discrete = discrete;}
    void setBpm(int bpm) {this->bpm = bpm;}

    bool getActive() {return active;}
    bool getDiscrete() {return discrete;}
    int getBpm() {return bpm;}
    
    int getColumn() {return sequencer.getColumn();}
    int getNumberRows() {return sequencer.getNumberRows();}
    int getNumberColumns() {return sequencer.getNumberColumns();}
    
    void reset(string &b) {sequencer.reset();}
    void randomize(string &b) {sequencer.randomize();}
    
    template<typename ListenerClass, typename ListenerMethod>
    void addBeatListener(ListenerClass *listener, ListenerMethod method) {
        sequencer.addBeatListener(listener, method);
    }
    template<typename ListenerClass, typename ListenerMethod>
    void removeBeatListener(ListenerClass *listener, ListenerMethod method) {
        sequencer.removeBeatListener(listener, method);
    }

    template<typename ListenerClass, typename ListenerMethod>
    void addInterpolatedBeatListener(ListenerClass *listener, ListenerMethod method) {
        ofAddListener(interpolatedSequencerEvent, listener, method);
    }
    template<typename ListenerClass, typename ListenerMethod>
    void removeInterpolatedBeatListener(ListenerClass *listener, ListenerMethod method) {
        ofRemoveListener(interpolatedSequencerEvent, listener, method);
    }

    void playBeat(vector<float> &column) {
        bpmTime = ofGetElapsedTimeMillis();
    }


protected:
    
    ofxSequencer sequencer;
    Control control;
    bool smooth;
    bool active, pActive;
    bool discrete, pDiscrete;
    int rows, cols;
    int bpm, pBpm;
    float bpmTime;
    float bpmInterval;
    bool visible;
    int x, y;    
    ofEvent<vector<float> > interpolatedSequencerEvent;
};