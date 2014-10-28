#pragma once

#include "Control.h"


class Control;
class Sequencer;


class MetaController
{
public:
    
    void setup(Control *control);
    void update();
    void draw();
    
    void setPosition(int x, int y, int width=200, int height=150);
    void setVisible(bool visible);
    void toggleVisible() {setVisible(!visible);}
    
    void sequencerStepEvent(vector<float> &column);
    void sequencerInterpolatedStepEvent(vector<float> &column);
    void processColumn(vector<float> &column);
    
    void mousePressed(ofMouseEventArgs &evt);
    
    Control *control;
    Sequencer *sequencer;
    
    vector<ParameterBase *> seqParams;
    vector<bool> seqActive;
    
    int x, y, width, height;
    bool visible;
    
    void drawLabel(int paramIdx, int idx, string suffix="");
};