#pragma once

#include "Control.h"


class MetaController;


// meta-controller
/*
 - control button to activate/display sequencer
 - bind variables to rows
 */



class Control
{
public:
    ~Control();
    Control();
    
    void update(ofEventArgs &data);
    void draw(ofEventArgs &data);
    
    void savePreset();
    void loadPreset(string path);
    
    void setName(string name);
    string getName() {return name;}
    
    void setWidth(int width);
    void setGuiPosition(int x, int y);
    void setActive(bool active);
    void setVisible(bool visible);
    void toggleVisible();
    void togglePresetsVisible();
    
    vector<ofxUIDropDownList *> getMenus();
    vector<ParameterBase *> & getParameters() {return parameters;}
    void clear();
    
    template <typename T>
    void addParameter(string name, T *value, T min, T max, bool invisible=false);
    
    template <typename T>
    void addParameter(string name, T *value, bool invisible=false);
    
    template <typename ListenerClass, typename ListenerMethod>
    void addEvent(string name, ListenerClass *listener, ListenerMethod method);
    
    template <typename ListenerClass, typename ListenerMethod>
    void addMenu(string name, vector<string> items, ListenerClass *listener, ListenerMethod method);
    
    void addColor(string name, ofColor *value);
    
    void triggerMenuEvent(string menuName, string selection, bool trigger);
    bool headerSelected;
    
    /* meta controller */
    void setupMetaController();
    bool metaActive;
    
private:
    
    struct GuiColorVecPair {
        ofColor *color;
        ofVec4f *vec;
        void update() {
            color->set(vec->x, vec->y, vec->z, vec->w);
        }
    };
    
    void guiEvent(ofxUIEventArgs &e);
    void guiPresetsEvent(ofxUIEventArgs &e);
    void setupGui();
    void setupGuiPresets();
    void updateColors();
    
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
    bool active, visible;
    int width, spacing;
    int numLerpFrames;
    
    vector<ParameterBase *> parameters;
    map<ParameterBase *, bool> parametersVisible;
    map<string, ofEvent<string>*> events;
    map<string, vector<string> > menus;
    map<string, ofEvent<string>*> menuEvents;
    map<string, GuiColorVecPair*> colors;
    
    
    MetaController *meta;
};


template <typename T> void Control::addParameter(string name, T *value, T min, T max, bool invisible) {
    ParameterBase *parameter = new Parameter<T>(name, *value, min, max);
    parameters.push_back(parameter);
    parametersVisible[parameter] = !invisible;
    setupGui();
}

template <typename T> void Control::addParameter(string name, T *value, bool invisible) {
    ParameterBase *parameter = new Parameter<T>(name, *value);
    parameters.push_back(parameter);
    parametersVisible[parameter] = !invisible;
    setupGui();
}

template <typename ListenerClass, typename ListenerMethod>
void Control::addEvent(string name, ListenerClass *listener, ListenerMethod method) {
    events[name] = new ofEvent<string>();
    ofAddListener(*events[name], listener, method);
    setupGui();
}

template <typename ListenerClass, typename ListenerMethod>
void Control::addMenu(string name, vector<string> items, ListenerClass *listener, ListenerMethod method) {
    if (menus.count(name) == 0) {
        menus[name] = items;
        menuEvents[name] = new ofEvent<string>();
        ofAddListener(*menuEvents[name], listener, method);
        setupGui();
    }
    else {
        menus[name] = items;
        setupGui();
    }
}
