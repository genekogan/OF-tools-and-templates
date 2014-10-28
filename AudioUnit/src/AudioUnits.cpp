#include "AudioUnits.h"


void AudioUnits::setup() {
//    aalto.setup(AALTO);
    
    player.setup();
    
    player.connectTo(mixer, 0);
    //aalto.connectTo(mixer);
    
    mixer.connectTo(output);
    output.start();
    
}

void AudioUnits::update() {
    //aalto.update();
    player.update();
    
    //cout << (int) player.getCurrentTimestamp().mSampleTime % (int)  player.getLength() << endl;
}

void AudioUnits::draw() {
    //aalto.draw();
    player.draw();
}

void AudioUnits::savePreset() {
    //aalto.savePreset();
}