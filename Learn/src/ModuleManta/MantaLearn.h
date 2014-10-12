#pragma once

#include "ofMain.h"
#include "MantaController.h"
#include "Learn.h"



class MantaLearn : public Learn
{
public:
    MantaLearn();
    
    void update();
    void draw();
    
    void addAllPadsAsInput();
    
protected:

    MantaController manta;
};
