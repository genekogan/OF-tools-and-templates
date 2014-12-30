#pragma once

#include "Control.h"


class MetaController;


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
    bool getVisible() {return visible;}
    
    vector<ofxUIDropDownList *> getMenus();
    vector<ParameterBase *> & getParameters() {return parameters;}
    ParameterBase * getParameter(string name);
    
    void clear();
    
    template <typename T>
    void addParameter(string name, T *value, T min, T max, float warp=1.0, bool invisible=false);
    
    template <typename T>
    void addParameter(string name, T *value, bool invisible=false);
    
    template <typename ListenerClass, typename ListenerMethod>
    void addEvent(string name, ListenerClass *listener, ListenerMethod method);
    
    template <typename ListenerClass, typename ListenerMethod>
    void addMenu(string name, vector<string> items, ListenerClass *listener, ListenerMethod method);
    
    void addColor(string name, ofColor *value);
    
    void addLabel(string name);
    void addSpacer();
    
    void triggerMenuEvent(string menuName, string selection, bool trigger);
    bool headerSelected;
    
    // meta controller
    MetaController * getMetaController() {return meta;}
    void setupMetaController();
    void removeMetaController();
    void setViewMeta(bool viewMeta);
    bool getMetaActive() {return metaActive;}
    
private:
    
    struct GuiElement {
        enum GuiType { GUI_PARAMETER, GUI_EVENT, GUI_MENU, GUI_LABEL, GUI_SPACER };
        int idxElement;
        string nameElement;
        GuiType type;
        GuiElement(GuiType type, int idxElement=0, string nameElement="") {
            this->type = type;
            this->idxElement = idxElement;
            this->nameElement = nameElement;
        }
    };
    
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
    
    
    void resetPresetsList();
    void addElementToGui(GuiElement *g);
    
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
    
    vector<GuiElement *> guiElements;
    vector<ParameterBase *> parameters;
    map<ParameterBase *, bool> parametersVisible;
    map<string, ofEvent<string>*> events;
    map<string, vector<string> > menus;
    map<string, ParameterBase*> stringEvents;
    map<string, ofEvent<string>*> menuEvents;
    map<string, GuiColorVecPair*> colors;
    
    MetaController *meta;
    bool metaActive;
    bool viewMeta;
};

template <typename T> void Control::addParameter(string name, T *value, T min, T max, float warp, bool invisible) {
    ParameterBase *parameter = new Parameter<T>(name, *value, min, max, warp);
    parameters.push_back(parameter);
    parametersVisible[parameter] = !invisible;
    GuiElement * guiElement = new GuiElement(GuiElement::GUI_PARAMETER, parameters.size()-1);
    guiElements.push_back(guiElement);
    if (!invisible) addElementToGui(guiElement);
}

template <typename T> void Control::addParameter(string name, T *value, bool invisible) {
    ParameterBase *parameter = new Parameter<T>(name, *value);
    parameters.push_back(parameter);
    parametersVisible[parameter] = !invisible;
    if (parameter->getType() == ParameterBase::STRING) {
        stringEvents[name] = parameter;
    }
    GuiElement * guiElement = new GuiElement(GuiElement::GUI_PARAMETER, parameters.size()-1);
    guiElements.push_back(guiElement);
    if (!invisible) addElementToGui(guiElement);
}

template <typename ListenerClass, typename ListenerMethod>
void Control::addEvent(string name, ListenerClass *listener, ListenerMethod method) {
    events[name] = new ofEvent<string>();
    ofAddListener(*events[name], listener, method);
    GuiElement * guiElement = new GuiElement(GuiElement::GUI_EVENT, 0, name);
    guiElements.push_back(guiElement);
    addElementToGui(guiElement);
}

template <typename ListenerClass, typename ListenerMethod>
void Control::addMenu(string name, vector<string> items, ListenerClass *listener, ListenerMethod method) {
    if (menus.count(name) == 0) {
        menus[name] = items;
        menuEvents[name] = new ofEvent<string>();
        ofAddListener(*menuEvents[name], listener, method);
        GuiElement * guiElement = new GuiElement(GuiElement::GUI_MENU, 0, name);
        guiElements.push_back(guiElement);
        addElementToGui(guiElement);
    }
    else {
        menus[name] = items;
        ((ofxUIDropDownList *) gui->getWidget(name))->clearToggles();
        ((ofxUIDropDownList *) gui->getWidget(name))->addToggles(items);

    }
}