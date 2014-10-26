#pragma once

#include "ofMain.h"
#include "Constants.h"
#include "Birl.h"
#include "BirlParameter.h"



class BirlLearner : public Learn
{
public:
    BirlLearner();
    
    void setup(Birl *birl);
    void setupOscSender(string host, int port);

    void update();
    void draw();
    
    // add output
    BirlOutputParameter * addOutput(string name, float *value, float min, float max);
    BirlOutputParameter * addOutput(string name, float min, float max);
    
    // presets
    bool savePreset(string filename);
    void loadPreset(string filename, bool loadExamples=true);
    
    // style
    void setFont(string path);
    void setFontSizes(int small, int medium, int large);

protected:
    
    // gui
    void setupGui();
    void setGuiMode(BirlMode mode);
    void setGuiMode() {setGuiMode(mode);}
    void setPerformanceMode();
    void setTrainingMode();
    void toggleViewPreferences();
    void resetGuiPositions();

    // gui events
    void gui1Event(ofxUIEventArgs &e);
    void gui2Event(ofxUIEventArgs &e);
    void gui3Event(ofxUIEventArgs &e);
    void guiModeEvent(ofxUIEventArgs &e);
    void guiPresetsEvent(ofxUIEventArgs &e);
    void guiSettingsEvent(ofxUIEventArgs &e);
    void guiProcessSettings();
    
    // events
    void parameterSelected(LearnParameter & parameter);
    void resetInputs();
    void resetInputButtons();
    void setOutputTrainingSettings();
    
    // gui and mode
    BirlMode mode;
    ofxUICanvas *guiMode, *guiPresets, *guiSettings, *guiSettingsButton;
    ofxUITextInput *guiEmbouchureMax, *guiKeysMax, *guiKeysDiscreteMax, *guiHiddenLayers, *guiTargetRmse, *guiMaxSamples;
    
    // birl
    Birl *birl;
    
    // birl settings
    int embouchureMax, keysMax, hiddenLayers, maxSamples;
    float keysDiscreteMax, targetRmse;
    bool viewSettings;
    string settingsChanged;
    
    // presets
    string presetFilename;
    bool presetLoadedWithoutExamples;
};
