#include "OscManager.h"

//----------
bool OscManager::setupSender(string host, int portOut) {
    try {
        oscSender.setup(host, portOut);
        this->host = host;
        this->portOut = portOut;
        sending = true;
        ofLog(OF_LOG_NOTICE, "Connect OSC Sender "+host+", port "+ofToString(portOut));
    }
    catch(runtime_error &e) {
        ofLog(OF_LOG_ERROR, ofToString(e.what()));
    }
    return sending;
}

//----------
bool OscManager::setupReceiver(int portIn) {
    try {
        oscReceiver.setup(portIn);
        this->portIn = portIn;
        receiving = true;
        ofLog(OF_LOG_NOTICE, "Connect OSC Receiver, port "+ofToString(portIn));
    }
    catch(runtime_error &e) {
        ofLog(OF_LOG_ERROR, ofToString(e.what()));
    }
    return receiving;
}

//----------
void OscManager::update() {
    if (receiving)
        oscReceiveChanges();
    if (sending)
        oscSendChanges();
}

//----------
void OscManager::registerToOscSender(vector<ParameterBase *> &parameters) {
    for (int i=0; i<parameters.size(); i++) {
        registerToOsc(parameters[i], true);
    }
}

//----------
void OscManager::registerToOscReceiver(vector<ParameterBase *> &parameters) {
    for (int i=0; i<parameters.size(); i++) {
        registerToOsc(parameters[i], false);
    }
}

//----------
void OscManager::registerToOsc(ParameterBase *parameter, bool send) {
    if (parameter->getType() == ParameterBase::BOOL) {
        registerParameterToOsc<bool>(parameter, send);
    }
    else if (parameter->getType() == ParameterBase::STRING) {
        registerParameterToOsc<string>(parameter, send);
    }
    else if (parameter->getType() == ParameterBase::INT) {
        registerParameterToOsc<int>(parameter, send);
    }
    else if (parameter->getType() == ParameterBase::FLOAT) {
        registerParameterToOsc<float>(parameter, send);
    }
    else if (parameter->getType() == ParameterBase::VEC2F) {
        registerParameterToOsc<ofVec2f>(parameter, send);
    }
    else if (parameter->getType() == ParameterBase::VEC3F) {
        registerParameterToOsc<ofVec3f>(parameter, send);
    }
    else if (parameter->getType() == ParameterBase::VEC4F) {
        registerParameterToOsc<ofVec4f>(parameter, send);
    }
}

//----------
void OscManager::oscSendChanges() {
    for (map<string, TrackerBase*>::iterator it=outputTrackers.begin(); it!=outputTrackers.end(); ++it){
        if (it->second->getType() == ParameterBase::BOOL) {
            checkIfToSendOscMessage<bool>(it);
        }
        else if (it->second->getType() == ParameterBase::STRING) {
            checkIfToSendOscMessage<string>(it);
        }
        else if (it->second->getType() == ParameterBase::INT) {
            checkIfToSendOscMessage<int>(it);
        }
        else if (it->second->getType() == ParameterBase::FLOAT) {
            checkIfToSendOscMessage<float>(it);
        }
        else if (it->second->getType() == ParameterBase::VEC2F) {
            checkIfToSendOscMessage<ofVec2f>(it);
        }
        else if (it->second->getType() == ParameterBase::VEC3F) {
            checkIfToSendOscMessage<ofVec3f>(it);
        }
        else if (it->second->getType() == ParameterBase::VEC4F) {
            checkIfToSendOscMessage<ofVec4f>(it);
        }
    }
}

//----------
void OscManager::oscReceiveChanges() {
    while(oscReceiver.hasWaitingMessages()){
        ofxOscMessage msg;
        oscReceiver.getNextMessage(&msg);
        string address = msg.getAddress();
        if (inputTrackers.count(address) > 0) {
            oscReceiveProcessMessage(address, msg);
        }
        else if (eventTrackers.count(address) > 0) {
            bool b = msg.getArgAsInt32(0) == 1 ? true : false;
            ofNotifyEvent(*eventTrackers[address], b, this);
        }
        else {
            oscReceiveTouchOscChanges(msg);
        }
    }
}

//----------
void OscManager::oscReceiveTouchOscChanges(ofxOscMessage &msg) {
    vector<string> m = ofSplitString(msg.getAddress(), "/");
    if (m.size()>1 && m[m.size()-1].length()==1 && ofToInt(m[m.size()-1])>0) {
        int idx = ofToInt(m[m.size()-1]);
        m.pop_back();
        string address = ofJoinString(m, "/");
        if (inputTrackers.count(address) > 0) {
            oscReceiveProcessTouchOscMessage(address, msg, idx);
        }
    }
}

