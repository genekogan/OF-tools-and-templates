#include "ControlPanel.h"


//-------
Control::Control() {
    gui = new ofxUICanvas("control");
    guiPresets = new ofxUICanvas("controlPresets");
    setWidth(150);
    spacing = gui->getWidgetSpacing();
    headerSelected = false;
    setupGuiPresets();
    visible = true;
    metaActive = false;
    setActive(true);
}

//-------
void Control::setupMetaController() {
    meta = new MetaController();
    meta->setup(this);
    metaActive = true;
}

//-------
void Control::update(ofEventArgs &data) {
    if (active) {
        updateColors();
        if (metaActive) {
            meta->update();
        }
    }
}

//-------
void Control::draw(ofEventArgs &data) {
    if (active && metaActive) {
        meta->draw();
    }
}

//-------
void Control::setName(string name) {
    this->name = name;
    setupGuiPresets();
}

//-------
void Control::savePreset() {
    Presets presets;
    bool saved = presets.savePreset(*this);
    if (!saved) return;
    setupGuiPresets();
}

//-------
void Control::loadPreset(string path) {
    Presets presets;
    presets.loadPreset(*this, path, numLerpFrames);
}

//-------
void Control::setWidth(int width) {
    this->width = width;
    gui->setWidth(width);
    guiPresets->setWidth(width);
}

//-------
void Control::setGuiPosition(int x, int y) {
    gui->setPosition(x, y);
    guiPresets->setPosition(x, y);
}

//-------
void Control::setActive(bool active) {
    this->active = active;
    if (active) {
        ofAddListener(gui->newGUIEvent, this, &Control::guiEvent);
        ofAddListener(guiPresets->newGUIEvent, this, &Control::guiPresetsEvent);
        ofAddListener(ofEvents().update, this, &Control::update);
        ofAddListener(ofEvents().draw, this, &Control::draw);
    }
    else {
        ofRemoveListener(gui->newGUIEvent, this, &Control::guiEvent);
        ofRemoveListener(guiPresets->newGUIEvent, this, &Control::guiPresetsEvent);
        ofRemoveListener(ofEvents().update, this, &Control::update);
        ofRemoveListener(ofEvents().draw, this, &Control::draw);
    }
    setVisible(active);
}

//-------
void Control::setVisible(bool visible) {
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

//-------
void Control::toggleVisible() {
    setVisible(!visible);
}

//-------
void Control::togglePresetsVisible() {
    visible = !visible;
    gui->setVisible(visible);
    guiPresets->setVisible(!visible);
}

//-------
vector<ofxUIDropDownList *> Control::getMenus() {
    vector<ofxUIDropDownList *> dropdowns;
    for (map<string,vector<string> >::iterator it=menus.begin(); it!=menus.end(); ++it){
        ofxUIDropDownList *dropdown = (ofxUIDropDownList *) gui->getWidget(it->first);
        dropdowns.push_back(dropdown);
    }
    return dropdowns;
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
        if (!parametersVisible[parameters[i]])   continue;
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
        menu->open();
        menu->setPadding(1);
    }
    
    /* add events */
    for (map<string,ofEvent<string>*>::iterator it=events.begin(); it!=events.end(); ++it){
        gui->addButton(it->first, false);
    }
    
    /* set color of color sliders */
    updateColors();
    
    gui->autoSizeToFitWidgets();
}

//-------
void Control::guiEvent(ofxUIEventArgs &e) {
    // menu select notification
    if (menus.count(e.getParentName()) > 0) {
        triggerMenuEvent(e.getParentName(), e.getName(), e.getToggle()->getValue());
    }
    
    // event notification
    else if (events.count(e.getName()) > 0) {
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
        togglePresetsVisible();
    }
}

//-------
void Control::triggerMenuEvent(string menuName, string selection, bool trigger) {
    if (selection == "")    return;
    if (trigger) {
        ofNotifyEvent(*menuEvents[menuName], selection, this);
        ((ofxUILabelToggle *) gui->getWidget(selection))->setValue(trigger);
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
void Control::addColor(string name, ofColor *value) {
    string sn = ofToString(ofRandom(1000));
    ofVec4f *vec = new ofVec4f(value->r, value->g, value->b, value->a);
    GuiColorVecPair *color = new GuiColorVecPair();
    color->color = value;
    color->vec = vec;
    colors[name] = color;
    ParameterBase *parameter = new Parameter<ofVec4f>(name, *vec, ofVec4f(0, 0, 0, 0), ofVec4f(255, 255, 255, 255));
    parameters.push_back(parameter);
    parametersVisible[parameter] = true;
    setupGui();
}

//-------
void Control::addParameterToGui(Parameter<string> *parameter) {
    gui->addTextInput(parameter->getName(), *parameter->getReference());
}

//-------
void Control::clear() {
    for (map<string, GuiColorVecPair*>::iterator it=colors.begin(); it!=colors.end(); ++it){
        delete it->second;
        colors.erase(it);
    }
    for (map<string, ofEvent<string>*>::iterator it=menuEvents.begin(); it!=menuEvents.end(); ++it){
        it->second->disable();
        it->second->clear();
        delete it->second;
        menuEvents.erase(it);
    }
    for (map<string, ofEvent<string>*>::iterator it=events.begin(); it!=events.end(); ++it){
        it->second->disable();
        it->second->clear();
        delete it->second;
        events.erase(it);
    }
    for (map<ParameterBase *, bool>::iterator it=parametersVisible.begin(); it!=parametersVisible.end(); ++it){
        parametersVisible.erase(it);
    }
    for (int i=0; i<parameters.size(); i++) {
        delete parameters[i];
    }
    colors.clear();
    menuEvents.clear();
    menus.clear();
    events.clear();
    parameters.clear();
    parametersVisible.clear();
}

//-------
void Control::updateColors() {
    for (map<string, GuiColorVecPair*>::iterator it=colors.begin(); it!=colors.end(); ++it){
        it->second->update();
        gui->getWidget(it->first+"->x")->setColorFill(*colors[it->first]->color);
        gui->getWidget(it->first+"->y")->setColorFill(*colors[it->first]->color);
        gui->getWidget(it->first+"->z")->setColorFill(*colors[it->first]->color);
        gui->getWidget(it->first+"->w")->setColorFill(*colors[it->first]->color);
    }
}

//-------
Control::~Control() {
    setActive(false);
    gui->removeWidgets();
    gui->disable();
    guiPresets->removeWidgets();
    guiPresets->disable();
    clear();
    delete gui;
    delete guiPresets;
}
