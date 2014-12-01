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
    
    void makeTrainingSetFromGMM(int idxOutput) {
        
        int numInstances = 12;
        
        LearnOutputParameter * output = outputs->at(idxOutput);
        
        vector<LearnInputParameter*> ainputs = output->getActiveInputs();
        float min = output->getMin();
        float max = output->getMax();
        
        
        
        int numClusters = gmmClusterSets[idxOutput].size();
        
        


        
        vector<float> instance;
        for (int i=0; i<numInstances; i++) {
            
            // sample from one of the gaussians, according to its prior
            int idxCurrent = 0;
            float rand = ofRandom(1);
            float current = 0.0;
            while (rand > current) {
                current += gmmClusterSets[idxOutput][idxCurrent].prior;
                idxCurrent++;
            }
            vector<double> *mean = &gmmClusterSets[idxOutput][idxCurrent-1].mean;
            vector<double> *std = &gmmClusterSets[idxOutput][idxCurrent-1].std;

            // get fake output value
            double valAssigned = gmmClusterSets[idxOutput][idxCurrent-1].assignedValue;
            double valStd = gmmClusterSets[idxOutput][idxCurrent-1].assignedStd;
            cout << valAssigned << " " << valStd << "---"<<endl;
            double val = ofLerp(min, max, valAssigned + ofRandom(-1, 1) * valStd);
            
            // create instance
            instance.clear();
            instance.push_back(val);
            for (int p=0; p<mean->size(); p++) {
                float featValue = mean->at(p) + ofRandom(-1, 1) * std->at(p);
                instance.push_back(featValue);
            }
            output->addInstance(instance);
        }
        
        
        
        
        
        
    }
    
    void makeTrainingSetFromGMMAll() {
        for (int i=0; i<outputs->size(); i++) {
            makeTrainingSetFromGMM(i);
        }
    }
    
    
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
    vector<LearnInputParameter *> *inputs;
    ofxGMM gmm;
    vector<vector<GMMCluster> > gmmClusterSets;
    bool remapping;
    bool collecting, training;
    bool kMeansTrained, gmmTrained;

    int x, y;
    bool visible, gmmVisible;
    
    int idxDraggingCluster;
    bool dragging;
    
};

