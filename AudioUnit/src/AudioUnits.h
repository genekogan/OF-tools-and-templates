#pragma once

#include "ofMain.h"
#include "Instrument.h"
#include "FilePlayer.h"
#include "ofxAudioUnit.h"


class AudioUnits {
public:
    AudioUnits();
    void update();
    void draw();
    
    void setupInstrument(Instrument::InstrumentType type);
    void setupFilePlayer();
    
    void start() {output.start();}
    void stop() {output.stop();}
    
    void savePreset();

    void setGuiPosition(int x, int y);
    void setVisible(bool visible);
    void toggleVisible() {setVisible(!visible);}

    Instrument & getInstrument() { return instrument; }

protected:
    
    Instrument instrument;
    FilePlayer player;
    
    ofxAudioUnitMixer mixer;
	ofxAudioUnitOutput output;
    
    bool instrumentSetup, filePlayerSetup;

    int idxMixer;

    bool visible;
};