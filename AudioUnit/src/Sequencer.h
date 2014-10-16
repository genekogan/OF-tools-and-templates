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

    void update();
    void draw();
    
    void setValue(int row, int col, float value);
    
    int getColumn() {return sequencer.getColumn();}
    int getNumberRows() {return sequencer.getNumberRows();}
    int getNumberColumns() {return sequencer.getNumberColumns();}
    
    void reset(string &b) {sequencer.reset();}
    
    template<typename ListenerClass, typename ListenerMethod>
    void addBeatListener(ListenerClass *listener, ListenerMethod method) {
        sequencer.addBeatListener(listener, method);
    }

protected:
    
    ofxSequencer sequencer;
    Control control;
    bool active, pActive;
    bool discrete, pDiscrete;
    int bpm, pBpm;
    bool visible;
};