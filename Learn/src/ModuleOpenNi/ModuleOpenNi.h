#pragma once

#include "ofMain.h"
#include "Learn.h"
#include "OpenNi.h"


class ModuleOpenNi : public Learn
{
public:
    ~ModuleOpenNi() {openNi.close();}
    ModuleOpenNi();
    
    void update();
    void draw();
    
    void addJointsAsInput();
    
protected:
    
    OpenNi openNi;

};
