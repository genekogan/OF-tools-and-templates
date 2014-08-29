#pragma once

#include "ofMain.h"


template <typename ParameterType>
class ofxParameter : public ofParameter<ParameterType>
{
public:
    using ofParameter<ParameterType>::operator=;
    
    ofxParameter<ParameterType>() {
        ofAddListener(ofEvents().update, this, &ofxParameter::update);
        frame = 1.0;
        numFrames = 0;
    }
    
    ~ofxParameter() {
        ofRemoveListener(ofEvents().update, this, &ofxParameter::update);
    }
    
    void lerpTo(ParameterType goToValue, int numFrames) {
        setInterpolation(goToValue, numFrames, LERP);
    }
    
    void easeTo(ParameterType goToValue, int numFrames, float easeCoefficient=0.65) {
        this->easeCoefficient = easeCoefficient;
        setInterpolation(goToValue, numFrames, EASE);
    }
    
private:
    
    enum LerpType { LERP, EASE };
    
    void setInterpolation(ParameterType endValue, int numFrames, LerpType lerpType) {
        this->endValue = endValue;
        this->numFrames = numFrames;
        this->lerpType = lerpType;
        startValue = this->get();
        frame = 0.0;
    }
    
    void update(ofEventArgs &data) {
        //cout << "update parameter :: " << this->getName() << endl;
        if (frame++ <= numFrames) {
            float t = lerpType==LERP ? frame/numFrames : pow(frame/numFrames, easeCoefficient);
            this->set(startValue*(1.0f-t)+endValue*t);
        }
    }
    
    ParameterType startValue, endValue;
    float frame;
    int numFrames;
    LerpType lerpType;
    float easeCoefficient;
};
