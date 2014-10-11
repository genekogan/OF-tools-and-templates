#pragma once

#include "ofMain.h"
#include "MantaController.h"
#include "Learn.h"


class MantaOutputParameter : public LearnOutputParameter
{
public:
    MantaOutputParameter(string name, float* val, float min, float max) : LearnOutputParameter(name, val, min, max) {
        guiFeatureSelector = new ofxUICanvas("features");
        ofAddListener(guiFeatureSelector->newGUIEvent, this, &MantaOutputParameter::guiFeatureSelectEvent);
        setupFeatureSelectorGui();
    }
    
    void setupFeatureSelectorGui() {
        guiFeatureSelector->setPosition(420, 500);
        guiFeatureSelector->setColorOutline(ofColor(255,200));
        guiFeatureSelector->setDrawOutline(true);
        guiFeatureSelector->clearWidgets();

        vector<string> inputLabels;
        inputLabels.push_back("all pads");
        inputLabels.push_back("all velocity");
        guiFeatureList = guiFeatureSelector->addDropDownList("select inputs", inputLabels, 200.0f);
        
        /*
        vector<ofxUILabelToggle *> toggles = guiSelector->getToggles();
        for (int i=0; i<toggles.size(); i++) {
            string inputName = toggles[i]->getName();
            for (int j=0; j<activeInputs.size(); j++) {
                if (inputName == activeInputs[j]->getName()) {
                    toggles[i]->setValue(value);
                }
            }
        }
         */
        
        
        guiFeatureList->setAutoClose(false);
        guiFeatureList->setAllowMultiple(true);
        guiFeatureList->open();
        guiFeatureSelector->autoSizeToFitWidgets();
        guiFeatureSelector->setVisible(true);

        
        setupHeaders();
    }
    
    
    void guiFeatureSelectEvent(ofxUIEventArgs &e) {
        if (e.getName() == "all pads") {
            cout << "all pads!!!" << endl;
        }
    }
    
    ofxUICanvas *guiFeatureSelector;
    ofxUIDropDownList *guiFeatureList;
};



class MantaLearn : public Learn
{
public:
    MantaLearn();
    
    MantaOutputParameter * addOutput(string name, float *val, float min, float max);
    MantaOutputParameter * addOutput(string name, float min, float max) {return addOutput(name, new float(), min, max);}

    void update();
    void draw();
    
    void addAllPadsAsInputs();
    
    
protected:

    vector<MantaOutputParameter *> outputs;
    MantaController manta;
};
