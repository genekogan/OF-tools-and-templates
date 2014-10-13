#pragma once

#include "ofMain.h"
#include "ofxAudioUnit.h"
#include "Sequencer.h"
#include "Control.h"
#include "MantaController.h"

class Aalto
{
public:
    void setup();

    void update();
    void draw();
    
    void noteOn(int note, int velocity) {
        aalto.midiNoteOn(note, velocity);
        noteStatus[note] = true;
    }

    void noteOff(int note, int velocity) {
        aalto.midiNoteOff(note, velocity);
        noteStatus[note] = false;
    }

    void noteEvent(int note, int velocity) {
        aalto.midiNoteOn(note, velocity);
        aalto.midiNoteOff(note, velocity);
    }

    void noteEvent2(int note, int velocity) {
        aalto.midiNoteOn(note, velocity);
        
        noteEvents[note] = ofGetFrameNum();
        
        //aalto.midiNoteOff(note, velocity);
    }

    
    void randomizeSequencer() {
        
//        for (int r=0; r<sequencer.)
      //  for (int row=0; row<seq->getRowCount(); row++) {
        //    for (int col=0; col<seq->getColumnCount(); col++) {
    //            seq->setToggle(row, col, ofRandom(1) < ofMap(ofGetMouseY(), 0, ofGetHeight(), 0, 1));
          //  }
//        }
    }
    
    map<int, int> noteEvents;
    
    
    void toggleNote(int note, int velocity) {
        if (noteStatus[note]) {
            aalto.midiNoteOff(note, velocity);
        }
        else {
            aalto.midiNoteOn(note, velocity);
        }
        noteStatus[note] = !noteStatus[note];
    }

    void printList() {

        vector<AudioUnitParameterInfo> params = aalto.getParameterList();
        for (int i=0; i<params.size(); i++) {
            cout << "void set"<<params[i].name<<"(float value) { aalto.setParameter("<<i<<", 0, value); }   //"<< params[i].minValue << "->"<<params[i].maxValue << endl;
        }
        
        
    }
    
    bool noteStatus[128];
    
    int col;
    
    int p0, p1, p2, p5, p12, p13;
    float p4, p10, p11, p14, p15, p16, p20,p21,p22,p23,p24,p25;
    
    void savePreset(string filename="") {
        if (filename=="") {
            filename = ofSystemTextBoxDialog("choose a preset name");
        }
        if (filename=="")   return;
        aalto.saveCustomPresetAtPath(ofToDataPath("presetsAudio/aalto/"+filename+".aupreset"));
        setupGuiPresets();
    }
    
    void loadPreset(string &filename) {
        aalto.loadCustomPresetAtPath(ofToDataPath("presetsAudio/aalto/"+filename));
    }
    
    void sequencerStep(vector<float> &column);

    void setKeyVoices(int v) { aalto.setParameter(0, 0, v); }   //1->4
    void setKeyMod(int v) { aalto.setParameter(1, 0, v); }   //1->127
    void setKeyBend(int v) { aalto.setParameter(2, 0, v); }   //0->24
    void setKeyUnison(bool v) { aalto.setParameter(3, 0, v?1:0); }   //0->1
    void setKeyGlide(float v) { aalto.setParameter(4, 0, v); }   //0.005->5
    
    void setSeqWave(int v) { aalto.setParameter(5, 0, v); }   //0->3  (???)
    void setSeqLocal(bool v) { aalto.setParameter(6, 0, v?1:0); }   //0->1   (???)
    
    void setSeqHost(bool v) { aalto.setParameter(7, 0, v?1:0); }   //0->1
    void setSeqLoop(bool v) { aalto.setParameter(8, 0, v?1:0); }   //0->1
    void setSeqRetrig(bool v) { aalto.setParameter(9, 0, v?1:0); }   //0->1
    
    void setSeqRate(float v) { aalto.setParameter(10, 0, v); }   //0.001->13.75
    void setSeqRatio(float v) { aalto.setParameter(11, 0, v); }   //0.25->8
    void setSeqSteps(int v) { aalto.setParameter(13, 0, v); }   //0->16
    void setSeqOffset(int v) { aalto.setParameter(15, 0, v); }   //0->15
    
