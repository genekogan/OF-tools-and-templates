#pragma once

#include "ofMain.h"
#include "SuperCollider.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    
        void exit();

		void keyPressed(int key);
		
    SuperCollider sc3;

};
