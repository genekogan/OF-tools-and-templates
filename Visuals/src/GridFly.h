#include "ofMain.h"
#include "Scene.h"


class GridFly : public Scene
{
public:
    void setup();
    void update();
    void draw();
    
    
private:
    void addRow();
    
    ofMesh mesh;
    ofEasyCam cam;
    
    vector<ofVec3f> *vertices;
    float minY, dy, y, Z;
    ofPoint camPosition = ofPoint(0, 100);
    
    ofxParameter<int> nx;
    ofxParameter<int> ny;
    ofxParameter<ofVec2f> length;
    ofxParameter<float> margin;
    ofxParameter<ofVec2f> noiseFactor;
    ofxParameter<float> speed;
    ofxParameter<float> angle;
    ofxParameter<ofVec3f> axis;
    ofxParameter<ofColor> color;

};


/*
 - random shapes floating around with random data
 - 3d shapes / obj from thingiverse
*/