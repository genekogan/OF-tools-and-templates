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
    
    // gui
    void setupGui();
    void setupGuiPreview();

    // visibility
    void setGuiPreviewPosition(int x, int y);
    void setPreviewMode(BirlMode mode);
    void setVisible(bool visible);
    void deselect();

    // style
    void setFont(string path);
    void setFontSizes(int small, int medium, int large);

    // input groups
    bool inputKeys, inputKeysDiscrete, inputPressure, inputEmbouchure;

protected:

    // gui events
    void guiEvent(ofxUIEventArgs &e);
    void guiPreviewEvent(ofxUIEventArgs &e);
    void guiPreviewSetMinMax();
    
    // recording and training
    void setInputs();
    void setRecording(bool record);
    void setTrained(bool trained);
    void predict();
    
    // extra gui elements
    ofxUICanvas *guiPreview, *guiPerform;
    ofxUISlider *guiPerformValue;
    ofxUITextInput *guiPreviewOsc, *guiPerformOsc, *guiPerformValueText;
    ofxUILabelToggle *guiInputKeys, *guiInputKeysDiscrete, *guiInputPressure, *guiInputEmbouchure;
};
