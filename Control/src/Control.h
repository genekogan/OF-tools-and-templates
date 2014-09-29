#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "Parameter.h"
#include "OscManager.h"


class Control
{
public:
    ~Control();
    
    Control() {
        gui = new ofxUICanvas("control");
        setWidth(150);
        spacing = gui->getWidgetSpacing();
        ofAddListener(gui->newGUIEvent, this, &Control::guiEvent);
        headerSelected = false;
    }
    
    void setName(string name) {
        this->name = name;
    }
    
    void setWidth(int width) {
        this->width = width;
        gui->setWidth(width);
    }

    void setGuiPosition(int x, int y) {
        gui->setPosition(x, y);
    }
    
    void setVisible(bool visible) {
        this->visible = visible;
        if (visible)    gui->enable();
        else            gui->disable();
        gui->setVisible(visible);
    }
    
    void toggleVisible() {
        setVisible(!visible);
    }
    
    void clearParameters();

    vector<ParameterBase *> & getParameters() {
        return parameters;
    }
    
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
        void update() {color->set(vec->x, vec->y, vec->z, vec->w);}
    };
    
    void guiEvent(ofxUIEventArgs &e);
    void setupGui();
    
    void addParameterToGui(string name, bool *t);
    void addParameterToGui(string name, string *t);
    void addParameterToGui(string name, int min, int max, int *value);
    void addParameterToGui(string name, float min, float max, float *value);
    void addParameterToGui(string name, ofVec2f min, ofVec2f max, ofVec2f *value);
    void addParameterToGui(string name, ofVec3f min, ofVec3f max, ofVec3f *value);
    void addParameterToGui(string name, ofVec4f min, ofVec4f max, ofVec4f *value);
    
    ofxUICanvas *gui;
    string name;
    bool visible;
    int width, spacing;
    
    vector<ParameterBase *> parameters;
    map<string, ofEvent<string>*> events;
    map<string, vector<string> > menus;
    map<string, ofEvent<string>*> menuEvents;
    map<string, GuiColorVecPair*> colors;
};
