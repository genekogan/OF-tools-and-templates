#include "SuperCollider.h"


//-------
SuperCollider::SuperCollider() {

}

//-------
void SuperCollider::setup() {
    cout << "setup sc3 " << endl;
    
    control.addParameter("param", &a, 0.0f, 1.0f);
    
    getsc3();
}

//-------
void SuperCollider::getsc3() {
    
    //vector<Instrument *> mods;
    string synthFile = "/Users/Gene/Code/openFrameworks/tools/SuperCollider/synths.scd";
    string synthType = "source";
    
    
    ofxRegex regex;
    ofFile file;
    file.open(synthFile);
    
    string sc3file = file.readToBuffer().getText();
    string exp = ofToString("\n@synthDef "+synthType+"\n(.+\n)+.+/\n");
    vector<string> synthDefs = regex.getMatchedStrings(sc3file, exp);
    
    for (int i=0; i<1; i++) {//synthDefs.size(); i++) {
        Instrument *mod = new Instrument();
        mods.push_back(mod);
        vector <string> statements = ofSplitString(synthDefs[i], "\n");
        for (int j=0; j<statements.size(); j++) {
            vector <string> statement = ofSplitString(statements[j], " ");
            if      (statement[0] == "@name") {
                cout << "get name " << statement[1] << endl;
                mod->setup(statement[1]);
            }
            else if (statement[0] == "@param") {
                cout << "get param " << statement[1] << " " << ofToFloat(statement[2]) << " " <<  ofToFloat(statement[3]) << endl;
                mod->addParameter(statement[1], ofToFloat(statement[2]), ofToFloat(statement[3]));
            }
            else if (statement[0] == "@buffer") {
                //cout << "buf name " << statement[1] << endl;
                //string bufName = statement[1];  // does nothing for now
            }
        }
    }
    //return mods;
    
}
