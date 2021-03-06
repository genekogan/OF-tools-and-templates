#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxUI.h"
#include "Constants.h"



class Birl
{
public:
    void setup(int port);
    void update();
    void updateFakeData();
    void draw(int x, int y, int w, int h);
    
    void setKeysMax(int keysMax) {this->keysMax = keysMax;}
    void setKeysDiscreteThreshold(float keysDiscreteThreshold) {this->keysDiscreteThreshold = keysDiscreteThreshold;}
    void setEmbouchureMax(int embouchureMax) {this->embouchureMax = embouchureMax;}

    vector<float> & getKeys()         { return keys; }
    vector<float> & getKeysDiscrete() { return keysDiscrete; }
    vector<float> & getPressure()     { return pressure; }
    vector<float> & getEmbouchure()   { return embouchure; }
    vector<int>   & getButtons()      { return buttons; }

    bool getButtonsChanged();
    
private:

    bool oscConnected;

    ofxOscReceiver osc;
    
    vector<float> keys;
    vector<float> keysDiscrete;
    vector<float> pressure;
    vector<float> embouchure;
    vector<int> buttons;
    
    float maxbreathpos = 45000.0f;
    float maxbreathneg = 15000.0f;
    
    int keysMax, embouchureMax;
    float keysDiscreteThreshold;
    
    bool buttonsChanged;
};
