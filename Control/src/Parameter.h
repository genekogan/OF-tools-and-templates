#pragma once

//#include "Control.h"

class ParameterBase
{
public:
    enum Type { BOOL, INT, FLOAT, VEC2F, VEC3F, VEC4F, STRING };
    
    virtual ~ParameterBase() {}
    
    string getName() {return name; }
    string getOscAddress() {return oscAddress; }
    Type getType()   {return type;}

    void setName(string name) {this->name = name; }
    void setOscAddress(string oscAddress) {this->oscAddress = oscAddress; }
    template<class T> void setType();

    template<class T> void setReference(T *val);
    template<class T> void setMin(T min);
    template<class T> void setMax(T max);

    template<class T> T get();
    template<class T> T* getReference();
    template<class T> T getMin();
    template<class T> T getMax();

    void setWarp(float warp) {this->warp = warp;}
    float getWarp() {return warp;}
    
    string name;
    string oscAddress;
    Type type;
    float warp;
};


template <typename T>
class Parameter : public ParameterBase
{
public:
    ~Parameter() {
        //ofRemoveListener(ofEvents().update, this, &Parameter::update);
    }
    
    Parameter(string name, T &val) {
        this->name = name;
        this->oscAddress = "/"+name;
        setType<T>();
        value = &val;
    }
    
    Parameter(string name, T &val, T min, T max, float warp_=1.0) : minValue(min), maxValue(max) {
        this->name = name;
        this->oscAddress = "/"+name;
        setType<T>();
        value = &val;
        warp = warp_;
    }
    
    T get() {return *value;}
    T* getReference() {return value;}
    T getMin() {return minValue;}
    T getMax() {return maxValue;}
    
    void setReference(T *val) {value = val;}
    void set(const T& val) {*value=val;}
    void setMin(T min) {minValue=min;}
    void setMax(T max) {maxValue=max;}
    
    void lerpTo(T val, int nf) {
        if (nf == 0) {
            this->set(val);
            return;
        }
        endValue = val;
        startValue = *value;
        numFrames = nf;
        frame = 0;
        ofAddListener(ofEvents().update, this, &Parameter::update);
    }
    
    void update(ofEventArgs &data) {
        if (++frame <= numFrames) {
            float t = (float) frame / numFrames;
            this->set(startValue*(1.0f-t)+endValue*t);
        }
        else {
            ofRemoveListener(ofEvents().update, this, &Parameter::update);
        }
    }
    
protected:
    
    T *value;
    T minValue, maxValue;
    T startValue, endValue;
    int frame, numFrames;
};


template<class T> T ParameterBase::get() { return dynamic_cast<Parameter<T>&>(*this).get(); }
template<class T> T ParameterBase::getMin() { return dynamic_cast<Parameter<T>&>(*this).getMin(); }
template<class T> T ParameterBase::getMax() { return dynamic_cast<Parameter<T>&>(*this).getMax(); }
template<class T> T* ParameterBase::getReference() { return dynamic_cast<Parameter<T>&>(*this).getReference(); }
template<class T> void ParameterBase::setReference(T *val) { return dynamic_cast<Parameter<T>&>(*this).setReference(); }
template<class T> void ParameterBase::setMin(T min) { return dynamic_cast<Parameter<T>&>(*this).setMin(); }
template<class T> void ParameterBase::setMax(T max) { return dynamic_cast<Parameter<T>&>(*this).setMax(); }


template<class T> void ParameterBase::setType() { }
template<> inline void ParameterBase::setType<bool>() { type = BOOL; }
template<> inline void ParameterBase::setType<int>() { type = INT; }
template<> inline void ParameterBase::setType<float>() { type = FLOAT; }
template<> inline void ParameterBase::setType<ofVec2f>() { type = VEC2F; }
template<> inline void ParameterBase::setType<ofVec3f>() { type = VEC3F; }
template<> inline void ParameterBase::setType<ofVec4f>() { type = VEC4F; }
template<> inline void ParameterBase::setType<string>() { type = STRING; }


