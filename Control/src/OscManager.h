#pragma once

#include "Control.h"


class OscManager
{
public:
    ~OscManager();
    
    bool setupSender(string host, int portOut);
    bool setupReceiver(int portIn);
    
    void update();
    
    string getHost() {return host;}
    int getSenderPort() {return portOut;}
    int getReceiverPort() {return portIn;}
    
    bool getSending() {return sending;}
    bool getReceiving() {return receiving;}
    
    void registerToOscSender(vector<ParameterBase *> &parameters);
    void registerToOscReceiver(vector<ParameterBase *> &parameters);
    void registerToOsc(ParameterBase *parameter, bool send);
    
    template<typename ListenerClass, typename ListenerMethod>
    void registerOscEventListener(string address, ListenerClass *listener, ListenerMethod method) {
        eventTrackers[address] = new ofEvent<bool>();
        ofAddListener(*eventTrackers[address], listener, method);
    }
    
    void sendMessageManually(string address, vector<int> val);
    void sendMessageManually(string address, vector<float> val);
    
    ofxTouchOscPage * makeTouchOscPage(string name, vector<ParameterBase *> &parameters);
    void saveTouchOscLayout(string name, vector<ParameterBase *> &parameters);
    
    void clearInputTrackers();
    void clearOutputTrackers();
    void clearEventTrackers();
    
    void listTrackedParameters();
    
protected:
    
    struct TrackerBase;
    template <typename T> struct Tracker;
    
    template <typename T>
    void registerParameterToOsc(ParameterBase *parameter, bool send);
    
    template <typename T>
    void checkIfToSendOscMessage(map<string, TrackerBase*>::iterator &it);
    
    template<typename T>
    void addOscArgs(ofxOscMessage &msg, T val);
    
    void oscSendChanges();
    void oscReceiveChanges();
    void oscReceiveTouchOscChanges(ofxOscMessage &msg);
    
    void oscReceiveProcessMessage(string address, ofxOscMessage &msg);
    void oscReceiveProcessTouchOscMessage(string address, ofxOscMessage &msg, int idx);
    
    map<string, TrackerBase* > outputTrackers;
    map<string, TrackerBase* > inputTrackers;
    map<string, ofEvent<bool>* > eventTrackers;
    ofxOscSender oscSender;
    ofxOscReceiver oscReceiver;
    string host;
    int portIn, portOut;
    bool sending, receiving;
};


//---------
struct OscManager::TrackerBase {
    ParameterBase *parameter;
    template<class T> T getValue() {
        return ((Parameter<T> *) parameter)->get();
    }
    ParameterBase::Type getType() {
        return parameter->getType();
    }
};

template <typename T>
struct OscManager::Tracker : public TrackerBase {
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