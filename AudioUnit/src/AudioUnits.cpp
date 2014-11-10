#include "AudioUnits.h"


//-------
AudioUnits::AudioUnits() {
    mixer.connectTo(output);
    idxMixer = 0;
    instrumentSetup = false;
    filePlayerSetup = false;
    visible = true;
}

//-------
void AudioUnits::setupInstrument(Instrument::InstrumentType type) {
    instrument.setup(type);
    instrument.connectTo(mixer, idxMixer);
    idxMixer++;
    instrumentSetup = true;
}

//-------
void AudioUnits::setupFilePlayer() {
    player.setup();
    player.connectTo(mixer, idxMixer);
    player.setGuiPosition(5, 500);
    idxMixer++;
    filePlayerSetup = true;
}

//-------
void AudioUnits::update() {
    if (instrumentSetup) {
        instrument.update();
    }
    if (filePlayerSetup) {
        player.update();
    }
}

//-------
void AudioUnits::draw() {
    if (visible) {
        if (instrumentSetup) {
            instrument.draw();
        }
        if (filePlayerSetup) {
            player.draw();
        }
    }
}

//-------
void AudioUnits::setVisible(bool visible) {
    this->visible = visible;
    if (instrumentSetup) {
        player.setVisible(visible);
    }
    if (filePlayerSetup) {
        instrument.setVisible(visible);
    }
}

//-------
void AudioUnits::savePreset() {
    if (instrumentSetup) {
        instrument.savePreset();
    }
}