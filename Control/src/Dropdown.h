#pragma once

#include "ofMain.h"
#include "ofxGui.h"


class Dropdown
{
public:
    template<class ListenerClass, typename ListenerMethod>
	void setup(string name, ofxPanel *gui, ListenerClass * listener, ListenerMethod method) {
        this->gui = gui;
        gui->add(label.setup(name, ""));
		value.addListener(listener, method);
	}
    
    void addItem(string name) {
        ofxToggle *tog = new ofxToggle();
        tog->setup(name, false);
        tog->addListener(this, &Dropdown::update);
        togs.push_back(tog);
        sels.push_back(false);
        choices.push_back(name);
        gui->add(tog);
        *togs[0] = true;
    }
    
    void addItems(vector<string> names) {
        for (int i=0; i<names.size(); i++) {
            addItem(names[i]);
        }
    }

    ~Dropdown() {
        for (int i=0; i<togs.size(); i++) {
            delete togs[i];
        }
    }

private:
    
    void update(bool & a) {
        for (int i=0; i<togs.size(); i++) {
            if (*togs[i] != sels[i]) {
                switchValue(i);
                return;
            }
        }
    }
    
    void switchValue(int idx) {
        sels[idx] = true;
        *togs[idx] = true;
        for (int i=0; i<choices.size(); i++) {
            if (i == idx)   continue;
            sels[i] = false;
            *togs[i] = false;
        }
        value = choices[idx];
    }
    
    ofxPanel *gui;
    ofxLabel label;
    vector<string> choices;
    vector<ofxToggle *> togs;
    vector<bool> sels;
    ofParameter<string> value;
};