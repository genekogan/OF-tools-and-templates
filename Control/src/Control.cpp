#include "Control.h"


//-------
void Control::setupGui() {
    gui->clearWidgets();
    gui->addLabel(name);
    gui->addSpacer();
    
    /* add parameters */
    for (int i=0; i<parameters.size(); i++) {
        ParameterBase::Type type = parameters[i]->getType();
        if      (type == ParameterBase::BOOL) {
            addParameterToGui(parameters[i]->getName(),
                              parameters[i]->getReference<bool>());
        }
        else if (type == ParameterBase::STRING) {
            addParameterToGui(parameters[i]->getName(),
                              parameters[i]->getReference<string>());
        }
        else if (type == ParameterBase::INT) {
            addParameterToGui(parameters[i]->getName(),
                              parameters[i]->getMin<int>(),
                              parameters[i]->getMax<int>(),
                              parameters[i]->getReference<int>());
        }
        else if (type == ParameterBase::FLOAT) {
            addParameterToGui(parameters[i]->getName(),
                              parameters[i]->getMin<float>(),
                              parameters[i]->getMax<float>(),
                              parameters[i]->getReference<float>());
        }
        else if (type == ParameterBase::VEC2F) {
            addParameterToGui(parameters[i]->getName(),
                              parameters[i]->getMin<ofVec2f>(),
                              parameters[i]->getMax<ofVec2f>(),
                              parameters[i]->getReference<ofVec2f>());
        }
        else if (type == ParameterBase::VEC3F) {
            addParameterToGui(parameters[i]->getName(),
                              parameters[i]->getMin<ofVec3f>(),
                              parameters[i]->getMax<ofVec3f>(),
                              parameters[i]->getReference<ofVec3f>());
        }
        else if (type == ParameterBase::VEC4F) {
            addParameterToGui(parameters[i]->getName(),
                              parameters[i]->getMin<ofVec4f>(),
                              parameters[i]->getMax<ofVec4f>(),
                              parameters[i]->getReference<ofVec4f>());
        }
    }
    
    /* add menus */
    for (map<string,vector<string> >::iterator it=menus.begin(); it!=menus.end(); ++it){
        gui->addDropDownList(it->first, it->second);
    }
    
    /* add events */
    for (map<string,ofEvent<bool>*>::iterator it=events.begin(); it!=events.end(); ++it){
        gui->addButton(it->first, false);
    }

    /* set color of color sliders */
    for (map<string,GuiColorVecPair*>::iterator it=colors.begin(); it!=colors.end(); ++it){
        it->second->update();
        gui->getWidget(it->first+"->x")->setColorFill(*colors[it->first]->color);
        gui->getWidget(it->first+"->y")->setColorFill(*colors[it->first]->color);
        gui->getWidget(it->first+"->z")->setColorFill(*colors[it->first]->color);
    }

    gui->autoSizeToFitWidgets();
}

//-------
void Control::guiEvent(ofxUIEventArgs &e) {
    /* menu select notification */
    if (!menus[e.getParentName()].empty()) {
        string selection = e.getName();
        ofNotifyEvent(*menuEvents[e.getParentName()], selection, this);
    }
    
    /* event notification */
    else if (events[e.getName()] != 0) {
        bool state = e.getButton()->getValue();
        ofNotifyEvent(*events[e.getName()], state, this);
    }
    
    /* color change */
    else {
        string colorName = ofSplitString(e.getName(), "->")[0];
        if (colors[colorName]) {
            colors[colorName]->update();
            gui->getWidget(colorName+"->x")->setColorFill(*colors[colorName]->color);
            gui->getWidget(colorName+"->y")->setColorFill(*colors[colorName]->color);
            gui->getWidget(colorName+"->z")->setColorFill(*colors[colorName]->color);
            gui->getWidget(colorName+"->w")->setColorFill(ofColor(colors[colorName]->color->a));
            gui->getWidget(colorName+"->x")->setColorFillHighlight(*colors[colorName]->color);
            gui->getWidget(colorName+"->y")->setColorFillHighlight(*colors[colorName]->color);
            gui->getWidget(colorName+"->z")->setColorFillHighlight(*colors[colorName]->color);
            gui->getWidget(colorName+"->w")->setColorFillHighlight(ofColor(colors[colorName]->color->a));
        }
    }
}

//-------
void Control::addParameterToGui(string name, bool *t) {
    gui->addToggle(name, t);
}

//-------
void Control::addParameterToGui(string name, int min, int max, int *t) {
    gui->addMinimalIntSlider(name, min, max, t);
}

//-------
void Control::addParameterToGui(string name, float min, float max, float *t) {
    gui->addMinimalSlider(name, min, max, t);
}

//-------
void Control::addParameterToGui(string name, ofVec2f min, ofVec2f max, ofVec2f *t) {
    gui->addSpacer();
    gui->addMinimalSlider(name+"->x", min.x, max.x, &t->x);
    gui->addMinimalSlider(name+"->y", min.y, max.y, &t->y);
    gui->addSpacer();
}

//-------
void Control::addParameterToGui(string name, ofVec3f min, ofVec3f max, ofVec3f *t) {
    gui->addSpacer();
    gui->addMinimalSlider(name+"->x", min.x, max.x, &t->x);
    gui->addMinimalSlider(name+"->y", min.y, max.y, &t->y);
    gui->addMinimalSlider(name+"->z", min.z, max.z, &t->z);
    gui->addSpacer();
}

//-------
void Control::addParameterToGui(string name, ofVec4f min, ofVec4f max, ofVec4f *t) {
    gui->addSpacer();
    gui->addMinimalSlider(name+"->x", min.x, max.x, &t->x);
    gui->addMinimalSlider(name+"->y", min.y, max.y, &t->y);
    gui->addMinimalSlider(name+"->z", min.z, max.z, &t->z);
    gui->addMinimalSlider(name+"->w", min.w, max.w, &t->w);
    gui->addSpacer();
}

//-------
void Control::addParameterToGui(string name, string *t) {
    gui->addTextInput(name, *t);
}

//-------
void Control::clearParameters() {
    for (map<string, GuiColorVecPair*>::iterator it=colors.begin(); it!=colors.end(); ++it){
        delete it->second;
    }
    for (map<string, ofEvent<string>*>::iterator it=menuEvents.begin(); it!=menuEvents.end(); ++it){
        delete it->second;
    }
    for (map<string, ofEvent<bool>*>::iterator it=events.begin(); it!=events.end(); ++it){
        delete it->second;
    }
    for (int i=0; i<parameters.size(); i++) {
        delete parameters[i];
    }
    colors.clear();
    menuEvents.clear();
    menus.clear();
    events.clear();
    parameters.clear();
}

//-------
Control::~Control() {
    ofRemoveListener(gui->newGUIEvent, this, &Control::guiEvent);
    clearParameters();
    delete gui;
}
