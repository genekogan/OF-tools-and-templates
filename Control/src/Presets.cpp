#include "Presets.h"


//--------------
bool Presets::savePreset(string name, vector<ParameterBase *> &parameters) {
    ofXml mainXml = getXml(parameters);
    string filename = ofSystemTextBoxDialog("Choose a filename");
    if (filename=="")   return false;
    string path = ofToDataPath("presets/"+name+"/"+filename+".xml");
    mainXml.save(path);
    return true;
}

//--------------
ofXml Presets::getXml(vector<ParameterBase *> &parameters) {
    ofXml mainXml;
    mainXml.addChild("Parameters");
    mainXml.setTo("Parameters");
    
    for (int i=0; i<parameters.size(); i++) {
        string name = parameters[i]->getName();
        string oscAddress = parameters[i]->getOscAddress();

        ofXml xml;
        xml.addChild("Parameter");
        xml.setTo("Parameter");
        xml.addValue("Name", name);
        xml.addValue("OscAddress", oscAddress);
        
        ParameterBase::Type type = parameters[i]->getType();
        if      (type == ParameterBase::BOOL) {
            xml.addValue("Type", "Bool");
            parameterToXml(xml, (Parameter<bool> *) parameters[i]);
        }
        else if (type == ParameterBase::STRING) {
            xml.addValue("Type", "String");
            parameterToXml(xml, (Parameter<string> *) parameters[i]);
        }
        else if (type == ParameterBase::INT) {
            xml.addValue("Type", "Int");
            parameterToXml(xml, (Parameter<int> *) parameters[i]);
        }
        else if (type == ParameterBase::FLOAT) {
            xml.addValue("Type", "Float");
            parameterToXml(xml, (Parameter<float> *) parameters[i]);
        }
        else if (type == ParameterBase::VEC2F) {
            xml.addValue("Type", "Vec2f");
            parameterToXml(xml, (Parameter<ofVec2f> *) parameters[i]);
        }
        else if (type == ParameterBase::VEC3F) {
            xml.addValue("Type", "Vec3f");
            parameterToXml(xml, (Parameter<ofVec3f> *) parameters[i]);
        }
        else if (type == ParameterBase::VEC4F) {
            xml.addValue("Type", "Vec4f");
            parameterToXml(xml, (Parameter<ofVec4f> *) parameters[i]);
        }
        mainXml.addXml(xml);
    }
    return mainXml;
}

//--------------
void Presets::loadPreset(string path, vector<ParameterBase *> &parameters, int numFrames) {
    ofXml xml;
    bool xmlLoaded = xml.load(path);
    
    if (!xmlLoaded) {
        cout << "failed to load " << endl;
        return;
    }
    
    xml.setTo("Parameters");
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
}
