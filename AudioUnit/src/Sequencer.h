#pragma once

#include "ofMain.h"
#include "ofxSequencer.h"
#include "Control.h"


class Sequencer {
public:
    void setup(int rows, int cols);
    
    void setValue(int row, int col, float value) {
        sequencer.setValue(row, col, value);
    }
    
    
    
    void setPosition(int x, int y) {
        sequencer.setPosition(x, y, 400, 200);
        control.setGuiPosition(x+405, y);
    }
    
    void setVisible(bool visible) {
        this->visible = visible;
        sequencer.setVisible(visible);
        control.setVisible(visible);
    }
    
    void toggleVisible() {setVisible(!visible);}
    
    void update();    
    void draw();
    
    
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