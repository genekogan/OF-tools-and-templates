
enum LerpType { LERP, EASE };


class ParameterBase
{
public:
    virtual ~ParameterBase() {}
    template<class T> const T& get() const;
    template<class T> const T& getRef() const;
    template<class T> const string getName() const;
    template<class T, class U> void set(const U& rhs);
    template<class T> void lerpTo(T rhs, int numFrames);
    template<class T> void easeTo(T rhs, int numFrames);
    virtual void setEasingCoefficient(float e) {};
    virtual void update() {};
};


template <typename T>
class Parameter : public ParameterBase
{
public:
    Parameter(ofParameter<T> *rhs) : value(rhs) { }
    Parameter(ofParameter<T> *rhs, T min, T max) : value(rhs), minValue(min), maxValue(max) { }

    const T& get() const {return *value;}
    const T& getRef() const {return value;}
    const string getName() const {return value->getName();}
    
    void set(const T& rhs) {*value=rhs;}
    void setEasingCoefficient(float e) {easeCoeff = e;}
    
    void lerpTo(T rhs, int numFrames) {
        setLerp(rhs, numFrames, LERP);
    }
    
    void easeTo(T rhs, int numFrames) {
        setLerp(rhs, numFrames, EASE);
    }
    
    void setLerp(T rhs, int numFrames, LerpType lerpType) {
        this->lerpType = lerpType;
        this->numFrames = numFrames;
        frame = 0;
        startValue = *value;
        endValue = rhs;
    }

    
private:
    
    void update() {
        if (frame++ < numFrames) {
            if (lerpType == LERP) {
                float t = (float) frame / (numFrames-1);
                *value = startValue * (1.0f-t) + endValue * t;
            }
            else {
                float t = pow((float) frame / (numFrames-1), easeCoeff);
                *value = startValue * (1.0f-t) + endValue * t;
            }
        }
    }
    
    ofParameter<T> *value;
    T minValue, maxValue;
    
    T startValue, endValue;
    int frame = 0;
    int numFrames = 0;
    
    LerpType lerpType;
    float easeCoeff = 0.65;
};


// dynamically cast base class templates

template<class T> const T& ParameterBase::get() const
{ return dynamic_cast<const Parameter<T>&>(*this).get(); }

template<class T> const T& ParameterBase::getRef() const
{ return dynamic_cast<const Parameter<T>&>(*this).getRef(); }

template<class T, class U> void ParameterBase::set(const U& rhs)
{ return dynamic_cast<Parameter<T>&>(*this).set(rhs); }

template<class T> void ParameterBase::lerpTo(T rhs, int numFrames)
{ return dynamic_cast<Parameter<T>&>(*this).lerpTo(rhs, numFrames); }

template<class T> void ParameterBase::easeTo(T rhs, int numFrames)
{ return dynamic_cast<Parameter<T>&>(*this).easeTo(rhs, numFrames); }
