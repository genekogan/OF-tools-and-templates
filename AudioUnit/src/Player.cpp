#include "Player.h"


//-----------
void Player::setSelection(float _t1, float _t2) {
    this->t1 = min(_t1, _t2);
    this->t2 = max(_t1, _t2);
    waveform.setSelection(t1, t2);
    sample1 = t1 * player.getLength();
    sample2 = t2 * player.getLength();
    numSamples = sample2 - sample1;
    player.playAtSampleTime(sample1);
}

//-----------
void Player::setup() {
    string path = "/Users/Gene/Desktop/test_wav.wav";
    //string path = "/Users/Gene/Desktop/helloworld.wav";
    //string path = "/Users/Gene/audio/my sounds/meditation.wav";

    ofAddListener(waveform.selectionEvent, this, &Player::waveformSelectionEvent);
    
    player.setFile(path);
    player.loop();

    waveform.load(path); // supports mono or stereo .wav files
	waveform.generateWaveForm();
    setSelection(0, 1);
}

//-----------
void Player::connectTo(ofxAudioUnitMixer &mixer, int channel) {
    player.connectTo(tap);
    tap.connectTo(mixer, channel);
    mixer.setInputVolume(0, channel);

}

//-----------
void Player::waveformSelectionEvent(ofPoint &e) {
    setSelection(e.x, e.y);
}

//-----------
void Player::update() {
    currentSample = player.getCurrentTimestamp().mSampleTime;
    if (currentSample >= numSamples) {
        player.playAtSampleTime(sample1);
    }
}

//-----------
void Player::draw() {
    int x = 5;
    int y = 500;

    int width = ofGetWidth()-410;
    int height = 100;
    float amp = 2.0; // 1.0 + 3.0* (float) ofGetMouseY() / ofGetHeight();
    

    int cursor = ofMap(currentSample, 0, numSamples, x, x+width);

    
    ofPushMatrix();
    ofPushStyle();

    waveform.drawWaveForm(x, y, width, height, amp);

    ofSetColor(0, 255, 0);
    ofLine(cursor, y-height/2, cursor, y+1.5*height);
    
    ofPopStyle();
    ofPopMatrix();
}
