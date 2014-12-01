#include "Analyze.h"


//-----------
Analyze::Analyze() {
    control.setVisible(false);
    idxView = -1;
}

//-----------
void Analyze::setup() {
    setVisible(true);
    remapping = false;
    collecting = false;
    setupGui();
}

//-----------
void Analyze::setupGui() {
    control.setVisible(true);
    control.clear();
    control.setName("Analyze");
    control.addEvent("remap inputs", this, &Analyze::toggleRemapping);
    control.addParameter("remapping", &remapping);
    control.addSpacer();
    control.addEvent("collect", this, &Analyze::toggleCollecting);
    control.addParameter("collecting", &collecting);
    control.addSpacer();
    control.addEvent("train kmeans", this, &Analyze::startTraining);
    control.addEvent("train gmm", this, &Analyze::trainGMM);
    
    if (gmmTrained) {
        
    }
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
void Analyze::setInputMapping() {
    for (int i=0; i<inputs->size(); i++) {
        if (inputs->at(i)->get() > inputs->at(i)->getMax()) {
            inputs->at(i)->setMax(inputs->at(i)->get());
        }
        if (inputs->at(i)->get() < inputs->at(i)->getMin()) {
            inputs->at(i)->setMin(inputs->at(i)->get());
        }
    }
}

//-----------
void Analyze::selectOutput(string &s) {
    if (s=="_hide_") {
        gmmVisible = false;
        return;
    }
    gmmVisible = true;
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

//-----------
void Analyze::startCollecting() {
    vector<string> outputNames;
    outputNames.push_back("_hide_");
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
        spreadsheet->clear();
        spreadsheet->setHeaders(headers);
        spreadsheets.push_back(spreadsheet);
        
        vector<int> newClusters;
        clusters.push_back(newClusters);
    }
    idxView = 0;
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
            GMMCluster cluster(outputs->at(idx)->getName(),
                               gmm.getPrior(i), gmm.getMean(i), gmm.getStandardDeviation(i));
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
    
    if (gmmTrained && gmmVisible) {
        drawGMM(idxView);
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
    if (visible) {
        ofAddListener(ofEvents().mousePressed, this, &Analyze::mousePressed);
        ofAddListener(ofEvents().mouseDragged, this, &Analyze::mouseDragged);
        ofAddListener(ofEvents().mouseReleased, this, &Analyze::mouseReleased);
    }
    else {
        ofRemoveListener(ofEvents().mousePressed, this, &Analyze::mousePressed);
        ofRemoveListener(ofEvents().mouseDragged, this, &Analyze::mouseDragged);
        ofRemoveListener(ofEvents().mouseReleased, this, &Analyze::mouseReleased);
    }
}

//-----------
void Analyze::drawGMM(int g) {
    vector<LearnInputParameter *> ainputs = outputs->at(g)->getActiveInputs();
    
    ofPushMatrix();
    ofPushStyle();
    
    ofSetColor(100);
    ofFill();
    ofRect(0, 0, 170 + 105*gmmClusterSets[g].size(), 64+12*ainputs.size());

    ofSetColor(0, 255, 0);
    ofRect(5, 8, 100, 12);
    ofRect(5, 22, 100, 12);
    ofSetColor(255);
    ofDrawBitmapString("auto-train", 7, 19);
    ofDrawBitmapString("all", 7, 33);
    
    for (int c=0; c<gmmClusterSets[g].size(); c++) {
        string name = gmmClusterSets[g][c].name;
        double prior = gmmClusterSets[g][c].prior;
        vector<double> mean = gmmClusterSets[g][c].mean;
        vector<double> std = gmmClusterSets[g][c].std;
        
        ofPushMatrix();
        ofTranslate(130 + 105*c, 8);
        
        // assingment to output
        float aval = gmmClusterSets[idxView][c].assignedValue;
        float astd = gmmClusterSets[idxView][c].assignedStd;
        
        ofSetColor(0);
        ofRect(0, 0, 100, 11);
        ofSetColor(0, 255, 0);
        ofLine(100 * (aval-astd), 0, 100 * (aval-astd), 11);
        ofLine(100 * (aval+astd), 0, 100 * (aval+astd), 11);
        ofSetColor(255, 0, 0);
        ofLine(100 * aval, 0, 100 * aval, 11);
        ofSetColor(255);

        // gaussian cluster
        ofTranslate(0, 10);
        ofDrawBitmapString("gaussian "+ofToString(c), 2, 20);
        ofTranslate(0, 24);

        ofSetColor(0);
        ofRect(0, 0, 100, 11);
        ofSetColor(185);
        ofRect(0, 0, prior*100, 11);
        ofSetColor(255);
        ofDrawBitmapString("prior", 0, 10);
        
        ofTranslate(0, 16);
        
        for (int p=0; p<mean.size(); p++) {
            float m = (mean[p] - ainputs[p]->getMin()) / (ainputs[p]->getMax() - ainputs[p]->getMin());
            float s = std[p] / (ainputs[p]->getMax() - ainputs[p]->getMin());
            
            ofPushMatrix();
            ofTranslate(0, p*12);
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
        ofTranslate(0, 68+i*12);
        ofSetColor(255);
        ofDrawBitmapString(ainputs[i]->getName(), 2, 0);
        ofPopStyle();
        ofPopMatrix();
    }
    
    ofPopStyle();
    ofPopMatrix();
}

//-----------
void Analyze::mousePressed(ofMouseEventArgs &e) {
    if (!gmmTrained || idxView==-1) return;
    int numClusters = gmmClusterSets[idxView].size();
    for (int c=0; c<numClusters; c++) {
        ofRectangle rect = ofRectangle(x + 180 + 130 + 105*c, y + 8, 100, 11);
        if (rect.inside(e.x, e.y)) {
            idxDraggingCluster = c;
            dragging = true;
            return;
        }
    }
    ofRectangle rect1 = ofRectangle(x + 180 + 5, y + 8, 100, 11);
    ofRectangle rect2 = ofRectangle(x + 180 + 5, y + 22, 100, 11);
    if (rect1.inside(e.x, e.y)) {
        makeTrainingSetFromGMM(idxView);
    }
    else if (rect2.inside(e.x, e.y)) {
        makeTrainingSetFromGMMAll();
    }
}

//-----------
void Analyze::mouseDragged(ofMouseEventArgs &e) {
    if (dragging) {
        ofRectangle rect = ofRectangle(x + 180 + 130 + 105*idxDraggingCluster, y + 8, 100, 11);
        if (rect.inside(e.x, e.y)) {
            if (ofGetKeyPressed()) {
                gmmClusterSets[idxView][idxDraggingCluster].assignedStd += 0.01 * (e.x - ofGetPreviousMouseX());
                float maxMargin = min(gmmClusterSets[idxView][idxDraggingCluster].assignedValue, 1.0-gmmClusterSets[idxView][idxDraggingCluster].assignedValue);
                gmmClusterSets[idxView][idxDraggingCluster].assignedStd = ofClamp(gmmClusterSets[idxView][idxDraggingCluster].assignedStd, 0, maxMargin);
            }
            else {
                double newValue = (double) (e.x - (x + 180 + 130 + 105*idxDraggingCluster)) / 100.0;
                gmmClusterSets[idxView][idxDraggingCluster].assignedValue = newValue;
            }
        }
    }
}

//-----------
void Analyze::mouseReleased(ofMouseEventArgs &e){
    
}
