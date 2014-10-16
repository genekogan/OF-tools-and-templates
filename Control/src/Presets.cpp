#include "Presets.h"


//--------------
bool Presets::savePreset(Control &control, string filename) {
    if (filename=="") {
        filename = ofSystemTextBoxDialog("Choose a filename");
    }
    if (filename=="") {
        return false;
    }
    string path = ofToDataPath("presets/"+control.getName()+"/"+filename+".xml");
    ofXml xml = getXml(control);
    xml.save(path);
    return true;
}

//--------------
void Presets::loadPreset(Control &control, string path, int numFrames) {
    ofXml xml;
    bool xmlLoaded = xml.load(path);
    
    if (xmlLoaded) {
        loadPreset(control, xml, numFrames);
    }
    else {
        ofLog(OF_LOG_ERROR, "failed to load preset "+path);
    }
}

//--------------
void Presets::loadPreset(Control &control, ofXml &xml, int numFrames) {
    vector<ParameterBase *> parameters = control.getParameters();
    vector<ofxUIDropDownList *> menus = control.getMenus();
    
    xml.setTo("Preset");
    xml.setTo("Parameters");
    
    if (xml.exists("Parameter[0]")) {
        xml.setTo("Parameter[0]");

        do {
            string name = xml.getValue<string>("Name");
            string type = xml.getValue<string>("Type");
            
            if      (type == "Bool") {
                updateParameter<bool>(xml, parameters, ParameterBase::BOOL);
            }
            else if (type == "String") {
                updateParameter<string>(xml, parameters, ParameterBase::STRING);
            }
            else if (type == "Int") {
                updateParameter<int>(xml, parameters, ParameterBase::INT, numFrames);
            }
            else if (type == "Float") {
                updateParameter<float>(xml, parameters, ParameterBase::FLOAT, numFrames);
            }
            else if (type == "Vec2f") {
                updateParameter<ofVec2f>(xml, parameters, ParameterBase::VEC2F, numFrames);
            }
            else if (type == "Vec3f") {
                updateParameter<ofVec3f>(xml, parameters, ParameterBase::VEC3F, numFrames);
            }
            else if (type == "Vec4f") {
                updateParameter<ofVec4f>(xml, parameters, ParameterBase::VEC4F, numFrames);
            }
        }
        while(xml.setToSibling());
        
        xml.setToParent();
    }
        
    xml.setToParent();
    
    xml.setTo("Menus");
        
    if (xml.exists("Menu[0]")) {
        xml.setTo("Menu[0]");
        
        do {
            string menuName = xml.getValue<string>("Name");
            xml.setTo("Items");
            xml.setTo("Item[0]");
            do {
                string itemName = xml.getValue("Name");
                bool value = xml.getValue("Value") == "1";
                control.triggerMenuEvent(menuName, itemName, value);
            }
            while(xml.setToSibling());
            xml.setToParent();
            xml.setToParent();
        }
        while (xml.setToSibling());
        
        xml.setToParent();
    }
}

//--------------
ofXml Presets::getXml(Control &control) {
    ofXml parametersXml = getXml(control.getParameters());
    ofXml menusXml = getXml(control.getMenus());
    ofXml xml;
    xml.addChild("Preset");
    xml.setTo("Preset");
    xml.addXml(parametersXml);
    xml.addXml(menusXml);
    return xml;
}

//--------------
ofXml Presets::getXml(vector<ParameterBase *> &parameters) {
    ofXml xml;
    xml.addChild("Parameters");
    xml.setTo("Parameters");
    for (int i=0; i<parameters.size(); i++) {
        ofXml xmlp = getXml(parameters[i]);
        xml.addXml(xmlp);
    }
    return xml;
}

//--------------
ofXml Presets::getXml(ParameterBase * parameter) {
    string name = parameter->getName();
    string oscAddress = parameter->getOscAddress();
    ParameterBase::Type type = parameter->getType();
    
    ofXml xml;
    xml.addChild("Parameter");
    xml.setTo("Parameter");
    xml.addValue("Name", name);
    xml.addValue("OscAddress", oscAddress);
    
    if      (type == ParameterBase::BOOL) {
        xml.addValue("Type", "Bool");
        parameterToXml(xml, (Parameter<bool> *) parameter);
    }
    else if (type == ParameterBase::STRING) {
        xml.addValue("Type", "String");
        parameterToXml(xml, (Parameter<string> *) parameter);
    }
    else if (type == ParameterBase::INT) {
        xml.addValue("Type", "Int");
        parameterToXml(xml, (Parameter<int> *) parameter);
    }
    else if (type == ParameterBase::FLOAT) {
        xml.addValue("Type", "Float");
        parameterToXml(xml, (Parameter<float> *) parameter);
    }
    else if (type == ParameterBase::VEC2F) {
        xml.addValue("Type", "Vec2f");
        parameterToXml(xml, (Parameter<ofVec2f> *) parameter);
    }
    else if (type == ParameterBase::VEC3F) {
        xml.addValue("Type", "Vec3f");
        parameterToXml(xml, (Parameter<ofVec3f> *) parameter);
    }
    else if (type == ParameterBase::VEC4F) {
        xml.addValue("Type", "Vec4f");
        parameterToXml(xml, (Parameter<ofVec4f> *) parameter);
    }
    return xml;
}

//--------------
ofXml Presets::getXml(vector<ofxUIDropDownList *> menus) {
    ofXml xml;
    xml.addChild("Menus");
    xml.setTo("Menus");
    for (int i=0; i<menus.size(); i++) {
        ofXml xmlm = getXml(menus[i]);
        xml.addXml(xmlm);
    }
    return xml;
}

//--------------
ofXml Presets::getXml(ofxUIDropDownList * menu) {
    string name = menu->getName();
    vector<ofxUILabelToggle *> toggles = menu->getToggles();
    ofXml xml, xmlItems;
    xml.addChild("Menu");
    xml.setTo("Menu");
    xml.addValue("Name", name);
    xmlItems.addChild("Items");
    xmlItems.setTo("Items");
    for (int j=0; j<toggles.size(); j++) {
        ofXml xmlItem;
        xmlItem.addChild("Item");
        xmlItem.setTo("Item");
        xmlItem.addValue("Name", toggles[j]->getName());
        xmlItem.addValue("Value", toggles[j]->getValue() ? "1" : "0");
        xmlItems.addXml(xmlItem);
    }
    xml.addXml(xmlItems);
    return xml;
}
