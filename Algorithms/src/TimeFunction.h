#pragma once

#include "ofMain.h"
#include "Parameter.h"


template <typename T>
class TimeFunction
{
public:
    TimeFunction(bool autoUpdate) {
        setup(autoUpdate);
    }
    
    TimeFunction() {
        setup(true);
    }
    
    ~TimeFunction() {
        setAutoUpdate(false);
    }
    
    void setup(bool autoUpdate) {
        setAutoUpdate(autoUpdate);
        noiseOffset = ofVec3f(ofRandom(100), ofRandom(100), ofRandom(100));
        sinePhase = ofVec3f(ofRandom(TWO_PI), ofRandom(TWO_PI), ofRandom(TWO_PI));
        delTime = 1.0;
        reset();
    }
    
    void setAutoUpdate(bool autoUpdate) {
        this->autoUpdate = autoUpdate;
        if (autoUpdate) {
            ofAddListener(ofEvents().update, this, &TimeFunction::update);
        }
        else {
            ofRemoveListener(ofEvents().update, this, &TimeFunction::update);
        }
    }
    
    void reset();
    
    /* getters */
    ofxParameter<T> getConstant() { return constant; }
    ofxParameter<T> getTimeCoefficient() { return timeCoefficient; }
	ofxParameter<T> getRandMin() { return randMin; }
	ofxParameter<T> getRandMax() { return randMax; }
	ofxParameter<T> getNoiseMin() { return noiseMin; }
	ofxParameter<T> getNoiseMax() { return noiseMax; }
	ofxParameter<T> getNoiseSpeed() { return noiseSpeed; }
	ofxParameter<ofVec3f> getNoiseOffset() { return noiseOffset; }
	ofxParameter<T> getSineMin() { return sineMin; }
	ofxParameter<T> getSineMax() { return sineMax; }
	ofxParameter<T> getSineFreq() { return sineFreq; }
	ofxParameter<ofVec3f> getSinePhase() { return sinePhase; }
	ofxParameter<T> getDelTime() { return delTime; }
	
	/* reference getters */
    ofxParameter<T> & getConstantRef() { return constant; }
    ofxParameter<T> & getTimeCoefficientRef() { return timeCoefficient; }
    ofxParameter<T> & getRandMinRef() { return randMin; }
    ofxParameter<T> & getRandMaxRef() { return randMax; }
    ofxParameter<T> & getNoiseMinRef() { return noiseMin; }
    ofxParameter<T> & getNoiseMaxRef() { return noiseMax; }
    ofxParameter<T> & getNoiseSpeedRef() { return noiseSpeed; }
    ofxParameter<ofVec3f> & getNoiseOffsetRef() { return noiseOffset; }
    ofxParameter<T> & getSineMinRef() { return sineMin; }
    ofxParameter<T> & getSineMaxRef() { return sineMax; }
    ofxParameter<T> & getSineFreqRef() { return sineFreq; }
    ofxParameter<ofVec3f> & getSinePhaseRef() { return sinePhase; }
    ofxParameter<T> & getDelTimeRef() { return delTime; }
    
    /* setters */
	void setConstant(T z) { this->constant.set(z); }
	void setTimeCoefficient(T z) { this->timeCoefficient.set(z); }
	void setRandMin(T z) { this->randMin.set(z); }
	void setRandMax(T z) { this->randMax.set(z); }
	void setNoiseMin(T z) { this->noiseMin.set(z); }
	void setNoiseMax(T z) { this->noiseMax.set(z); }
	void setNoiseSpeed(T z) { this->noiseSpeed.set(z); }
	void setNoiseOffset(ofVec3f z) { this->noiseOffset.set(z); }
	void setSineMin(T z) { this->sineMin.set(z); }
	void setSineMax(T z) { this->sineMax.set(z); }
	void setSineFreq(T z) { this->sineFreq.set(z); }
	void setSinePhase(ofVec3f z) { this->sinePhase.set(z); }
	void setTime(T z) { this->time.set(z); }
	void setDelTime(float z) { this->delTime.set(z); }
    
    /* main function setters */
	void setFunctionRandom(T randMin, T randMax) {
	    this->randMin.set(randMin);
	    this->randMax.set(randMax);
	}
	void setFunctionNoise(T noiseMin, T noiseMax, T noiseSpeed, ofVec3f noiseOffset) {
	    this->noiseMin.set(noiseMin);
	    this->noiseMax.set(noiseMax);
	    this->noiseSpeed.set(noiseSpeed);
	    this->noiseOffset.set(noiseOffset);
	}
	void setFunctionNoise(T noiseMin, T noiseMax, T noiseSpeed) {
        setFunctionNoise(noiseMin, noiseMax, noiseSpeed,
                         ofVec3f(ofRandom(100), ofRandom(100), ofRandom(100)));
    }
	void setFunctionSine(T sineMin, T sineMax, T sineFreq, ofVec3f sinePhase) {
	    this->sineMin.set(sineMin);
	    this->sineMax.set(sineMax);
	    this->sineFreq.set(sineFreq);
	    this->sinePhase.set(sinePhase);
	}
    void setFunctionSine(T sineMin, T sineMax, T sineFreq) {
        setFunctionSine(sineMin, sineMax, sineFreq,
                        ofVec3f(ofRandom(TWO_PI), ofRandom(TWO_PI), ofRandom(TWO_PI)));
    }
    
