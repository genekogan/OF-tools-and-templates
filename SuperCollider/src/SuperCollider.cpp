#include "SuperCollider.h"



//-------
SuperColliderLayer * SuperCollider::addLayer(string synthType, string synthFile) {
    SuperColliderLayer *newLayer = new SuperColliderLayer();
    newLayer->setup(synthType, synthFile);
    
    layers.push_back(newLayer);
    setGuiPosition(5, 5);
    return newLayer;
}

//-------
void SuperCollider::update() {
    for (int i=0; i<layers.size(); i++) {
        layers[i]->update();
    }
}

//-------
void SuperColliderLayer::setup(string synthType, string synthFile) {
    ofxRegex regex;
    ofFile file;
    file.open(synthFile);
    string sc3file = file.readToBuffer().getText();
    string exp = ofToString("\n@synthDef "+synthType+"\n(.+\n)+.+/\n");
    vector<string> synthDefs = regex.getMatchedStrings(sc3file, exp);
    
    for (int i=0; i<synthDefs.size(); i++) {
        Instrument *instrument = new Instrument();
        instruments.push_back(instrument);
        vector <string> statements = ofSplitString(synthDefs[i], "\n");
        for (int j=0; j<statements.size(); j++) {
            vector <string> statement = ofSplitString(statements[j], " ");
            if      (statement[0] == "@name") {
                instrument->setup(statement[1]);
            }
            else if (statement[0] == "@param") {
                instrument->addParameter(statement[1], ofToFloat(statement[2]), ofToFloat(statement[3]));
            }
            else if (statement[0] == "@buffer") {
                //cout << "buf name " << statement[1] << endl;
            }
        }
    }
    
    vector<string> synthNames;
    for (int i=0; i<instruments.size(); i++) {
        synthNames.push_back(instruments[i]->getName());
        instruments[i]->setVisible(false);
        instruments[i]->setGuiPosition(300, 5);
    }
    
    
    
    bus = new ofxSCBus();
    //for (int i=0; i<instruments.size(); i++) {
        //instruments[i]->setBusOut(bus);
        //instruments[i]->setBusOutToDac();
    //}
    
    
    control.addMenu("synths", synthNames, this, &SuperColliderLayer::guiEvent);
}

//-------
void SuperColliderLayer::guiEvent(string &s) {
    for (int i=0; i<instruments.size(); i++) {
        if (instruments[i]->getName() == s) {
            instruments[i]->setVisible(true);
        }
        else {
            instruments[i]->setVisible(false);
        }
    }
}

//-------
void SuperColliderLayer::update() {
    for (int i=0; i<instruments.size(); i++) {
        instruments[i]->update();
    }
}
