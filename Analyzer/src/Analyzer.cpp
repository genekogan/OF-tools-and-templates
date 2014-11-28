#include "Analyzer.h"


//-----------
Analyzer::Analyzer() {
    control.setVisible(false);
}

//-----------
void Analyzer::setup(Learn *learn) {
    this->learn = learn;
    
    visible = true;
    collecting = false;

    
    control.setVisible(true);
    control.setName("Analyzer");
    control.addEvent("collect", this, &Analyzer::toggleCollecting);
    control.addEvent("train", this, &Analyzer::startTraining);
}

//-----------
void Analyzer::toggleCollecting(string &s) {
    collecting = !collecting;
    if (collecting && spreadsheets.size() == 0) {
        startCollecting();
    }
}

//-----------
void Analyzer::startCollecting() {
    
    vector<string> outputNames;
    vector<LearnOutputParameter *> *outputs = learn->getOutputs();
    for (int i=0; i<outputs->size(); i++) {
        outputNames.push_back(outputs->at(i)->getName());
        
        vector<LearnInputParameter *> inputs = outputs->at(i)->getActiveInputs();
        ofxSpreadsheet *spreadsheet = new ofxSpreadsheet();
        vector<string> headers;
        headers.push_back("cluster");
        for (int j=0; j<inputs.size(); j++) {
            headers.push_back(inputs[j]->getName());
        }
        spreadsheet->setup(600, 300);
        spreadsheet->setHeaders(headers);
        spreadsheets.push_back(spreadsheet);
        
        vector<int> newClusters;
        clusters.push_back(newClusters);
    }
    
    control.addMenu("outputs", outputNames, this, &Analyzer::selectOutput);
}

//-----------
void Analyzer::startTraining(string &s) {
    idxTrain = 0;
    training = true;
    trainNextOutput();
}

//-----------
void Analyzer::trainNextOutput() {
    analyzer.clearTrainingInstances();
    vector<vector<float> > entries = spreadsheets[idxTrain]->getEntries();
    for (int i=0; i<entries.size(); i++) {
        vector<double> instance = (vector<double>&) entries[i];
        analyzer.addTrainingInstance(instance);
    }
    cout << "START TRAINING! "<<idxTrain << endl;
    analyzer.beginTrainClusters(3);
}

//-----------
void Analyzer::update() {
    if (collecting) {
        
        vector<LearnOutputParameter *> *outputs = learn->getOutputs();
        for (int i=0; i<outputs->size(); i++) {
            vector<LearnInputParameter *> inputs = outputs->at(i)->getActiveInputs();

            vector<float> entry;
            entry.push_back(-1);
            for (int i=0; i<inputs.size(); i++) {
                entry.push_back(inputs[i]->get());
                //float normalizedValue = (inputs[i]->get() - inputs[i]->getMin()) / (inputs[i]->getMax() - inputs[i]->getMin());
                //entry.push_back(normalizedValue);
            }
            cout << "add " << i << " " << spreadsheets.size() << endl;
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
    }

}

//-----------
void Analyzer::draw() {

    
    if (!visible)  return;
    
    
    
    
    ofPushMatrix();

    
    //ofTranslate(x, y);
    

    if (spreadsheets.size() > 0) {
        spreadsheets[idxView]->draw(x+150, y);
    }
    
    

    
    ofPopMatrix();
    
    

    
    
    

}

//-----------
void Analyzer::setGuiPosition(int x, int y) {
    this->x = x;
    this->y = y;
    control.setGuiPosition(x, y);
}

//-----------
void Analyzer::setVisible(bool visible) {
    this->visible = visible;
    control.setVisible(visible);
}

