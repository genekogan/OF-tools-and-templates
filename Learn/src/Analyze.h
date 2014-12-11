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
    void setInputs(vector<LearnInputGroup *> *inputs);
    void setOutputs(vector<LearnOutputParameter *> *outputs);

    void update();
    void draw();
    
    void drawGMM(int g);
    
    void toggleCollecting(string &s);
    void startCollecting();
    void clear(string &s);

    void startTraining(string &s);
    void trainGMM(string &s);
    void trainNextOutput();
    void proceedTraining();
    
    void setGuiPosition(int x, int y);
    void setVisible(bool visible);
    void toggleVisible() {setVisible(!visible);}
    
    void autoTrain(string &s);
    void makeTrainingSetFromGMM(int idxOutput);
    void makeTrainingSetFromGMMAll();
    
private:
    
    struct GMMCluster {
        string name;
        double prior;
        vector<double> mean;
        vector<double> std;
        double assignedValue, assignedStd;
        GMMCluster(string n, double p, vector<double> m, vector<double> s) {
            name = n;
            prior = p;
            mean = m;
            std = s;
            assignedValue = 0;
            assignedStd = 0;
        }
    };
    
    void setInputMapping();
    void selectOutput(string &s);
    
    void mousePressed(ofMouseEventArgs &e);
    void mouseDragged(ofMouseEventArgs &e);
    void mouseReleased(ofMouseEventArgs &e);
    
    void setupGui();
    
    Control control;
    
    ofxLearnThreaded analyzer;
    vector<vector<int> > clusters;
    
    vector<ofxSpreadsheet*> spreadsheets;
    int idxView, idxTrain;
    
    vector<LearnOutputParameter *> *outputs;
    vector<LearnInputGroup *> *inputs;
    
    ofxGMM gmm;
    vector<vector<GMMCluster> > gmmClusterSets;
    bool remapping;
    bool collecting, training;
    bool kMeansTrained, gmmTrained;

    int x, y;
    bool visible, gmmVisible;
    
    int idxDraggingCluster;
    bool dragging;
    
    int numAutoSamples;
    
};

