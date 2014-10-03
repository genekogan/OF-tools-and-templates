#include "Control.h"


//-------
void Control::savePreset() {
    Presets presets;
    bool saved = presets.savePreset(getName(), parameters);
    if (!saved) return;
    setupGuiPresets();
}

//-------
void Control::loadPreset(string path) {
    Presets presets;
    presets.loadPreset(path, parameters, numLerpFrames);
    updateColors();
}

//-------
void Control::setupGuiPresets() {
    guiPresets->clearWidgets();
    guiPresets->setWidth(width);
    guiPresets->addLabelButton("controlHeader", false, (float)width-42.0f)->setLabelText("Presets");
    guiPresets->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiPresets->addLabelButton("P", false, 16.0f);
    ((ofxUILabelButton *) guiPresets->getWidget("P"))->setName("controlPresetsView");
    guiPresets->addLabelButton("S", false, 16.0f);
    ((ofxUILabelButton *) guiPresets->getWidget("S"))->setName("controlPresetsSave");
    guiPresets->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    guiPresets->addSpacer();
    guiPresets->addIntSlider("Lerp frames", 0, 120, &numLerpFrames);
    ofDirectory dir(ofToDataPath("presets/"+getName()+"/"));
    dir.allowExt("xml");
    dir.listDir();
    vector<string> filenames;
    for(int i = 0; i < dir.numFiles(); i++) {
        filenames.push_back(dir.getName(i));
    }
    presetNames = guiPresets->addDropDownList("presets", filenames);
    presetNames->setAutoClose(false);
    presetNames->open();
    guiPresets->autoSizeToFitWidgets();
    guiPresets->setVisible(!visible);
}

//-------
void Control::setupGui() {
    gui->clearWidgets();
    gui->setWidth(width);
    gui->addLabelButton("controlHeader", false, (float)width-42.0f)->setLabelText(name);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addLabelButton("P", false, 16.0f);
    ((ofxUILabelButton *) gui->getWidget("P"))->setName("controlPresetsView");
    gui->addLabelButton("S", false, 16.0f);
    ((ofxUILabelButton *) gui->getWidget("S"))->setName("controlPresetsSave");
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    gui->addSpacer();
    
    /* add parameters */
    for (int i=0; i<parameters.size(); i++) {
        ParameterBase::Type type = parameters[i]->getType();
        if      (type == ParameterBase::BOOL) {
            addParameterToGui((Parameter<bool> *) parameters[i]);
        }
        else if (type == ParameterBase::STRING) {
            addParameterToGui((Parameter<string> *) parameters[i]);
        }
        else if (type == ParameterBase::INT) {
            addParameterToGui((Parameter<int> *) parameters[i]);
        }
        else if (type == ParameterBase::FLOAT) {
            addParameterToGui((Parameter<float> *) parameters[i]);
        }
        else if (type == ParameterBase::VEC2F) {
            addParameterToGui((Parameter<ofVec2f> *) parameters[i]);
        }
        else if (type == ParameterBase::VEC3F) {
            addParameterToGui((Parameter<ofVec3f> *) parameters[i]);
        }
        else if (type == ParameterBase::VEC4F) {
            addParameterToGui((Parameter<ofVec4f> *) parameters[i]);
        }
    }
    
    /* add menus */
    for (map<string,vector<string> >::iterator it=menus.begin(); it!=menus.end(); ++it){
        ofxUIDropDownList *menu = gui->addDropDownList(it->first, it->second);
        menu->setAutoClose(false);
        //menu->setAllowMultiple(true);
        menu->open();
        menu->setPadding(1);
    }
    
    /* add events */
    for (map<string,ofEvent<string>*>::iterator it=events.begin(); it!=events.end(); ++it){
        gui->addButton(it->first, false);
    }
    
    /* set color of color sliders */
    int idx = ofRandom(1000);
    for (map<string,GuiColorVecPair*>::iterator it=colors.begin(); it!=colors.end(); ++it){
        it->second->update();
        gui->getWidget(it->first+"->x")->setColorFill(*colors[it->first]->color);
        gui->getWidget(it->first+"->y")->setColorFill(*colors[it->first]->color);
        gui->getWidget(it->first+"->z")->setColorFill(*colors[it->first]->color);
        gui->getWidget(it->first+"->w")->setColorFill(*colors[it->first]->color);
    }
    gui->autoSizeToFitWidgets();
}

//-------
void Control::guiEvent(ofxUIEventArgs &e) {
    
    // menu select notification
    if (!menus[e.getParentName()].empty()) {
        string selection = e.getName();
        ofNotifyEvent(*menuEvents[e.getParentName()], selection, this);
    }
    
    // event notification
    else if (events[e.getName()] != 0) {
        if (e.getButton()->getValue() == 1) return;
        string state = e.getName();
        ofNotifyEvent(*events[e.getName()], state, this);
    }
    
    // label callback
    else if (e.getName() == "controlHeader") {
        if (e.getButton()->getValue() == 1) return;
        headerSelected = true;
    }
    
    // load presets
    else if (e.getName() == "controlPresetsView") {
        if (e.getButton()->getValue() == 1) return;
        togglePresetsVisible();
    }

    // save preset
    else if (e.getName() == "controlPresetsSave") {
        if (e.getButton()->getValue() == 1) return;
        savePreset();
    }

    // color change
    else {
        string colorName = ofSplitString(e.getName(), "->")[0];
        if (colors.count(colorName) > 0) {
            colors[colorName]->update();
            gui->getWidget(colorName+"->x")->setColorFill(*colors[colorName]->color);
            gui->getWidget(colorName+"->y")->setColorFill(*colors[colorName]->color);
            gui->getWidget(colorName+"->z")->setColorFill(*colors[colorName]->color);
            gui->getWidget(colorName+"->w")->setColorFill(*colors[colorName]->color);
            gui->getWidget(colorName+"->x")->setColorFillHighlight(*colors[colorName]->color);
            gui->getWidget(colorName+"->y")->setColorFillHighlight(*colors[colorName]->color);
            gui->getWidget(colorName+"->z")->setColorFillHighlight(*colors[colorName]->color);
            gui->getWidget(colorName+"->w")->setColorFillHighlight(*colors[colorName]->color);
        }
    }

}

