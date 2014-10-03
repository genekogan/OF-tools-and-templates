#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "Parameter.h"
#include "OscManager.h"
#include "Presets.h"


class Control
{
public:
    ~Control();
    
    Control() {
        gui = new ofxUICanvas("control");
        guiPresets = new ofxUICanvas("controlPresets");
        setWidth(150);
        spacing = gui->getWidgetSpacing();
        headerSelected = false;
        setupGuiPresets();
        visible = true;
        setActive(true);
    }
    
    void update(ofEventArgs &data) {
        updateColors();
    }
    
    void savePreset();
    
    void loadPreset(string path);
    
    void setName(string name) {
        this->name = name;
        setupGuiPresets();
    }
    
    void setWidth(int width) {
        this->width = width;
        gui->setWidth(width);
        guiPresets->setWidth(width);
    }

    void setGuiPosition(int x, int y) {
        gui->setPosition(x, y);
        guiPresets->setPosition(x, y);
    }
    
    void setActive(bool active) {
        if (active) {
            ofAddListener(gui->newGUIEvent, this, &Control::guiEvent);
            ofAddListener(guiPresets->newGUIEvent, this, &Control::guiPresetsEvent);
            ofAddListener(ofEvents().update, this, &Control::update);
        }
        else {
            ofRemoveListener(gui->newGUIEvent, this, &Control::guiEvent);
            ofRemoveListener(guiPresets->newGUIEvent, this, &Control::guiPresetsEvent);
            ofRemoveListener(ofEvents().update, this, &Control::update);
        }
        setVisible(active);
    }
    
    void setVisible(bool visible) {
        this->visible = visible;
        if (visible) {
            gui->enable();
            guiPresets->enable();
        }
        else {
            gui->disable();
            guiPresets->disable();
        }
        gui->setVisible(visible);
        guiPresets->setVisible(false);
    }
    
    void toggleVisible() {
        setVisible(!visible);
    }
    
    void togglePresetsVisible() {
        visible = !visible;
        gui->setVisible(visible);
        guiPresets->setVisible(!visible);
    }
    
    vector<ParameterBase *> & getParameters() {
        return parameters;
    }
    
    void clearParameters();

    template <typename T> void addParameter(string name, T *value, T min, T max) {
        ParameterBase *parameter = new Parameter<T>(name, *value, min, max);
        parameters.push_back(parameter);
        setupGui();
    }
    
    template <typename T> void addParameter(string name, T *value) {
        ParameterBase *parameter = new Parameter<T>(name, *value);
        parameters.push_back(parameter);
        setupGui();
    }
    
    template <typename ListenerClass, typename ListenerMethod>
    void addEvent(string name, ListenerClass *listener, ListenerMethod method) {
        events[name] = new ofEvent<string>();
        ofAddListener(*events[name], listener, method);
        setupGui();
    }
    
    template <typename ListenerClass, typename ListenerMethod>
    void addMenu(string name, vector<string> items, ListenerClass *listener, ListenerMethod method) {
        menus[name] = items;
        menuEvents[name] = new ofEvent<string>();
        ofAddListener(*menuEvents[name], listener, method);
        setupGui();
    }
    
    void addColor(string name, ofColor *value) {
        string sn = ofToString(ofRandom(1000));
        ofVec4f *vec = new ofVec4f(value->r, value->g, value->b, value->a);
        GuiColorVecPair *color = new GuiColorVecPair();
        color->color = value;
        color->vec = vec;
        colors[name] = color;
        ParameterBase *parameter = new Parameter<ofVec4f>(name, *vec, ofVec4f(0, 0, 0, 0), ofVec4f(255, 255, 255, 255));
        parameters.push_back(parameter);
        setupGui();
    }
    
    string getName() {return name;}

    bool headerSelected;
    
private:
    
    struct GuiColorVecPair {
        ofColor *color;
        ofVec4f *vec;
        void update() {
            color->set(vec->x, vec->y, vec->z, vec->w);
        }
    };
    
    void updateColors();
    
    void guiEvent(ofxUIEventArgs &e);
    void guiPresetsEvent(ofxUIEventArgs &e);
    void setupGui();
    void setupGuiPresets();
    
    void addParameterToGui(Parameter<bool> *parameter);
    void addParameterToGui(Parameter<int> *parameter);
    void addParameterToGui(Parameter<float> *parameter);
    void addParameterToGui(Parameter<ofVec2f> *parameter);
    void addParameterToGui(Parameter<ofVec3f> *parameter);
    void addParameterToGui(Parameter<ofVec4f> *parameter);
    void addParameterToGui(Parameter<string> *parameter);
    
    ofxUICanvas *gui, *guiPresets;
    ofxUIDropDownList *presetNames;

    string name;
    bool visible;
    int width, spacing;
    int numLerpFrames;
    
    vector<ParameterBase *> parameters;
    map<string, ofEvent<string>*> events;
    map<string, vector<string> > menus;
    map<string, ofEvent<string>*> menuEvents;
    map<string, GuiColorVecPair*> colors;
};
