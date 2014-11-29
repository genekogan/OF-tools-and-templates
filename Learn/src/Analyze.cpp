#include "Analyze.h"


//-----------
Analyze::Analyze() {
    control.setVisible(false);
}

//-----------
void Analyze::setup() {
    visible = true;
    remapping = false;
    collecting = false;
    
    control.setVisible(true);
    control.clear();
    control.setName("Analyze");
    control.addEvent("remap inputs", this, &Analyze::toggleRemapping);
    control.addEvent("collect", this, &Analyze::toggleCollecting);
    control.addEvent("train kmeans", this, &Analyze::startTraining);
    control.addEvent("train gmm", this, &Analyze::trainGMM);
}

//-----------
void Analyze::setInputs(vector<LearnInputParameter *> *inputs) {
    this->inputs = inputs;
}

//-----------
void Analyze::setOutputs(vector<LearnOutputParameter *> *outputs) {
    this->outputs = outputs;
}

//-----------
void Analyze::toggleRemapping(string &s) {
    remapping = !remapping;
    if (remapping) {
        bool confirm = ofSystemChoiceDialog("Warning: this will erase all output classifiers and examples. Proceed?");
        if (!confirm) {
            remapping = false;
            return;
        }
        for (int i=0; i<outputs->size(); i++) {
            outputs->at(i)->clearInstances();
        }
        for (int i=0; i<inputs->size(); i++) {
            inputs->at(i)->setMax(-9999999);
            inputs->at(i)->setMin( 9999999);
        }
    }
}

//-----------
void Analyze::toggleCollecting(string &s) {
    collecting = !collecting;
    if (collecting && spreadsheets.size() == 0) {
        startCollecting();
    }
}

//-----------
void Analyze::startCollecting() {    
    vector<string> outputNames;
    for (int i=0; i<outputs->size(); i++) {
        outputNames.push_back(outputs->at(i)->getName());
        
        vector<LearnInputParameter *> ainputs = outputs->at(i)->getActiveInputs();
        ofxSpreadsheet *spreadsheet = new ofxSpreadsheet();
        vector<string> headers;
        headers.push_back("cluster");
        for (int j=0; j<ainputs.size(); j++) {
            headers.push_back(ainputs[j]->getName());
        }
        spreadsheet->setup(600, 300);
        spreadsheet->setHeaders(headers);
        spreadsheets.push_back(spreadsheet);
        
        vector<int> newClusters;
        clusters.push_back(newClusters);
    }
    
    control.addMenu("outputs", outputNames, this, &Analyze::selectOutput);
}

//-----------
void Analyze::startTraining(string &s) {
    idxTrain = 0;
    training = true;
    trainNextOutput();
}

//-----------
void Analyze::trainGMM(string &s) {
    gmmClusterSets.clear();
    for (int idx=0; idx<spreadsheets.size(); idx++) {
        vector<vector<float> > entries = spreadsheets[idx]->getEntries();
        gmm.setup(entries[0].size()-1, 3);
        cout << "SETUP GMM " << entries[0].size() << " clusters " << 3 << endl;
        for (int i=0; i<entries.size(); i++) {
            vector<double> instance;
            for (int j=1; j<entries[i].size(); j++) {
                instance.push_back((double) entries[i][j]);
            }
            gmm.addSample(instance);
        }
        gmm.train();
        
        vector<GMMCluster> paramCluster;
        for (int i=0; i<gmm.getNumGaussians(); i++) {
            cout << "NEW CLUSTER "<<i << " : size " << gmm.getMean(i).size() << endl;
            GMMCluster cluster(outputs->at(idx)->getName(),
                               gmm.getPrior(i), gmm.getMean(i), gmm.getStandardDeviation(i));
            cout << ofToString(gmm.getMean(i)) << endl;
            cout << ofToString(gmm.getStandardDeviation(i)) << endl;
            cout << "========" << endl;
            paramCluster.push_back(cluster);
        }
        gmmClusterSets.push_back(paramCluster);
    }
    gmmTrained = true;
}

//-----------
void Analyze::trainNextOutput() {
    analyzer.clearTrainingInstances();
    vector<vector<float> > entries = spreadsheets[idxTrain]->getEntries();
    for (int i=0; i<entries.size(); i++) {
        analyzer.addTrainingInstance((vector<double>&) entries[i]);
    }
    analyzer.beginTrainClusters(3);
}