//-------
void Control::guiPresetsEvent(ofxUIEventArgs &e) {
    if (e.getName() == "controlHeader") {
        if (e.getButton()->getValue() == 1) return;
        headerSelected = true;
    }
    else if (e.getName() == "controlPresetsView") {
        if (e.getButton()->getValue() == 1) return;
        togglePresetsVisible();
    }
    else if (e.getName() == "controlPresetsSave") {
        if (e.getButton()->getValue() == 1) return;
        savePreset();
    }
    else if (e.getParentName() == "presets") {
        string path = ofToDataPath("presets/"+getName()+"/"+e.getName());
        loadPreset(path);
    }
}

//-------
void Control::addParameterToGui(Parameter<bool> *parameter) {
    gui->addToggle(parameter->getName(), parameter->getReference());
}

//-------
void Control::addParameterToGui(Parameter<int> *parameter) {
    gui->addMinimalIntSlider(parameter->getName(), parameter->getMin(), parameter->getMax(), parameter->getReference());
}

//-------
void Control::addParameterToGui(Parameter<float> *parameter) {
    gui->addMinimalSlider(parameter->getName(), parameter->getMin(), parameter->getMax(), parameter->getReference());
}

//-------
void Control::addParameterToGui(Parameter<ofVec2f> *parameter) {
    gui->addSpacer();
    gui->setWidgetSpacing(0.1);
    gui->addMinimalSlider(parameter->getName()+"->x", parameter->getMin().x, parameter->getMax().x, &parameter->getReference()->x);
    gui->addMinimalSlider(parameter->getName()+"->y", parameter->getMin().y, parameter->getMax().y, &parameter->getReference()->y)->setLabelVisible(false);
    gui->addSpacer();
    gui->setWidgetSpacing(spacing);
}

//-------
void Control::addParameterToGui(Parameter<ofVec3f> *parameter) {
    gui->addSpacer();
    gui->setWidgetSpacing(0.1);
    gui->addMinimalSlider(parameter->getName()+"->x", parameter->getMin().x, parameter->getMax().x, &parameter->getReference()->x);
    gui->addMinimalSlider(parameter->getName()+"->y", parameter->getMin().y, parameter->getMax().y, &parameter->getReference()->y)->setLabelVisible(false);
    gui->addMinimalSlider(parameter->getName()+"->z", parameter->getMin().z, parameter->getMax().z, &parameter->getReference()->z)->setLabelVisible(false);
    gui->addSpacer();
    gui->setWidgetSpacing(spacing);
}

//-------
void Control::addParameterToGui(Parameter<ofVec4f> *parameter) {
    gui->addSpacer();
    gui->setWidgetSpacing(0.1);
    gui->addMinimalSlider(parameter->getName()+"->x", parameter->getMin().x, parameter->getMax().x, &parameter->getReference()->x);
    gui->addMinimalSlider(parameter->getName()+"->y", parameter->getMin().y, parameter->getMax().y, &parameter->getReference()->y)->setLabelVisible(false);
    gui->addMinimalSlider(parameter->getName()+"->z", parameter->getMin().z, parameter->getMax().z, &parameter->getReference()->z)->setLabelVisible(false);
    gui->addMinimalSlider(parameter->getName()+"->w", parameter->getMin().w, parameter->getMax().w, &parameter->getReference()->w)->setLabelVisible(false);
    gui->addSpacer();
    gui->setWidgetSpacing(spacing);
}

//-------
void Control::addParameterToGui(Parameter<string> *parameter) {
    gui->addTextInput(parameter->getName(), *parameter->getReference());
}

//-------
void Control::clearParameters() {
    for (map<string, GuiColorVecPair*>::iterator it=colors.begin(); it!=colors.end(); ++it){
        delete it->second;
        colors.erase(it);
    }
    for (map<string, ofEvent<string>*>::iterator it=menuEvents.begin(); it!=menuEvents.end(); ++it){
        delete it->second;
        menuEvents.erase(it);
    }
    for (map<string, ofEvent<string>*>::iterator it=events.begin(); it!=events.end(); ++it){
        delete it->second;
        events.erase(it);
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
void Control::updateColors() {
    for (map<string, GuiColorVecPair*>::iterator it=colors.begin(); it!=colors.end(); ++it){
        it->second->update();
    }
}

//-------
Control::~Control() {
    ofRemoveListener(gui->newGUIEvent, this, &Control::guiEvent);
    ofRemoveListener(guiPresets->newGUIEvent, this, &Control::guiPresetsEvent);
    ofRemoveListener(ofEvents().update, this, &Control::update);
    gui->removeWidgets();
    gui->disable();
    guiPresets->removeWidgets();
    guiPresets->disable();
    clearParameters();
    delete gui;
    delete guiPresets;
}