//----------
void OscManager::oscReceiveProcessMessage(string address, ofxOscMessage &msg) {
    ParameterBase::Type type = inputTrackers[address]->getType();
    
    if      (type == ParameterBase::BOOL) {
        ((Parameter<bool> *) inputTrackers[address]->parameter)->set(msg.getArgAsInt32(0)==1);
    }
    else if (type == ParameterBase::STRING) {
        ((Parameter<string> *) inputTrackers[address]->parameter)->set(msg.getArgAsString(0));
    }
    else if (type == ParameterBase::INT) {
        ((Parameter<int> *) inputTrackers[address]->parameter)->set((int) msg.getArgAsFloat(0));
    }
    else if (type == ParameterBase::FLOAT) {
        ((Parameter<float> *) inputTrackers[address]->parameter)->set(msg.getArgAsFloat(0));
    }
    else if (type == ParameterBase::VEC2F) {
        ((Parameter<ofVec2f> *) inputTrackers[address]->parameter)->set(ofVec2f(msg.getArgAsFloat(0), msg.getArgAsFloat(1)));
    }
    else if (type == ParameterBase::VEC3F) {
        ((Parameter<ofVec3f> *) inputTrackers[address]->parameter)->set(ofVec3f(msg.getArgAsFloat(0), msg.getArgAsFloat(1), msg.getArgAsFloat(2)));
    }
    else if (type == ParameterBase::VEC4F) {
        ((Parameter<ofVec4f> *) inputTrackers[address]->parameter)->set(ofVec4f(msg.getArgAsFloat(0), msg.getArgAsFloat(1), msg.getArgAsFloat(2), msg.getArgAsFloat(3)));
    }
}

//----------
void OscManager::oscReceiveProcessTouchOscMessage(string address, ofxOscMessage &msg, int idx) {
    ParameterBase::Type type = inputTrackers[address]->getType();
    float value = msg.getArgAsFloat(0);
    
    if (type == ParameterBase::VEC2F) {
        if      (idx==1) {
            float newValue = ofLerp(((Parameter<ofVec2f> *) inputTrackers[address]->parameter)->getMin().x, ((Parameter<ofVec2f> *) inputTrackers[address]->parameter)->getMax().x, value);
            ((Parameter<ofVec2f> *) inputTrackers[address]->parameter)->getReference()->x = newValue;
        }
        else if (idx==2) {
            float newValue = ofLerp(((Parameter<ofVec2f> *) inputTrackers[address]->parameter)->getMin().y, ((Parameter<ofVec2f> *) inputTrackers[address]->parameter)->getMax().y, value);
            ((Parameter<ofVec2f> *) inputTrackers[address]->parameter)->getReference()->y = newValue;
        }
    }
    else if (type == ParameterBase::VEC3F) {
        if      (idx==1) {
            float newValue = ofLerp(((Parameter<ofVec3f> *) inputTrackers[address]->parameter)->getMin().x, ((Parameter<ofVec3f> *) inputTrackers[address]->parameter)->getMax().x, value);
            ((Parameter<ofVec3f> *) inputTrackers[address]->parameter)->getReference()->x = newValue;
        }
        else if (idx==2) {
            float newValue = ofLerp(((Parameter<ofVec3f> *) inputTrackers[address]->parameter)->getMin().y, ((Parameter<ofVec3f> *) inputTrackers[address]->parameter)->getMax().y, value);
            ((Parameter<ofVec3f> *) inputTrackers[address]->parameter)->getReference()->y = newValue;
        }
        else if (idx==3) {
            float newValue = ofLerp(((Parameter<ofVec3f> *) inputTrackers[address]->parameter)->getMin().z, ((Parameter<ofVec3f> *) inputTrackers[address]->parameter)->getMax().z, value);
            ((Parameter<ofVec3f> *) inputTrackers[address]->parameter)->getReference()->z = newValue;
        }
    }
    else if (type == ParameterBase::VEC4F) {
        if      (idx==1) {
            float newValue = ofLerp(((Parameter<ofVec4f> *) inputTrackers[address]->parameter)->getMin().x, ((Parameter<ofVec4f> *) inputTrackers[address]->parameter)->getMax().x, value);
            ((Parameter<ofVec4f> *) inputTrackers[address]->parameter)->getReference()->x = newValue;
        }
        else if (idx==2) {
            float newValue = ofLerp(((Parameter<ofVec4f> *) inputTrackers[address]->parameter)->getMin().y, ((Parameter<ofVec4f> *) inputTrackers[address]->parameter)->getMax().y, value);
            ((Parameter<ofVec4f> *) inputTrackers[address]->parameter)->getReference()->y = newValue;
        }
        else if (idx==3) {
            float newValue = ofLerp(((Parameter<ofVec4f> *) inputTrackers[address]->parameter)->getMin().z, ((Parameter<ofVec4f> *) inputTrackers[address]->parameter)->getMax().z, value);
            ((Parameter<ofVec4f> *) inputTrackers[address]->parameter)->getReference()->z = newValue;
        }
        else if (idx==4) {
            float newValue = ofLerp(((Parameter<ofVec4f> *) inputTrackers[address]->parameter)->getMin().w, ((Parameter<ofVec4f> *) inputTrackers[address]->parameter)->getMax().w, value);
            ((Parameter<ofVec4f> *) inputTrackers[address]->parameter)->getReference()->w = newValue;
        }
    }
}

