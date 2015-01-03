#pragma once

#include "ofMain.h"
#include "Kinect.h"
#include "ofxSecondWindow.h"
#include "ofxOsc.h"
#include "ProjectionMapping.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    Kinect kinect;
    
    vector<ofFbo> fbo;
    int idx;
    
    bool recording;
    
    int numTotalFrames = 360;
    int numFramesSkip = 6;
    int margin = 40;
    

    ofxOscReceiver osc;
    
    ofxSecondWindow window;
    ProjectionMapping mapping;
  
    Control control;
};
