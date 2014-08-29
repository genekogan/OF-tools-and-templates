#pragma once

#include "Parameter.h"
#include "Dropdown.h"
#include "ofxGui.h"


class Control
{
public:
    Control() {
        ofAddListener(ofEvents().draw, this, &Control::draw);
        visible = true;
        gui.setup(name);
        guiPresets.setup(name+" presets");
        setupGui();
        guiType = GUI_PARAMETERS;
    }
    
    ~Control() {
        ofRemoveListener(ofEvents().draw, this, &Control::draw);
    }
    
    string getName() {
        return name;
    }
    
    void setName(string name) {
        this->name = name;
        gui.setName(name);
        guiPresets.setName(name+" presets");
    }

    void clearParameters() {
        gui.clear();
        guiPresets.clear();
    }

    void savePreset() {
        string namePreset = ofSystemTextBoxDialog("preset name?");
        gui.saveToFile(ofToString("presets/"+name+"/"+namePreset+".xml"));
    }
    
    void loadPreset(string &name) {
        gui.loadFromFile("presets/"+name);
    }
    
    template <typename T>
    void registerParameter(string name, ofxParameter<T> *parameter, T min, T max) {
        //parameter->set(name, (min+max)*0.5, min, max);
        parameter->setName(name);
        parameter->setMin(min);
        parameter->setMax(max);
        gui.add(*parameter);
    }
    
    template <typename T>
    void registerParameter(string name, ofxParameter<T> *parameter) {
        parameter->set(name, *parameter);
        gui.add(*parameter);
    }

    template <typename L, typename M>
    void registerMenu(string name, L listenerClass, M listenerMethod, vector<string> choices) {
        Dropdown *menu = new Dropdown();
        menu->setup(name, &gui, listenerClass, listenerMethod);
        menu->addItems(choices);
    }
    
    template <typename L, typename M>
    void registerEvent(string name, L listenerClass, M listenerMethod) {
        ofxButton *button = new ofxButton();
        button->setup(name);
        button->addListener(listenerClass, listenerMethod);
        gui.add(button);
    }
    
    void registerLabel(string name) {
        ofxLabel *label = new ofxLabel();
        label->setup(name);
        gui.add(label);
    }

    void setGuiPosition(int x, int y) {
        gui.setPosition(x, y);
        guiPresets.setPosition(x, y);
    }

    bool getVisible() {
        return visible;
    }
    
    void setVisible(bool b) {
        visible = b;
    }
    
    void toggleVisible() {
        visible = !visible;
    }
    
    void refreshGui() {
        clearParameters();
        setVisible(true);
        setupGui();
    }

private:
    
    enum GuiType { GUI_PARAMETERS, GUI_PRESETS };
    
    void setupGui() {
        ofxButton *switchGui1 = new ofxButton();
        ofxButton *switchGui2 = new ofxButton();
        switchGui1->setup("view presets");
        switchGui2->setup("view parameters");
        switchGui1->addListener(this, &Control::toggleGui);
        switchGui2->addListener(this, &Control::toggleGui);
        
        ofxButton *saveButton = new ofxButton();
        saveButton->setup("save preset");
        saveButton->addListener(this, &Control::savePreset);
        
        gui.add(switchGui1);
        guiPresets.add(switchGui2);
        guiPresets.add(saveButton);
        cout <<ofToDataPath("presets/"+name+"/") << endl;
        ofDirectory dir(ofToDataPath("presets/"+name+"/"));
        dir.allowExt("xml");
        dir.listDir();
        vector<string> filenames;
        for(int i = 0; i < dir.numFiles(); i++) {
            filenames.push_back(dir.getName(i));
        }

        Dropdown *menu = new Dropdown();
        menu->setup(name, &guiPresets, this, &Control::loadPreset);
        menu->addItems(filenames);
    }
    
    void toggleGui() {
        guiType = guiType==GUI_PARAMETERS ? GUI_PRESETS : GUI_PARAMETERS;
    }
    
    void draw(ofEventArgs &data) {
        if (visible) {
            if (guiType == GUI_PARAMETERS) {
                gui.draw();
            }
            else if (guiType == GUI_PRESETS) {
                guiPresets.draw();
            }
        }
    }
    
    ofxPanel gui;
    ofxPanel guiPresets;
    string name;
    bool visible;
    GuiType guiType;
};

