#include "Player.h"


//-----------
void Player::setup() {
    player.setFile("/Users/Gene/audio/my sounds/meditation.wav");
    player.setFile("/Users/Gene/Desktop/test_wav.wav");
//    player.setFile("/Users/Gene/Downloads/172667__underlineddesigns__military-rotor-loop.wav");
    player.loop();
    
    
    player.showUI();
    
    player.prime();
    
    
    
    fbo.allocate(400, 200);
    fbo.begin();
    ofClear(0, 0);
    ofSetColor(0);
    ofFill();
    ofRect(0, 0, 400, 200);
    fbo.end();
}


//-----------
void Player::connectTo(ofxAudioUnitMixer &mixer, int channel) {
    player.connectTo(tap);
    tap.connectTo(mixer, channel);
    
    

}


//-----------
void Player::update() {
    t = (int) player.getCurrentTimestamp().mSampleTime % player.getLength();


    
    if (ofGetFrameNum() % 4 == 0) {
        blah();
    }
    
    /*
    ofBeginShape();
    ofNoFill();
    ofSetColor(0);
    for (int i=0; i<data.size(); i++) {
        int x = ofMap(i, 0, data.size(), 100, 500);
        int y = ofMap(data[i], -1, 1, 500, 700);
        ofVertex(x, y);
    }
    ofEndShape();
     */

}

//-----------
void Player::draw() {

    tap.getLeftWaveform(wave, ofGetWidth(), ofGetHeight());
    wave.draw();

    
    float r =  (float) t / player.getLength();
    
    
    tap.getLeftSamples(data);
    
    float rms = pow(0.5 + (tap.getLeftChannelRMS() + tap.getRightChannelRMS()), 2.0);
    
    
    fbo.begin();
    int x = ofMap(r, 0, 1, 0, fbo.getWidth());
    int y = ofMap(rms, 0, 1, 0, 0.5 * fbo.getHeight());
    ofSetColor(0, 255, 0, 100);
    ofLine(x, fbo.getHeight()*0.5 - y, x, fbo.getHeight() * 0.5 + y);
    ofSetColor(255);
    fbo.end();
    
    fbo.draw(200, 500);


    
}
