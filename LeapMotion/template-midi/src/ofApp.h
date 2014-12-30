#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include "LeapMotion.h"



class ofApp : public ofBaseApp {
public:
    ~ofApp();
	void setup();
	void update();
	void draw();
	void exit();
	
	void keyPressed (int key);
	void keyReleased (int key);
	
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased();
	
	ofxMidiOut midiOut;
	int channel;
	
	unsigned int currentPgm;
	int note, velocity;
	int pan, bend, touch, polytouch;
    

    LeapMotion leap;
    
};
