#pragma once

#include "ofMain.h"
#include "ofxSpreadsheet.h"
#include "ofxGMM.h"
#include "ofxLearn.h"
#include "Control.h"
#include "LearnParameter.h"




class Analyze
{
public:
    Analyze();
    
    void setup();
    void setInputs(vector<LearnInputParameter *> *inputs);
    void setOutputs(vector<LearnOutputParameter *> *outputs);
    void update();
    void draw();
    
    void drawGMM(int g);
    
    void toggleRemapping(string &s);
    void toggleCollecting(string &s);
    void startCollecting();
    void startTraining(string &s);
    void trainGMM(string &s);
    void trainNextOutput();
    void proceedTraining();
    
    void setGuiPosition(int x, int y);
    void setVisible(bool visible);
    void toggleVisible() {setVisible(!visible);}
    
private:
    
    struct GMMCluster {
        string name;
        double prior;
        vector<double> mean;
        vector<double> std;
        GMMCluster(string n, double p, vector<double> m, vector<double> s) {
            name = n;
            prior = p;
            mean = m;
            std = s;
        }
    };
    
    void setInputMapping() {
        for (int i=0; i<inputs->size(); i++) {
            if (inputs->at(i)->get() > inputs->at(i)->getMax()) {
                inputs->at(i)->setMax(inputs->at(i)->get());
            }
            if (inputs->at(i)->get() < inputs->at(i)->getMin()) {
                inputs->at(i)->setMin(inputs->at(i)->get());
            }
        }
    }
    
    void selectOutput(string &s) {
        for (int i=0; i<spreadsheets.size(); i++) {
            spreadsheets[i]->setInputsActive(false);
        }
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
    
    vector<LearnOutputParameter *> *outputs;
    vector<LearnInputParameter *> *inputs;
    ofxGMM gmm;
    vector<vector<GMMCluster> > gmmClusterSets;
    bool remapping;
    bool collecting, training;
    bool kMeansTrained, gmmTrained;

    int x, y;
    bool visible;
};

