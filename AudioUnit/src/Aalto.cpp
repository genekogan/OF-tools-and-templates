#include "Aalto.h"

//-----------
void Aalto::setup(){
    aalto = ofxAudioUnitSampler('aumu','Aalt', 'MLbs');
    aalto.showUI();
    aalto.connectTo(mixer).connectTo(output);
    
    output.start();
    
    
    
    printList();
    
    
    


    setupGui();
    setupGuiPresets();
}

//-----------
void Aalto::update() {
    
    
    setKeyVoices(p0);
    setKeyMod(p1);
    setKeyBend(p2);
    setKeyGlide(p4);
    
    setSeqWave(p5);
    
    
    setSeqRate(p10);
    setSeqRatio(p11);
    setSeqSteps(p12);
    setSeqOffset(p13);
    
    setSeqRateP(p14);
    setSeqStepsP(p15);
    setSeqOffsetP(p16);

    
    setFilterCutoff(p20);
    setFilterQ(p21);
    setFilterMix(p22);
    setFilterCutoffP(p23);
    setFilterQP(p24);
    setFilterMixP(p25);

    
}

//-----------
void Aalto::setupGui() {
    control.setName("aalto");
    
    control.addParameter("keyVoices", &p0, 1, 4);
    control.addParameter("keyMod", &p1, 1, 127);
    control.addParameter("keyBend", &p2, 0, 24);
    control.addParameter("keyGlide", &p4, 0.005f, 5.0f);
    control.addParameter("seqWave", &p5, 0, 3);
    
    control.addParameter("seqRate", &p10, 0.001f, 13.75f);
    control.addParameter("seqRatio", &p11, 0.25f, 8.0f);
    control.addParameter("seqSteps", &p12, 0, 16);
    control.addParameter("seqOffset", &p13, 0, 15);

    control.addParameter("seqRateP", &p14, -4.0f, 4.0f);
    control.addParameter("seqStepsP", &p15, -15.0f, 15.0f);
    control.addParameter("seqOffsetP", &p16, -15.0f, 15.0f);

    control.addParameter("filterCutoff", &p20, 20.0f, 2000.0f);
    control.addParameter("filterQ", &p21, 0.00f, 1.0f);
    control.addParameter("filterMix", &p22, -1.0f, 1.0f);
    control.addParameter("filterCutoffP", &p23, -4.0f, 4.0f);
    control.addParameter("filterQP", &p24, -1.0f, 1.0f);
    control.addParameter("filterMixP", &p25, -1.0f, 1.0f);

}

//-----------
void Aalto::setupGuiPresets() {
    vector<string> presets;
    ofDirectory dir(ofToDataPath("presetsAudio/aalto/"));
    dir.allowExt("aupreset");
    dir.listDir();
    for(int i = 0; i < dir.numFiles(); i++) {
        presets.push_back(dir.getName(i));
    }
    control.addMenu("presets", presets, this, &Aalto::loadPreset);
}

//-----------
Aalto::~Aalto() {
    aalto.~ofxAudioUnitSampler();
}