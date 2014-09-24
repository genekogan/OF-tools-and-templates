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
    
    int nx;
    int ny;
    ofVec2f length;
    float margin;
    ofVec2f noiseFactor;
    float speed;
    float angle;
    ofVec3f axis;
    ofColor color;

};


/*
 - random shapes floating around with random data
 - 3d shapes / obj from thingiverse
*/