    /* update time factors */
    void update() {
        timeFactor += (delTime * timeCoefficient);
        noiseFactor += (delTime * noiseSpeed);
        sineFactor += (delTime * sineFreq);
    }

    /* sample current value */
    ofxParameter<T> getRef() { return &value; }
    T get();
    
private:
    
    void update(ofEventArgs &data) {
        update();
    }
    
    ofxParameter<T> constant;
    ofxParameter<T> timeCoefficient;
    ofxParameter<T> randMin, randMax;
    ofxParameter<T> noiseMin, noiseMax, noiseSpeed;
    ofxParameter<T> sineMin, sineMax, sineFreq;
    ofxParameter<T> value;
 	ofxParameter<float> delTime;
    ofxParameter<ofVec3f> noiseOffset, sinePhase;
    
    T noiseFactor;
    T sineFactor;
    T timeFactor;
    
    bool autoUpdate;
};


template<> inline void TimeFunction<int>::reset() {
    setConstant(0);
    setTimeCoefficient(0);
    setFunctionNoise(0, 0, 0);
    setFunctionRandom(0, 0);
    setFunctionSine(0, 0, 0);
}
template<> inline void TimeFunction<float>::reset() {
    setConstant(0.0);
    setTimeCoefficient(0.0);
    setFunctionNoise(0.0, 0.0, 0.0);
    setFunctionRandom(0.0, 0.0);
    setFunctionSine(0.0, 0.0, 0.0);
}
template<> inline void TimeFunction<ofVec3f>::reset() {
    setConstant(ofVec3f(0, 0, 0));
    setTimeCoefficient(ofVec3f(0, 0, 0));
    setFunctionNoise(ofVec3f(0, 0, 0), ofVec3f(0, 0, 0), ofVec3f(0, 0, 0));
    setFunctionRandom(ofVec3f(0, 0, 0), ofVec3f(0, 0, 0));
    setFunctionSine(ofVec3f(0, 0, 0), ofVec3f(0, 0, 0), ofVec3f(0, 0, 0));
}
template<> inline void TimeFunction<ofVec2f>::reset() {
    setConstant(ofVec2f(0, 0));
    setTimeCoefficient(ofVec2f(0, 0));
    setFunctionNoise(ofVec2f(0, 0), ofVec2f(0, 0), ofVec2f(0, 0));
    setFunctionRandom(ofVec2f(0, 0), ofVec2f(0, 0));
    setFunctionSine(ofVec2f(0, 0), ofVec2f(0, 0), ofVec2f(0, 0));
}


template<> inline int TimeFunction<int>::get() {
    value.set(constant +
              timeFactor +
              ofRandom(randMin, randMax) +
              ofMap(ofNoise(noiseFactor, noiseOffset->x), 0, 1, noiseMin, noiseMax) +
              ofMap(sin(sineFactor + sinePhase->x), -1, 1, sineMin, sineMax) );
    return value;
}

template<> inline float TimeFunction<float>::get() {
    value.set(constant +
              timeFactor +
              ofRandom(randMin, randMax) +
              ofMap(ofNoise(noiseFactor, noiseOffset->x), 0, 1, noiseMin, noiseMax) +
              ofMap(sin(sineFactor + sinePhase->x), -1, 1, sineMin, sineMax) );
    return value;
}

template<> inline ofVec3f TimeFunction<ofVec3f>::get() {
    value.set(constant.get() +
              ofVec3f(timeFactor.x,
                      timeFactor.y,
                      timeFactor.z) +
              ofVec3f(ofRandom(randMin->x, randMax->x),
                      ofRandom(randMin->y, randMax->y),
                      ofRandom(randMin->z, randMax->z)) +
              ofVec3f(ofMap(ofNoise(noiseFactor.x, noiseOffset->x), 0, 1, noiseMin->x, noiseMax->x),
                      ofMap(ofNoise(noiseFactor.y, noiseOffset->y), 0, 1, noiseMin->y, noiseMax->y),
                      ofMap(ofNoise(noiseFactor.z, noiseOffset->z), 0, 1, noiseMin->z, noiseMax->z)) +
              ofVec3f(ofMap(sin(sineFactor.x + sinePhase->x), -1, 1, sineMin->x, sineMax->x),
                      ofMap(sin(sineFactor.y + sinePhase->y), -1, 1, sineMin->y, sineMax->y),
                      ofMap(sin(sineFactor.z + sinePhase->z), -1, 1, sineMin->z, sineMax->z)) );
    return value;
}

template<> inline ofVec2f TimeFunction<ofVec2f>::get() {
    value.set(constant.get() +
              ofVec2f(timeFactor.x,
                      timeFactor.y) +
              ofVec2f(ofRandom(randMin->x, randMax->x),
                      ofRandom(randMin->y, randMax->y))+
              ofVec2f(ofMap(ofNoise(noiseFactor.x, noiseOffset->x), 0, 1, noiseMin->x, noiseMax->x),
                      ofMap(ofNoise(noiseFactor.y, noiseOffset->y), 0, 1, noiseMin->y, noiseMax->y))+
              ofVec2f(ofMap(sin(sineFactor.x + sinePhase->x), -1, 1, sineMin->x, sineMax->x),
                      ofMap(sin(sineFactor.y + sinePhase->y), -1, 1, sineMin->y, sineMax->y)) );
    return value;
}
