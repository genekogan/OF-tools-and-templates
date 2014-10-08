#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxTouchOsc.h"
#include "Parameter.h"



class OscManager
{
public:
    void blah();
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

    void clearInputTrackers();
    void clearOutputTrackers();
    
    void saveTouchOscLayout(string name, vector<ParameterBase *> &parameters);

protected:
    
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
    void registerParameterToOsc(ParameterBase *parameter, bool send) {

        if (send) {
            if (outputTrackers.count(parameter->getOscAddress()) == 0) {
                cout << "ADD SEND " << parameter->getOscAddress() << " send " << send << endl;
                outputTrackers[parameter->getOscAddress()] = new Tracker<T>(parameter);
            }
            else {
                ofLog(OF_LOG_WARNING, "Warning: parameter "+parameter->getName()+" already registered to output OSC");
            }
        }
        else {
            if (inputTrackers.count(parameter->getOscAddress()) == 0) {
                cout << "ADD RCV " << parameter->getOscAddress() << " send " << send << endl;
                inputTrackers[parameter->getOscAddress()] = new Tracker<T>(parameter);
            }
            else {
                ofLog(OF_LOG_WARNING, "Warning: parameter "+parameter->getName()+" already registered to input OSC");
            }
        }
    }

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