//----------
template <typename T>
void OscManager::registerParameterToOsc(ParameterBase *parameter, bool send) {
    if (send) {
        if (outputTrackers.count(parameter->getOscAddress()) == 0) {
            outputTrackers[parameter->getOscAddress()] = new Tracker<T>(parameter);
        }
        else {
            ofLog(OF_LOG_WARNING, "Warning: parameter "+parameter->getName()+" already registered to output OSC");
        }
    }
    else {
        if (inputTrackers.count(parameter->getOscAddress()) == 0) {
            inputTrackers[parameter->getOscAddress()] = new Tracker<T>(parameter);
        }
        else {
            ofLog(OF_LOG_WARNING, "Warning: parameter "+parameter->getName()+" already registered to input OSC");
        }
    }
}

//----------
template <typename T>
void OscManager::checkIfToSendOscMessage(map<string, TrackerBase*>::iterator &it) {
    if (((Tracker<T> *) it->second)->checkChanged()) {
        //ofLog(OF_LOG_VERBOSE, "send OSC "+ it->first +" :: "+ ofToString(it->second->getValue<T>()));
        ofxOscMessage msg;
        msg.setAddress(it->first);
        addOscArgs(msg, it->second->getValue<T>());
        oscSender.sendMessage(msg);
    }
}

//----------
template<> inline void OscManager::addOscArgs<bool>(ofxOscMessage &msg, bool val) {
    msg.addIntArg(val);
}
template<> inline void OscManager::addOscArgs<string>(ofxOscMessage &msg, string val) {
    msg.addStringArg(val);
}
template<> inline void OscManager::addOscArgs<int>(ofxOscMessage &msg, int val) {
    msg.addIntArg(val);
}
template<> inline void OscManager::addOscArgs<float>(ofxOscMessage &msg, float val) {
    msg.addFloatArg(val);
}
template<> inline void OscManager::addOscArgs<ofVec2f>(ofxOscMessage &msg, ofVec2f val) {
    msg.addFloatArg(val.x);
    msg.addFloatArg(val.y);
}
template<> inline void OscManager::addOscArgs<ofVec3f>(ofxOscMessage &msg, ofVec3f val) {
    msg.addFloatArg(val.x);
    msg.addFloatArg(val.y);
    msg.addFloatArg(val.z);
}
template<> inline void OscManager::addOscArgs<ofVec4f>(ofxOscMessage &msg, ofVec4f val) {
    msg.addFloatArg(val.x);
    msg.addFloatArg(val.y);
    msg.addFloatArg(val.z);
    msg.addFloatArg(val.w);
}

//----------
void OscManager::sendMessageManually(string address, vector<int> val) {
    ofxOscMessage msg;
    msg.setAddress(address);
    for (int i=0; i<val.size(); i++)
        msg.addIntArg(val[i]);
    oscSender.sendMessage(msg);
}


//----------
void OscManager::sendMessageManually(string address, vector<float> val) {
    ofxOscMessage msg;
    msg.setAddress(address);
    for (int i=0; i<val.size(); i++)
        msg.addFloatArg(val[i]);
    oscSender.sendMessage(msg);
}

//----------
void OscManager::saveTouchOscLayout(string name, vector<ParameterBase *> &parameters) {
    ofxTouchOsc touchOsc;
    touchOsc.setScale(320, 540);
    ofxTouchOscPage * page = makeTouchOscPage(name, parameters);
    touchOsc.addPage(page);
    touchOsc.save(name);
}

