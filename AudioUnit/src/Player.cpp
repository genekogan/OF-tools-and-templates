#include "Player.h"


//-----------
void Player::setup() {
    ofAddListener(waveform.selectionEvent, this, &Player::waveformSelectionEvent);
    ofAddListener(waveform.selectEvent, this, &Player::waveformSelectEvent);
    
    control.setName("AudioPlayer");
    vector<string> items;
    items.push_back("/Users/Gene/Desktop/test_wav.wav");
    items.push_back("/Users/Gene/Desktop/helloworld.wav");
    items.push_back("/Users/Gene/audio/my sounds/meditation.wav");
    
    control.addMenu("select clip", items, this, &Player::selectClip);
}

//-----------
void Player::connectTo(ofxAudioUnitMixer &mixer, int channel) {
    player.connectTo(tap);
    tap.connectTo(mixer, channel);
    //mixer.setInputVolume(0, channel);
}

//-----------
void Player::selectClip(string &path) {
    player.setFile(path);
    waveform.clearSelections();
    waveform.load(path); // supports mono or stereo .wav files
	waveform.generateWaveForm();
    setSelection(0, 1);
    player.loop();
}

//-----------
void Player::waveformSelectionEvent(ofPoint &e) {
    if (ofGetKeyPressed()) {
        addSelection(e.x, e.y);
    }
    else {
        setSelection(e.x, e.y);
    }
}

//-----------
void Player::waveformSelectEvent(ofPoint &e) {
    //setSelection(e.x, e.y);

    this->t1 = min(e.x, e.y);
    this->t2 = max(e.x, e.y);
    waveform.setSelection(t1, t2);
    sample1 = t1 * player.getLength();
    sample2 = t2 * player.getLength();
    numSamples = sample2 - sample1;
    player.playAtSampleTime(sample1);
}

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

void Player::addSelection(float _t1, float _t2) {
    waveform.addSelection(_t1, _t2);
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
    

    //int cursor = ofMap(currentSample, 0, numSamples, 0, width);
    int cursor = ofMap(currentSample, 0, numSamples, x + width * t1, x + width * t2);
    
    ofPushMatrix();
    ofPushStyle();

    waveform.drawWaveForm(x, y, width, height, amp);

    ofSetColor(0, 255, 0);
    ofLine(cursor, y-height/2, cursor, y+1.5*height);
    
    ofPopStyle();
    ofPopMatrix();
}
