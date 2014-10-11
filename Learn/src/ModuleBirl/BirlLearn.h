#pragma once

#include "ofMain.h"
#include "Birl.h"
#include "BirlParameter.h"



class BirlLearner : public Learn
{
public:
    BirlLearner();
    void setup(Birl &birl);
    void setupOscSender(string host, int port);
    void setupGui();
    
    BirlOutputParameter * addOutput(string name, float *val, float min, float max);
    BirlOutputParameter * addOutput(string name, float min, float max) {return addOutput(name, new float(), min, max);}
    void resetInputs();
    void resetGuiPositions();
    
    void setPerformanceMode();
    void setTrainingMode();
    void setGuiMode(BirlMode mode);
    void setGuiMode() {setGuiMode(mode);}
    
    bool savePreset(string filename);
    void loadPreset(string filename);
        
protected:
    
    void gui1Event(ofxUIEventArgs &e);
    void gui2Event(ofxUIEventArgs &e);
    void gui3Event(ofxUIEventArgs &e);
    void guiModeEvent(ofxUIEventArgs &e);
    void guiPresetsEvent(ofxUIEventArgs &e);
    
    void parameterSelected(LearnParameter & parameter);
    void resetInputButtons();

    BirlMode mode;
    ofxUICanvas *guiMode;
    ofxUICanvas *guiPresets;
    
    Birl birl;
};
