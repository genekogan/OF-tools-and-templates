#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxTouchOsc.h"
#include "Parameter.h"



class OscManager
{
public:
    ~OscManager();
    
    void setupSender(string host, int portOut) {
        try {
            oscSender.setup(host, portOut);
            this->host = host;
            this->portOut = portOut;
            sending = true;
        }
        catch(runtime_error &e) {
            ofLog(OF_LOG_ERROR, ofToString(e.what()));
        }
    }
    
    void setupReceiver(int portIn) {
        try {
            oscReceiver.setup(portIn);
            this->portIn = portIn;
            receiving = true;
        }
        catch(runtime_error &e) {
            ofLog(OF_LOG_ERROR, ofToString(e.what()));
        }
    }
    
    string getHost() {return host;}
    int getSenderPort() {return portOut;}
    int getReceiverPort() {return portIn;}
    bool getSending() {return sending;}
    bool getReceiving() {return receiving;}
    
    void update() {
        if (receiving)
            oscReceiveChanges();
        if (sending)
            oscSendChanges();
    }
    
    void registerToOscSender(vector<ParameterBase *> &parameters) {
        for (int i=0; i<parameters.size(); i++) {
            registerToOsc(parameters[i], true);
        }
    }
    
    void registerToOscReceiver(vector<ParameterBase *> &parameters) {
        for (int i=0; i<parameters.size(); i++) {
            registerToOsc(parameters[i], false);
        }
    }
    
    void registerToOsc(ParameterBase *parameter, bool send);

    void clearInputTrackers();
    void clearOutputTrackers();
    
    void saveTouchOscLayout(string name, vector<ParameterBase *> &parameters);

private:
    
    struct TrackerBase {
        ParameterBase *parameter;
        template<class T> T getValue() {
            return ((Parameter<T> *) parameter)->get();
        }
        ParameterBase::Type getType() { return parameter->getType(); }
    };
    
    template <typename T>
    struct Tracker : public TrackerBase {
        T value;
        Tracker(ParameterBase *p) {
            parameter = p;
        }
        bool checkChanged() {
            bool isChanged = value != ((Parameter<T> *) parameter)->get();
            value = ((Parameter<T> *) parameter)->get();
            return isChanged;
        }
    };
    
    template <typename T>
    void checkIfToSendOscMessage(map<string, TrackerBase*>::iterator &it) {
        if (((Tracker<T> *) it->second)->checkChanged()) {
            cout << "send OSC " << it->first << " :: "<<ofToString(it->second->getValue<T>()) << endl;
            ofxOscMessage msg;
            msg.setAddress(it->first);
            addOscArgs(msg, it->second->getValue<T>());
            oscSender.sendMessage(msg);
        }
    }
    
    template<typename T> void addOscArgs(ofxOscMessage &msg, T val);

    void oscSendChanges();
    void oscReceiveChanges();
    void oscReceiveTouchOscChanges(ofxOscMessage &msg);
    
    void oscReceiveProcessMessage(string address, ofxOscMessage &msg);
    void oscReceiveProcessTouchOscMessage(string address, ofxOscMessage &msg, int idx);

    map<string, TrackerBase* > outputTrackers;
    map<string, TrackerBase* > inputTrackers;
    ofxOscSender oscSender;
    ofxOscReceiver oscReceiver;
    string host;
    int portIn, portOut;
    bool sending, receiving;
};


