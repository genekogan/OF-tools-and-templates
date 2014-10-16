#pragma once

#include "ofMain.h"
#include "ofxAudioUnit.h"
#include "Theory.h"
#include "Sequencer.h"
#include "Control.h"
#include "MantaController.h"


class Aalto
{
public:
    enum NoteMode { MANUAL, AUTO };
    enum NoteType { NOTE_ON, NOTE_OFF, NOTE_AUTO };
    enum OutputMode { PARAMETERS, NOTES };
    
    void setup();
    void update();
    void draw();
    
    void noteEvent(NoteType type, int note, int velocity=127);
    void mantaPadEvent(ofxMantaEvent &e);
    void mantaPadVelocityEvent(ofxMantaEvent &e);
    void sequencerStep(vector<float> &column);
    
    void chooseNoteMode(string &s);
    void choosePadMode(string &s);
    void chooseSequencerMode(string &s);
    
    void printParameterList();
    
    void showUI() {aalto.showUI();}
    
    void savePreset(string filename="");
    void loadPreset(string &filename);

    void setupGui();
    void setupGuiPresets();


    Control control;
    OutputMode sequencerMode;
    OutputMode padMode;
    NoteMode noteMode;

    MantaController manta;
    Sequencer sequencer;
    Theory theory;

    ofxAudioUnitSampler aalto, a2;
    ofxAudioUnitMixer mixer;
	ofxAudioUnitOutput output;
    
    bool noteStatus[128];
    map<int, int> noteEvents;


    
    void addParameterMapping(int idx, int parameterId, float min, float max) {
       // mappings[idx] = ParameterMapping(parameterId, idx, min, max);;
    }
    
    struct ParameterMapping {
        string name;
        int parameterId;
        float min, max;
        float rmin, rmax;
    };
    
    map<string, vector<string> > parameterGroups;
    vector<string> parameterGroupNames;
    vector<ParameterMapping> parameters;
    map<int, ParameterMapping> mappings;

    
    
    
    ofxUICanvas *guiP;
    ofxUIDropDownList *guiParameterGroups, *guiParameters;
    ofxUIRangeSlider *guiParameterRange;
    int guiActivePad;
    bool guiToSwitchParameters;
    string guiToSwitchParametersName;
    
    void setGuiParameterGroup(string parameterGroupName) {
        guiParameters->clearToggles();
        guiParameters->addToggles(parameterGroups[parameterGroupName]);
        guiParameters->open();
    }
    
    void guiParametersEvent(ofxUIEventArgs &e) {
        if (e.getParentName() == "parameter groups") {
            if (parameterGroups.count(e.getName())>0) {
                guiToSwitchParametersName = e.getName();
                guiToSwitchParameters = true;
            }
        }
        else if (e.getParentName() == "parameters") {
            for (int i=0; i<parameters.size(); i++) {
                if (parameters[i].name == e.getName()) {
//                    guiParameterRange->setName(parameters[i].name);
//                    guiParameterRange->setMaxAndMin(parameters[i].max, parameters[i].min);
                    mappings[guiActivePad] = parameters[i];
                    manta.markPad(guiActivePad / 8, guiActivePad % 8, true);
                    //manta.setLabel(guiActivePad/8, guiActivePad%8, parameters[i].name);
                    setGui1();
                    return;
                }
            }
        }
        else if (e.getName() == "remove") {
            mappings.erase(guiActivePad);
            manta.markPad(guiActivePad / 8, guiActivePad % 8, false);
            setGui1();
        }
        else if (e.getName() == "range") {
            mappings[guiActivePad].rmin = guiParameterRange->getValueLow();
            mappings[guiActivePad].rmax = guiParameterRange->getValueHigh();
        }
    }
    
    void setGui1() {
        guiP->clearWidgets();
        if (mappings.count(guiActivePad) == 0) {
            vector<string> parameterNames;
            guiP->addLabel("add parameter map");
            guiParameterGroups = guiP->addDropDownList("parameter groups", parameterGroupNames);
            guiParameters = guiP->addDropDownList("parameters", parameterNames);
            guiParameterGroups->setAutoClose(true);
            guiParameters->setAutoClose(true);
        }
        else {
            guiP->addLabel("p("+ofToString(guiActivePad/8)+", "+ofToString(guiActivePad%8)+") :: "+mappings[guiActivePad].name);
            guiParameterRange = guiP->addRangeSlider("range",
                                                     mappings[guiActivePad].min,
                                                     mappings[guiActivePad].max,
                                                     0.2, 0.5);
            guiP->addLabelButton("remove", false);
        }
    }
    
    void padClickEvent(int & pad) {
        guiActivePad = pad;
        setGui1();
    }
    
    
    
    
    
    
    
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


};
