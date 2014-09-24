#pragma once

#include "ofMain.h"


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

    template<class T> void setMin(T min);
    template<class T> void setMax(T max);

    template<class T> T* getReference();
    template<class T> T getMin();
    template<class T> T getMax();

    string name;
    string oscAddress;
    Type type;
};


template <typename T>
class Parameter : public ParameterBase
{
public:
    Parameter(string name, T *val) {
        this->name = name;
        this->oscAddress = "/"+name;
        setType<T>();
        value = val;
    }
    
    Parameter(string name, T *val, T min, T max) : minValue(min), maxValue(max) {
        this->name = name;
        this->oscAddress = "/"+name;
        setType<T>();
        value = val;
    }
    
    const T get() const {return *value;}
    T* getReference() {return value;}
    T getMin() {return minValue;}
    T getMax() {return maxValue;}
    
    void set(const T& rhs) {*value=rhs;}
    void setMin(T min) {minValue=min;}
    void setMax(T max) {maxValue=max;}
    
private:
    T *value;
    T minValue, maxValue;
};


template<class T> T ParameterBase::getMin()
{ return dynamic_cast<Parameter<T>&>(*this).getMin(); }
template<class T> T ParameterBase::getMax()
{ return dynamic_cast<Parameter<T>&>(*this).getMax(); }
template<class T> T* ParameterBase::getReference()
{ return dynamic_cast<Parameter<T>&>(*this).getReference(); }
template<class T> void ParameterBase::setMin(T min)
{ return dynamic_cast<Parameter<T>&>(*this).setMin(); }
template<class T> void ParameterBase::setMax(T max)
{ return dynamic_cast<Parameter<T>&>(*this).setMax(); }


template<class T> void ParameterBase::setType() { }
template<> inline void ParameterBase::setType<bool>() { type = BOOL; }
template<> inline void ParameterBase::setType<int>() { type = INT; }
template<> inline void ParameterBase::setType<float>() { type = FLOAT; }
template<> inline void ParameterBase::setType<ofVec2f>() { type = VEC2F; }
template<> inline void ParameterBase::setType<ofVec3f>() { type = VEC3F; }
template<> inline void ParameterBase::setType<ofVec4f>() { type = VEC4F; }
template<> inline void ParameterBase::setType<string>() { type = STRING; }


