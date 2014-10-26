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
    
    Control *control;
    
    Sequencer *sequencer;
    
};