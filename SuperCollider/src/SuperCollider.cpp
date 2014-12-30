#include "SuperCollider.h"


//-------
void SuperCollider::setup() {
    buses["bus1"] = new ofxSCBus();
    buses["bus2"] = new ofxSCBus();
    buses["bus3"] = new ofxSCBus();
    
    buffers["buf1"] = new ofxSCBuffer();

    control.setName("Sc3");
    

    string synthFile = "/Users/Gene/Code/openFrameworks/tools/SuperCollider/synths.scd";
    readFromFile("buffer", synthFile);
    readFromFile("event", synthFile);
    readFromFile("source", synthFile);
    readFromFile("modifier", synthFile);
}

//-------
void SuperCollider::readFromFile(string synthType, string synthFile) {
    SynthGroup *synthGroup = new SynthGroup(synthType, groupCount++);
    map<string, Synth*> *synths = new map<string, Synth*>();
    vector<string> synthNames;
    Synth *synth;
    
    ofxRegex regex;
    ofFile file;
    file.open(synthFile);
    string sc3file = file.readToBuffer().getText();
    string exp = ofToString("\n@synthDef "+synthType+"\n(.+\n)+.+/\n");
    vector<string> synthDefs = regex.getMatchedStrings(sc3file, exp);
    
    for (int i=0; i<synthDefs.size(); i++) {
        vector <string> statements = ofSplitString(synthDefs[i], "\n");
        for (int j=0; j<statements.size(); j++) {
            vector <string> statement = ofSplitString(statements[j], " ");
            if      (statement[0] == "@name") {
                synth = new Synth(statement[1], buses, buffers);
                (*synths)[statement[1]] = synth;
                synth->setVisible(false);
                synthNames.push_back(statement[1]);
            }
            else if (statement[0] == "@param") {
                synth->addParameter(statement[1], ofToFloat(statement[2]), ofToFloat(statement[3]));
            }
            else if (statement[0] == "@buffer") {
                //cout << "buf name " << statement[1] << endl;
            }
        }
    }

    synthGroup->setup(synths, synthNames, &control);
    groups[synthType] = synthGroup;
}

//-------
void SuperCollider::update() {
    map<string, SynthGroup*>::iterator it = groups.begin();
    while (it != groups.end()) {
        map<string, Synth*>::iterator itg = it->second->getSynths()->begin();
        while (itg != it->second->getSynths()->end()) {
            itg->second->update();
            ++itg;
        }
        ++it;
    }
}

//-------
void SuperCollider::free() {
    map<string, SynthGroup*>::iterator it = groups.begin();
    while (it != groups.end()) {
        map<string, Synth*>::iterator itg = it->second->getSynths()->begin();
        while (itg != it->second->getSynths()->end()) {
            itg->second->free();
            ++itg;
        }
        ++it;
    }
}