    void setSeqRateP(float v) { aalto.setParameter(12, 0, v); }   //-4->4
    void setSeqStepsP(float v) { aalto.setParameter(14, 0, v); }   //-15->15
    void setSeqOffsetP(float v) { aalto.setParameter(16, 0, v); }   //-15->15
    
    void setSeqRange(int v) { aalto.setParameter(17, 0, v); }   //-48->48
    void setSeqQuantize(bool v) { aalto.setParameter(18, 0, v?1:0); }   //0->1
    void setSeqGlide(float v) { aalto.setParameter(19, 0, v); }   //0->1
    void setSeqPw(float v) { aalto.setParameter(20, 0, v); }   //0->100
    void setSeqVDelay(float v) { aalto.setParameter(21, 0, v); }   //0->8
    void setSeqPulseDelay(float v) { aalto.setParameter(22, 0, v); }   //0->8
    
    void setSeqValue(int idx, float v) { aalto.setParameter(23+idx, 0, v); }   //0->15, 0->1
    void setSeqPulse(int idx, float v) { aalto.setParameter(39+idx, 0, v); }   //0->15, 0->1
    
    void setLfoFreq(float v) { aalto.setParameter(55, 0, v); }   //0.01->110
    void setLfoNoise(float v) { aalto.setParameter(56, 0, v); }   //0->1
    void setLfoLevel(float v) { aalto.setParameter(57, 0, v); }   //0->1
    void setLfoLevelP(float v) { aalto.setParameter(58, 0, v); }   //-2->2
    void setLfoFreqP(float v) { aalto.setParameter(59, 0, v); }   //-4->4
    
    void setEnv1Attack(float v) { aalto.setParameter(60, 0, v); }   //0.001->20
    void setEnv1Decay(float v) { aalto.setParameter(61, 0, v); }   //0.01->20
    void setEnv1Sustain(float v) { aalto.setParameter(62, 0, v); }   //0->1
    void setEnv1Release(float v) { aalto.setParameter(63, 0, v); }   //0.01->20
    
    void setEnv1Level(float v) { aalto.setParameter(64, 0, v); }   //0->1
    void setEnv1Xvel(bool v) { aalto.setParameter(65, 0, v?1:0); }   //0->1
    void setEnv1TrigSelect(float v) { aalto.setParameter(66, 0, v); }   //1->2
    void setEnv1AttackP(float v) { aalto.setParameter(67, 0, v); }   //-1->1
    void setEnv1DecayP(float v) { aalto.setParameter(68, 0, v); }   //-1->1
    void setEnv1ReleaseP(float v) { aalto.setParameter(69, 0, v); }   //-1->1
    
    void setEnv2Delay(float v) { aalto.setParameter(71, 0, v); }   //0->1
    void setEnv2Attack(float v) { aalto.setParameter(72, 0, v); }   //0.001->20
    void setEnv2Sustain(float v) { aalto.setParameter(73, 0, v); }   //0->1
    void setEnv2Release(float v) { aalto.setParameter(74, 0, v); }   //0.01->20
    
    void setEnv2Xvel(bool v) { aalto.setParameter(75, 0, v?1:0); }   //0->1
    void setEnv2TrigSelect(float v) { aalto.setParameter(76, 0, v); }   //1->2
    void setEnv2Xenv1(float v) { aalto.setParameter(77, 0, v); }   //1->2
    void setEnv2DelayP(float v) { aalto.setParameter(78, 0, v); }   //-1->1
    void setEnv2AttackP(float v) { aalto.setParameter(79, 0, v); }   //-1->1
    void setEnv2Repeat(float v) { aalto.setParameter(70, 0, v); }   //0.429688->220
    void setEnv2RepeatP(float v) { aalto.setParameter(80, 0, v); }   //-4->4
    
    void setOscNoise(float v) { aalto.setParameter(81, 0, v); }   //0->1
    void setOscRatio(float v) { aalto.setParameter(82, 0, v); }   //0->16
    void setOscOffset(int v) { aalto.setParameter(83, 0, v); }   //-200->200
    void setOscIndex(float v) { aalto.setParameter(84, 0, v); }   //0->11
    void setOscTimbre(float v) { aalto.setParameter(85, 0, v); }   //0->1
    void setOscPitch(float v) { aalto.setParameter(86, 0, v); }   //27.5->3520
    void setOscWaveshape(float v) { aalto.setParameter(87, 0, v); }   //-0.9->0.9
    