//----------
ofxTouchOscPage * OscManager::makeTouchOscPage(string name, vector<ParameterBase *> &parameters) {
    ofxTouchOscPage *page = new ofxTouchOscPage("myPage");
    page->setDefaultWidgetColor(YELLOW);
    
    float h = 0.94 / parameters.size();
    float y = 0.01;
    
    for (int i=0; i<parameters.size(); i++) {
        ParameterBase::Type type = parameters[i]->getType();
        if (type == ParameterBase::BOOL) {
            ofxTouchOscToggle *toggle = page->addToggle(parameters[i]->getName(), 0.01, y, 0.4, h*0.85);
            toggle->setOscAddress(parameters[i]->getOscAddress());
            page->addLabel(parameters[i]->getName(), 0.01, y, 0.4, h*0.85);
        }
        else if (type == ParameterBase::INT) {
            ofxTouchOscFader *fader = page->addFader(parameters[i]->getName(), 0.01, y, 0.98, h*0.85);
            fader->setOscAddress(parameters[i]->getOscAddress());
            fader->setRange(parameters[i]->getMin<int>(), parameters[i]->getMax<int>());
            page->addLabel(parameters[i]->getName(), 0.01, y, 0.98, h*0.85);
        }
        else if (type == ParameterBase::STRING) {
        }
        else if (type == ParameterBase::FLOAT) {
            ofxTouchOscFader *fader = page->addFader(parameters[i]->getName(), 0.01, y, 0.98, h*0.85);
            fader->setOscAddress(parameters[i]->getOscAddress());
            fader->setRange(parameters[i]->getMin<float>(), parameters[i]->getMax<float>());
            page->addLabel(parameters[i]->getName(), 0.01, y, 0.98, h*0.85);
        }
        else if (type == ParameterBase::VEC2F) {
            ofxTouchOscMultiFader *fader = page->addMultiFader(parameters[i]->getName(), 0.01, y, 0.98, h*0.85);
            fader->setOscAddress(parameters[i]->getOscAddress());
            fader->setNumber(2);
            page->addLabel(parameters[i]->getName(), 0.01, y, 0.98, h*0.85);
        }
        else if (type == ParameterBase::VEC3F) {
            ofxTouchOscMultiFader *fader = page->addMultiFader(parameters[i]->getName(), 0.01, y, 0.98, h*0.85);
            fader->setOscAddress(parameters[i]->getOscAddress());
            fader->setNumber(3);
            page->addLabel(parameters[i]->getName(), 0.01, y, 0.98, h*0.85);
        }
        else if (type == ParameterBase::VEC4F) {
            ofxTouchOscMultiFader *fader = page->addMultiFader(parameters[i]->getName(), 0.01, y, 0.98, h*0.85);
            fader->setOscAddress(parameters[i]->getOscAddress());
            fader->setNumber(4);
            page->addLabel(parameters[i]->getName(), 0.01, y, 0.98, h*0.85);
        }
        y += h;
    }
    return page;
}

//----------
void OscManager::clearInputTrackers() {
    for (map<string, TrackerBase*>::iterator it=inputTrackers.begin(); it!=inputTrackers.end(); ++it){
        delete it->second;
        inputTrackers.erase(it);
    }
    inputTrackers.clear();
}

//----------
void OscManager::clearOutputTrackers() {
    for (map<string, TrackerBase*>::iterator it=outputTrackers.begin(); it!=outputTrackers.end(); ++it){
        delete it->second;
        outputTrackers.erase(it);
    }
    outputTrackers.clear();
}

//----------
void OscManager::clearEventTrackers() {
    for (map<string, ofEvent<bool>*>::iterator it=eventTrackers.begin(); it!=eventTrackers.end(); ++it){
        it->second->disable();
        it->second->clear();
        delete it->second;
        eventTrackers.erase(it);
    }
    eventTrackers.clear();
}

//----------
OscManager::~OscManager() {
    clearInputTrackers();
    clearOutputTrackers();
    clearEventTrackers();
}

//----------
void OscManager::listTrackedParameters() {
    cout <<"List of tracked parameters:"<<endl;
    for (map<string, TrackerBase*>::iterator it=inputTrackers.begin(); it!=inputTrackers.end(); ++it){
        cout << " INPUT :: " <<it->first << endl;
    }
    for (map<string, TrackerBase*>::iterator it=outputTrackers.begin(); it!=outputTrackers.end(); ++it){
        cout << " OUTPUT :: " <<it->first << endl;
    }
}
