#include "Presets.h"

void Presets::load(string path) {
    cout << "load" << endl;
}
/*
void Presets::generateTouchOscLayout(vector<OutputParameter *> &parameters) {
    vector<OutputParameter *> oscParameters;
    for (int i=0; i<parameters.size(); i++) {
        if (parameters[i]->getType() == OSC) {
            oscParameters.push_back(parameters[i]);
        }
    }
    int numParameters = oscParameters.size();
    
    ofxTouchOsc touchOsc;
    touchOsc.setScale(320, 480);
    touchOsc.setDefaultColor(GREEN);
    ofxTouchOscPage *page = touchOsc.addPage("parameters");

    float h = 0.98 / numParameters;
    float y = 0.0;
    for (int i=0; i<numParameters; i++) {
        ofxTouchOscFader *fader = page->addFader(oscParameters[i]->getName(), 0.01, y, 0.98, h*0.85);
        fader->setOscAddress(oscParameters[i]->getAddress());
        y += h;
    }
    
    touchOsc.save("myTouchOscLayout");
}
*/