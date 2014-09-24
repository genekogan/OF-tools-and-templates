#include "OscManager.h"


//----------
void OscManager::registerToOsc(ParameterBase *parameter, bool send) {
    if (parameter->getType() == ParameterBase::BOOL) {
        if (send)
            outputTrackers[parameter->getOscAddress()] = new Tracker<bool>(parameter);
        else
            inputTrackers[parameter->getOscAddress()] = new Tracker<bool>(parameter);
    }
    else if (parameter->getType() == ParameterBase::STRING) {
        if (send)
            outputTrackers[parameter->getOscAddress()] = new Tracker<string>(parameter);
        else
            inputTrackers[parameter->getOscAddress()] = new Tracker<string>(parameter);
    }
    else if (parameter->getType() == ParameterBase::INT) {
        if (send)
            outputTrackers[parameter->getOscAddress()] = new Tracker<int>(parameter);
        else
            inputTrackers[parameter->getOscAddress()] = new Tracker<int>(parameter);
    }
    else if (parameter->getType() == ParameterBase::FLOAT) {
        if (send)
            outputTrackers[parameter->getOscAddress()] = new Tracker<float>(parameter);
        else
            inputTrackers[parameter->getOscAddress()] = new Tracker<float>(parameter);
    }
    else if (parameter->getType() == ParameterBase::VEC2F) {
        if (send)
            outputTrackers[parameter->getOscAddress()] = new Tracker<ofVec2f>(parameter);
        else
            inputTrackers[parameter->getOscAddress()] = new Tracker<ofVec2f>(parameter);
    }
    else if (parameter->getType() == ParameterBase::VEC3F) {
        if (send)
            outputTrackers[parameter->getOscAddress()] = new Tracker<ofVec3f>(parameter);
        else
            inputTrackers[parameter->getOscAddress()] = new Tracker<ofVec3f>(parameter);
    }
    else if (parameter->getType() == ParameterBase::VEC4F) {
        if (send)
            outputTrackers[parameter->getOscAddress()] = new Tracker<ofVec4f>(parameter);
        else
            inputTrackers[parameter->getOscAddress()] = new Tracker<ofVec4f>(parameter);
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
        
        if (inputTrackers[address]) {
            oscReceiveProcessMessage(address, msg);
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
        if (inputTrackers[address]) {
            oscReceiveProcessTouchOscMessage(address, msg, idx);
        }
    }
}

//----------
void OscManager::oscReceiveProcessMessage(string address, ofxOscMessage &msg) {
    ParameterBase::Type type = inputTrackers[address]->getType();

    if (type == ParameterBase::BOOL) {
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
        if (idx==1) {
            float newValue = ofLerp(((Parameter<ofVec2f> *) inputTrackers[address]->parameter)->getMin().x, ((Parameter<ofVec2f> *) inputTrackers[address]->parameter)->getMax().x, value);
            ((Parameter<ofVec2f> *) inputTrackers[address]->parameter)->getReference()->x = newValue;
        }
        else if (idx==2) {
            float newValue = ofLerp(((Parameter<ofVec2f> *) inputTrackers[address]->parameter)->getMin().y, ((Parameter<ofVec2f> *) inputTrackers[address]->parameter)->getMax().y, value);
            ((Parameter<ofVec2f> *) inputTrackers[address]->parameter)->getReference()->y = newValue;
        }
    }
    else if (type == ParameterBase::VEC3F) {
        if (idx==1) {
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
        if (idx==1) {
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
void OscManager::saveTouchOscLayout(string name, vector<ParameterBase *> &parameters) {
    ofxTouchOsc touchOsc;
    touchOsc.setScale(320, 540);
    
    ofxTouchOscPage *page1 = touchOsc.addPage("myPage");
    page1->setDefaultWidgetColor(YELLOW);
    
    float h = 0.94 / parameters.size();
    float y = 0.01;
    
    for (int i=0; i<parameters.size(); i++) {
        ParameterBase::Type type = parameters[i]->getType();
        if (type == ParameterBase::BOOL) {
            ofxTouchOscToggle *toggle = page1->addToggle(parameters[i]->getName(), 0.01, y, 0.4, h*0.85);
            toggle->setOscAddress(parameters[i]->getOscAddress());
            page1->addLabel(parameters[i]->getName(), 0.01, y, 0.4, h*0.85);
        }
        else if (type == ParameterBase::INT) {
            ofxTouchOscFader *fader = page1->addFader(parameters[i]->getName(), 0.01, y, 0.98, h*0.85);
            fader->setOscAddress(parameters[i]->getOscAddress());
            fader->setRange(parameters[i]->getMin<int>(), parameters[i]->getMax<int>());
            page1->addLabel(parameters[i]->getName(), 0.01, y, 0.98, h*0.85);
        }
        else if (type == ParameterBase::STRING) {
        }
        else if (type == ParameterBase::FLOAT) {
            ofxTouchOscFader *fader = page1->addFader(parameters[i]->getName(), 0.01, y, 0.98, h*0.85);
            fader->setOscAddress(parameters[i]->getOscAddress());
            fader->setRange(parameters[i]->getMin<float>(), parameters[i]->getMax<float>());
            page1->addLabel(parameters[i]->getName(), 0.01, y, 0.98, h*0.85);
        }
        else if (type == ParameterBase::VEC2F) {
            ofxTouchOscMultiFader *fader = page1->addMultiFader(parameters[i]->getName(), 0.01, y, 0.98, h*0.85);
            fader->setOscAddress(parameters[i]->getOscAddress());
            fader->setNumber(2);
            page1->addLabel(parameters[i]->getName(), 0.01, y, 0.98, h*0.85);
        }
        else if (type == ParameterBase::VEC3F) {
            ofxTouchOscMultiFader *fader = page1->addMultiFader(parameters[i]->getName(), 0.01, y, 0.98, h*0.85);
            fader->setOscAddress(parameters[i]->getOscAddress());
            fader->setNumber(3);
            page1->addLabel(parameters[i]->getName(), 0.01, y, 0.98, h*0.85);
        }
        else if (type == ParameterBase::VEC4F) {
            ofxTouchOscMultiFader *fader = page1->addMultiFader(parameters[i]->getName(), 0.01, y, 0.98, h*0.85);
            fader->setOscAddress(parameters[i]->getOscAddress());
            fader->setNumber(4);
            page1->addLabel(parameters[i]->getName(), 0.01, y, 0.98, h*0.85);
        }
        y += h;
    }
    
    touchOsc.save(name);
}

//----------
void OscManager::clearInputTrackers() {
    for (map<string, TrackerBase*>::iterator it=inputTrackers.begin(); it!=inputTrackers.end(); ++it){
        delete it->second;
    }
    inputTrackers.clear();
}

//----------
void OscManager::clearOutputTrackers() {
    for (map<string, TrackerBase*>::iterator it=outputTrackers.begin(); it!=outputTrackers.end(); ++it){
        delete it->second;
    }
    outputTrackers.clear();
}

//----------
OscManager::~OscManager() {
    for (map<string, TrackerBase*>::iterator it=inputTrackers.begin(); it!=inputTrackers.end(); ++it){
        delete it->second;
    }
    for (map<string, TrackerBase*>::iterator it=outputTrackers.begin(); it!=outputTrackers.end(); ++it){
        delete it->second;
    }
}