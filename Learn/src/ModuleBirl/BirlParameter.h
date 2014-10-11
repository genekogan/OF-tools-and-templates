#pragma once

#include "ofMain.h"
#include "Learn.h"
#include "Control.h"
#include "Constants.h"


enum BirlMode {
    BIRL_INPUTS,
    BIRL_TRAIN,
    BIRL_PERFORM };


class BirlOutputParameter : public LearnOutputParameter
{
public:
    BirlOutputParameter(string name, float *val, float min, float max);
    ~BirlOutputParameter();
    
    void draw();
    
    void setupGui();
    void setupGuiPreview();
    void setGuiPreviewPosition(int x, int y);
    void setPreviewMode(BirlMode mode);
    void deselect();
    
    void guiEvent(ofxUIEventArgs &e);
    void guiPreviewEvent(ofxUIEventArgs &e);

    bool inputKeys, inputKeysDiscrete, inputPressure, inputEmbouchure;
    
protected:

    void predict();

    void setInputs();
    void setTrained(bool trained);
    
    ofxUICanvas *guiPreview, *guiPerform;
    ofxUISlider *guiPerformValue;
    ofxUITextInput *guiPreviewOsc, *guiPerformOsc, *guiPerformValueText;
    ofxUILabelToggle *guiInputKeys, *guiInputKeysDiscrete, *guiInputPressure, *guiInputEmbouchure;
};
