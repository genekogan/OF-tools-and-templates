#pragma once

#include "ofMain.h"
#include "Control.h"
#include "ofxLearn.h"
#include "Learn.h"
#include "ofxSpreadsheet.h"


class Analyzer
{
public:
    Analyzer();

    void setup(Learn *learn);
    void update();
    void draw();
    
    void toggleCollecting(string &s);
    void startCollecting();
    void startTraining(string &s);
    void trainNextOutput();
    void proceedTraining();
    
    void setGuiPosition(int x, int y);
    void setVisible(bool visible);
    void toggleVisible() {setVisible(!visible);}

private:

    void selectOutput(string &s) {
        for (int i=0; i<spreadsheets.size(); i++) {
            spreadsheets[i]->setInputsActive(false);
        }
        vector<LearnOutputParameter *> *outputs = learn->getOutputs();
        for (int i=0; i<outputs->size(); i++) {
            if (s == outputs->at(i)->getName()) {
                idxView = i;
                spreadsheets[idxView]->setInputsActive(true);
                return;
            }
        }
    }

    
    Control control;
    
    ofxLearnThreaded analyzer;
    vector<vector<int> > clusters;
    
    vector<ofxSpreadsheet*> spreadsheets;
    int idxView, idxTrain;
    
    Learn *learn;
    bool collecting, training;
    
    int x, y;
    bool visible;
};