//-----------
void Analyze::update() {
    if (remapping) {
        setInputMapping();
    }
    if (collecting) {
        for (int i=0; i<outputs->size(); i++) {
            vector<LearnInputParameter *> ainputs = outputs->at(i)->getActiveInputs();
            vector<float> entry;
            entry.push_back(-1);
            for (int i=0; i<ainputs.size(); i++) {
                entry.push_back(ainputs[i]->get());
            }
            spreadsheets[i]->addEntry(entry);
        }
    }
    if (analyzer.getTrained()) {
        clusters[idxTrain] = analyzer.getClusters();
        for (int i=0; i<clusters[idxTrain].size(); i++) {
            spreadsheets[idxTrain]->writeCell(i, 0, clusters[idxTrain][i]);
        }
        spreadsheets[idxTrain]->redraw();
        analyzer.clear();
        if (idxTrain < spreadsheets.size()-1) {
            idxTrain++;
            trainNextOutput();
        }
        else {
            kMeansTrained = true;
        }
    }
}

//-----------
void Analyze::draw() {
    if (!visible)  return;
    
    if (!gmmTrained) {
        if (spreadsheets.size() > 0) {
            spreadsheets[idxView]->draw(x+150, y);
        }
    }
    
    ofPushMatrix();
    ofTranslate(x+180, y);
    
    if (gmmTrained) {
        drawGMM(0);
    }
    
    ofPopMatrix();
    
}

//-----------
void Analyze::setGuiPosition(int x, int y) {
    this->x = x;
    this->y = y;
    control.setGuiPosition(x, y);
}

//-----------
void Analyze::setVisible(bool visible) {
    this->visible = visible;
    control.setVisible(visible);
}



void Analyze::drawGMM(int g) {
    vector<LearnInputParameter *> ainputs = outputs->at(g)->getActiveInputs();
    
    cout << "intputs size " << ainputs.size() << endl;
    
    ofPushMatrix();
    ofPushStyle();
    
    
    ofSetColor(0);
    ofFill();
    ofRect(0, 0, 195 + 105*gmmClusterSets[g].size(), 36+12*ainputs.size());
    
    for (int c=0; c<gmmClusterSets[g].size(); c++) {
        string name = gmmClusterSets[g][c].name;
        double prio = gmmClusterSets[g][c].prior;
        vector<double> mean = gmmClusterSets[g][c].mean;
        vector<double> std = gmmClusterSets[g][c].std;
        
        ofPushMatrix();
        ofTranslate(80 + 105*c, 0);
        
        ofSetColor(0);
        ofDrawBitmapString("gaussian "+ofToString(c), 2, 20);
        
        cout << "mean size " << mean.size() << endl;
        for (int p=0; p<mean.size(); p++) {
            
            cout << "mean " << mean[p] << " std " << std[p] <<"  min/max " << ainputs[p]->getMin() <<"/"<< ainputs[p]->getMax() << endl;
            
            float m = (mean[p] - ainputs[p]->getMin()) / (ainputs[p]->getMax() - ainputs[p]->getMin());
            float s = std[p] / (ainputs[p]->getMax() - ainputs[p]->getMin());
            
            ofPushMatrix();
            ofTranslate(2, 22+p*12);
            ofSetColor(0);
            ofRect(0, 0, 100, 11);
            
            ofSetColor(0, 255, 0);
            ofLine(100*(m-s), 0, 100*(m-s), 11);
            ofLine(100*(m+s), 0, 100*(m+s), 11);
            ofSetColor(255, 0, 0);
            ofLine(100*m, 0, 100*m, 11);
            
            ofPopMatrix();
        }
        
        ofPopMatrix();
    }
    
    for (int i=0; i<ainputs.size(); i++) {
        ofPushMatrix();
        ofPushStyle();
        ofTranslate(0, 30+i*12);
        ofSetColor(255);
        ofDrawBitmapString(ainputs[i]->getName(), 0, 0);
        ofPopStyle();
        ofPopMatrix();
    }
    
    
    
    ofPopStyle();
    ofPopMatrix();
}