    void setOscNoiseP(float v) { aalto.setParameter(88, 0, v); }   //-1->1
    void setOscRatioP(float v) { aalto.setParameter(89, 0, v); }   //-4->4
    void setOscOffsetP(float v) { aalto.setParameter(90, 0, v); }   //-200->200
    void setOscIndexP(float v) { aalto.setParameter(91, 0, v); }   //-10->10
    void setOscTimbreP(float v) { aalto.setParameter(92, 0, v); }   //-1->1
    void setOscPitchExpP(float v) { aalto.setParameter(93, 0, v); }   //-4->4
    void setOscPitchLinP(float v) { aalto.setParameter(94, 0, v); }   //4->20000
    void setOscWaveshapeP(float v) { aalto.setParameter(95, 0, v); }   //-1->1
    void setOscCarrierOut(float v) { aalto.setParameter(96, 0, v); }   //0->1
    void setOscModOut(float v) { aalto.setParameter(97, 0, v); }   //0->1
    
    void setGateLevel(float v) { aalto.setParameter(98, 0, v); }   //0->1
    void setGateMode(bool v) { aalto.setParameter(99, 0, v?1:0); }   //0->1
    void setGateDecay(float v) { aalto.setParameter(100, 0, v); }   //0->2
    void setGateLevelP(float v) { aalto.setParameter(101, 0, v); }   //0->1
    
    void setDelayInput(float v) { aalto.setParameter(102, 0, v); }   //0->1
    void setDelayPeakfreq(int v) { aalto.setParameter(103, 0, v); }   //100->10000
    void setDelayDrive(float v) { aalto.setParameter(104, 0, v); }   //0->1
    void setDelayPeakres(float v) { aalto.setParameter(105, 0, v); }   //-6->6
    void setDelayFeedback(float v) { aalto.setParameter(106, 0, v); }   //0->1.05
    void setDelayFreq(float v) { aalto.setParameter(107, 0, v); }   //0.859375->3520
    
    void setDelayInputP(float v) { aalto.setParameter(108, 0, v); }   //-1->1
    void setDelayPeakfreqP(float v) { aalto.setParameter(109, 0, v); }   //-4->4
    void setDelayDriveP(float v) { aalto.setParameter(110, 0, v); }   //-1->1
    void setDelayPeakresP(float v) { aalto.setParameter(111, 0, v); }   //-4->4
    void setDelayFeedbackP(float v) { aalto.setParameter(112, 0, v); }   //-1->1
    void setDelayFreqP(float v) { aalto.setParameter(113, 0, v); }   //-4->4
    void setDelayOutputWet(float v) { aalto.setParameter(114, 0, v); }   //0->4
    void setDelayOutputDry(float v) { aalto.setParameter(115, 0, v); }   //0->2
    
    void setFilterCutoff(float v) { aalto.setParameter(116, 0, v); }   //20->20000
    void setFilterQ(float v) { aalto.setParameter(117, 0, v); }   //0->1
    void setFilterMix(float v) { aalto.setParameter(118, 0, v); }   //-1->1
    
    void setFilterCutoffP(float v) { aalto.setParameter(119, 0, v); }   //-4->4
    void setFilterQP(float v) { aalto.setParameter(120, 0, v); }   //-1->1
    void setFilterMixP(float v) { aalto.setParameter(121, 0, v); }   //-1->1
    
    void setOutputInputGain(float v) { aalto.setParameter(122, 0, v); }   //0->2
    void setOutputReverb(float v) { aalto.setParameter(123, 0, v); }   //0->1
    void setOutputReverbP(float v) { aalto.setParameter(125, 0, v); }   //-1->1
    void setOutputPan(float v) { aalto.setParameter(124, 0, v); }   //-1->1
    void setOutputPanP(float v) { aalto.setParameter(126, 0, v); }   //-1->1

    
    
protected:
    
    void setupGui();
    void setupGuiPresets();
    
    Control control;
    
    Sequencer sequencer;
    
    MantaController manta;
    
    

    ofxAudioUnitSampler aalto;
    ofxAudioUnitMixer mixer;
	ofxAudioUnitOutput output;

};
