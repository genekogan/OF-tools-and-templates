#include "ofMain.h"
#include "OpenNI.h"


class ofApp : public ofBaseApp
{
public:
	void setup();
    void exit();
    
	void update();
	void draw();
    
	void keyPressed(int key);
    
    OpenNi kinect;
};
