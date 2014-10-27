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
    
    void sequencerStepEvent(vector<float> &column);
    void sequencerInterpolatedStepEvent(vector<float> &column);

    
    Control *control;
    
    Sequencer *sequencer;
    
    vector<ParameterBase *> seqParams;
};