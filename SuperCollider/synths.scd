(
//==========================================
//  INPUTS
//==========================================


/*
@synthDef source
@name sourceBuffer
@buffer buf
@param reset 0 1
@param rate 0.5 1.5
*/
SynthDef(\sourceBuffer, { arg busOut, buf, reset, rate;
	var trig, nf, idx, out;
	nf = BufFrames.kr(buf);
	//trig = In.kr(bus_trig);
	//idx = Phasor.ar(trig, BufRateScale.kr(buf), 0, nf, reset * nf);
	idx = Phasor.ar(1, rate * BufRateScale.kr(buf), 0, nf, reset * nf);
	out = BufRd.ar(1, buf, idx);
	//Out.kr(bus_cursor, idx);
	Out.ar(busOut, out);
}).store;

/*
@synthDef source
@name playBuf
@buffer buf
@param rate 0.5 1.5
@param start 0 1
@param time 0 10
*/
SynthDef(\playBuf, { arg busOut, buf, rate, start, time;
	var out, env, startPos;
	startPos = start * BufSampleRate.kr(buf) * BufDur.kr(buf);
	env = EnvGen.kr(Env([0, 1, 1, 0], [0.1, time, 0.1]), 1, doneAction: 2);
	out = env * PlayBuf.ar(1, buf, rate, 1, startPos);
	Out.ar(busOut, out);
}).store;

/*
@synthDef source
@name audioIn
*/
SynthDef(\audioIn, { arg busOut;
	var out = SoundIn.ar(0);
	Out.ar(busOut, out);
}).store;


/*
@synthDef source
@name pickler
@param pulserate 1 30
@param ambience 0.001 0.99
@param numharmonics 1 16
*/
SynthDef(\pickler, { arg busOut, pulserate, numharmonics, ambience;
	var trig, pitches, freq, idxfreq, out, out2, delay;
	pitches = -12 + [ 28, 36, 40, 47, 60, 64, 67, 71, 72, 76, 79, 83, 84 ];
	trig = Impulse.kr(pulserate);                 // trigger rate (5 hz)
	idxfreq = TIRand.kr(0, 13, trig);
	freq = Select.kr(idxfreq, pitches).midicps;
	out = Blip.ar(                        // Generate blip
		freq,
		TRand.kr(1, numharmonics, trig),             // Num harmonics ranges from 1 to 12
		max(0, TRand.kr(-0.5, 0.4, trig))  // amplitude ranges from 0 to 0.4
	);
	out = Pan2.ar(out, TRand.kr(-1.0, 1.0, trig));   // Pan randomly
	out = out*EnvGen.kr(Env.perc(0, 1), trig);       // apply envelope to it
	out = Mix.ar({out}.dup(6))*0.2;
	delay = CombL.ar(out, 1.0, 4/6, 6);              // create comb filter delay
	out = out + delay;                                   // play out + its delay from combL
	16.do({ out = AllpassC.ar(out, 1.0, DC.kr(Rand()).range(0.001, ambience), 8)});
	Out.ar(busOut, out);
}).store;


/*
@synthDef source
@name sahliquid
@param atk 1 9
@param ctrFreq 0 1
@param clockRate 0 300
*/
SynthDef(\sahliquid, { arg busOut, atk, ctrFreq, clockRate;
	var env, clockTime, clock, centerFreq, freq, panPos, patch;
	env = EnvGen.kr(Env.adsr(atk, 1, 1), 1, doneAction: 2);
	centerFreq = 100 + 8000 * pow(ctrFreq, 2);
	clockTime = 2.0*clockRate.reciprocal;
	clock = Impulse.kr(clockRate, 0.4);
	freq = Latch.kr(WhiteNoise.kr(centerFreq * 0.5, centerFreq), clock);
	panPos = Latch.kr(WhiteNoise.kr, clock);
	patch = CombN.ar(
		Pan2.ar(
			SinOsc.ar(freq, 0, Decay2.kr(clock, 0.1 * clockTime, 0.9 * clockTime)),
			panPos
		), 0.3, 0.3, 2);
	Out.ar(busOut, env * patch);
}).store;


/*
@synthDef source
@name pluck1
@param pluck 0 1
*/
SynthDef(\pluck1, { arg busOut, pluck;
	var pitch, out;
	pitch = [ 52, 57, 62, 67, 71, 76 ];   // e a d g b e
	out = Mix.arFill(pitch.size, { arg i;
		var pos, trigger1, trigger2, pluck1, pluck2, period, string1, string2;
		// place trigger points from 0.25 to 0.75
		pos = 0.25 + (i * 0.1);
		period = pitch.at(i).midicps.reciprocal;

		trigger1 = HPZ1.kr(pluck > pos).abs;
		pluck1 = PinkNoise.ar(Decay.kr(trigger1, 0.05));
		string1 = CombL.ar(pluck1, period, period, 4);

		trigger2 = HPZ1.kr(pluck > (pos + 0.015)).abs;
		pluck2 = PinkNoise.ar(Decay.kr(trigger2, 0.05));
		string2 = CombL.ar(pluck2, period/2, period/2, 4);

		Pan2.ar(string1 + string2, i * 0.2 - 0.5);
	});
	out = LeakDC.ar(LPF.ar(out, 12000));
	Out.ar(busOut, out);
}).store;


/*
@synthDef source
@name pluck2
@param pluck 0 1
*/
SynthDef(\pluck2, { arg busOut, pluck;
	var pitch1, pitch2, out;
	pitch1 = [ 52, 57, 62, 67, 71, 76, 81, 86, 91, 96 ];
	pitch2 = pitch1 + 7;
	out = Mix.arFill(pitch1.size, { arg i;
		var trigger, pluck1, pluck2, period1, period2, string1, string2;
		// place trigger points from 0.25 to 0.75
		trigger = HPZ1.kr(pluck > (0.25 + (i * 0.1)));

		pluck1 = PinkNoise.ar(Decay.kr(trigger.max(0), 0.05));
		period1 = pitch1.at(i).midicps.reciprocal;
		string1 = CombL.ar(pluck1, period1, period1, 4);

		pluck2 = BrownNoise.ar(Decay.kr(trigger.neg.max(0), 0.05));
		period2 = pitch2.at(i).midicps.reciprocal;
		string2 = CombL.ar(pluck2, period2, period2, -4);

		Pan2.ar(string1 + string2, i * 0.2 - 0.5);
	});
	out = LeakDC.ar(LPF.ar(out, 12000));
	Out.ar(busOut, out);
}).store;


/*
@synthDef source
@name quickChimes
*/
SynthDef(\quickChimes, { arg busOut;
	var out, exc, amp;
	amp = LFPulse.kr(12,0,0.1,0.002);
	exc = LPZ1.ar(GrayNoise.ar([amp,amp]));
	out = Klank.ar(`[FloatArray.fill(8, { arg i; 100*pow(2,i) }),
			nil,
			FloatArray[1, 1, 1, 1]], exc);
	Out.ar(busOut, out);
}).store;


/*
@synthDef source
@name rezonator
@param freq 10 3000
@param rq 0.01 0.1
*/
SynthDef(\rezonator, { arg busOut, freq, rq;
	var res, comb, out;
	res = RHPF.ar(
		LFSaw.ar([4, 7], 0.0, 0.2),
		freq,
		rq);
	comb = CombC.ar(res, 1.5, MouseY.kr(0.001, 1.5, 3), 3.5);
	out = AllpassC.ar(comb, 0.6, 0.3, 4);
	Out.ar(0, out);
}).store;


/*
@synthDef source
@name warmChimes
*/
SynthDef(\warmChimes, { arg busOut;
	var out, scale, specs, freqs, amps, rings,
	numRes = 5, bells = 20, pan;
	scale = [60, 62, 64, 67, 69].midicps;
	out = Mix.fill(bells, {
		freqs = Array.fill(numRes, {rrand(1, 15)*(scale.choose)});
		amps = Array.fill(numRes, {rrand(0.3, 0.9)});
		rings = Array.fill(numRes, {rrand(1.0, 4.0)});
		specs = [freqs, amps, rings].round(0.01);
		pan = (LFNoise1.kr(rrand(3, 6))*2).softclip;
		Pan2.ar( Klank.ar(`specs, Dust.ar(1/6, 0.03)), pan)
	});
	Out.ar(busOut, out);
}).store;



/*
@synthDef source
@name pmOsc
@param left 5 13
@param right 5 13
@param indexLow 1 3
@param indexHigh 4 10
*/
SynthDef(\pmOsc, { arg busOut, left, right, indexLow, indexHigh;
	var pitches, idxpitch, pitch, timbre, trigger, env, index, out;
	pitches = -12 + [ 28, 36, 40, 47, 60, 64, 67, 71, 76, 79, 84 ];
	trigger = Impulse.kr([left, right]); // trigger
	//pitch = TRand.kr(36, 72, trigger).round(1); // C2 to C6
	idxpitch = TIRand.kr(0, pitches.size, trigger).round(1);
	pitch = Select.kr(idxpitch, pitches).midicps;
	timbre = LFNoise0.kr(1/20, mul: 0.2, add: 2); // mod control
	env = Linen.kr(trigger, releaseTime: 1/[left, right]); // envelope
	index = env * indexHigh + indexLow; // env scaled and offset for index
	out = PMOsc.ar(pitch, pitch*timbre, index, mul: env);
	Out.ar(busOut, out);
}).store;


/*
@synthDef source
@name repeater
@param freq 1 5000
@param lfoDepth 0 1500
*/
SynthDef(\repeater, { arg busOut, freq, lfoDepth;
	var out, input, modulator, lfoRate, filtered;
	lfoRate = 200.0;
	freq = LFSaw.kr(lfoRate, 0, lfoDepth, lfoDepth * 1.2);
	filtered = RLPF.ar({WhiteNoise.ar(0.03)}.dup, freq, 0.1);
	input = CombN.ar(filtered, 4.0, 2.0, 0.5, 0.2, filtered);
	modulator = SinOsc.ar(freq, [0,0.5pi]);
	out = input * modulator;
	Out.ar(busOut, out);
}).store;



/*
@synthDef source
@name wormdig
*/
SynthDef(\wormdig, { arg busOut;
	var out;
	var time = 60*7;
	var hex = {|f| 1 - LFTri.ar(f)};
	var line = {|s,e| Line.kr(s,e,time,1,0)};
	var hexes = hex.(line.(17,1647)) * hex.(line.(217,17)) * hex.(67) * hex.([55,55.1]) * 0.05;
	var verb1 = hexes * SinOsc.ar(300) * SinOsc.ar(0.01);
	var verb2 = hexes * SinOsc.ar(SinOsc.ar(0.0334).range(100, 157.7)) * SinOsc.ar(0.008);
	var verb3 = hexes * SinOsc.ar(SinOsc.ar(0.0234).range(200, 257.7)) * SinOsc.ar(0.0043);
	var verb4 = (hexes * 20 * SinOsc.ar(3257.7)).clip(-1,1) * SinOsc.ar(1257.7) * SinOsc.ar(0.023) / 30;
	var combine = verb1 + verb2 + verb3 + CombC.ar(verb4 * SinOsc.ar(Line.ar(300, 10000, 60*7)), 10, 1, 10, 1, verb4);
	combine = combine * EnvGen.ar(Env.linen(0.01, time - 15, 13));
	combine = combine + DelayC.ar(GVerb.ar(combine, [1.7, 1.8], 10, drylevel: 0), 10, Line.kr(0.01, 1, time), 0.5) * 0.1;
	combine = combine + LPF.ar(CombC.ar(combine, 20, Line.kr(1, 0.01, time), 20), LFTri.ar([0.0312, 0.0222]).range(50, 10000)) * 0.1;
	combine = Compander.ar(combine, combine, 0.5, 1, 1/20) * 8;
	combine = Compander.ar(combine, combine, 0.5, 1, 1/20) / 2.5;
	out = HPF.ar(combine, 55);
	Out.ar(busOut, out);
}).store;


/*
@synthDef source
@name rezlpf
@param freq 10 3000
@param rq 0.01 0.1
*/
SynthDef(\rezlpf, { arg busOut, freq, rq;
	var out = RLPF.ar(                                    // resonant low pass filter
		LFSaw.ar([8, 12], 0, 0.2),               // in = sawtooth (amp 0.2, freq = [8hz, 12hz]
		//LFNoise1.ar([2, 3].choose, 1500, 1600),  // cutoff freq b/w 100 and 3100 hz, changed 2 or 3x a second
		freq,
		rq,                                    // reciproqal of Q
		mul: 0.4                                 // x 0.4
	);
	Out.ar(busOut, out);
}).store;




//==========================================
//  MODS
//==========================================


/*
@synthDef modifier
@name pass
*/
SynthDef(\pass, { arg busOut, busIn;
	Out.ar(busOut, In.ar(busIn, 1));
}).store;


/*
@synthDef modifier
@name deladd
@param deltime 0 5
*/
SynthDef(\deladd, { arg busOut, busIn, deltime;
	var sig, out;
	sig = In.ar(busIn, 1);
	out = Mix.ar([sig, DelayC.ar(sig, 5.0, deltime)]);
	Out.ar(busOut, out);
}).store;


/*
@synthDef modifier
@name fxcomb
@param deltime 0 3
@param dectime 0 3
*/
SynthDef(\fxcomb, { arg busOut, busIn, deltime, dectime;
	var sig, out;
	sig = In.ar(busIn, 1);
	out = CombC.ar(sig, 3.0, deltime, dectime);
	Out.ar(busOut, out);
}).store;


/*
@synthDef modifier
@name freeverb
@param mix 0 1
@param room 0 1
@param damp 0 1
*/
SynthDef(\freeverb, { arg busOut, busIn, mix, room, damp;
	var in, out;
	in = In.ar(busIn);
	out = FreeVerb.ar(in, mix, room, damp);
	Out.ar(busOut, out);
}).store;


/*
@synthDef modifier
@name pitchShift
@param pitchRatio 0 4
@param pitchDeviation 0 1
@param timeDispersion 0 1
*/
SynthDef(\pitchShift, { arg busIn, busOut, pitchRatio, pitchDeviaton, timeDispersion;
	var in, out;
	in = In.ar(busIn);
	out = PitchShift.ar(in, 0.05, pitchRatio, pitchDeviaton, timeDispersion);
	Out.ar(busOut, out);
}).store;


/*
@synthDef modifier
@name rlpf
@param freq 0 1000
@param rq 0 1
*/
SynthDef(\rlpf, { arg busIn, busOut, freq, rq;
	var in, out;
	in = In.ar(busIn);
	out = RLPF.ar(in, freq, rq);
	Out.ar(busOut, out);
}).store;


/*
@synthDef modifier
@name rhpf
@param freq 0 1000
@param rq 0 1
*/
SynthDef(\rhpf, { arg busIn, busOut, freq, rq;
	var in, out;
	in = In.ar(busIn);
	out = RHPF.ar(in, freq, rq);
	Out.ar(busOut, out);
}).store;


/*
@synthDef modifier
@name resonz
@param freq 0 1000
@param bwr 0 1
*/
SynthDef(\resonz, { arg busIn, busOut, freq, bwr;
	var in, out;
	in = In.ar(busIn);
	out = Resonz.ar(in, freq, bwr);
	Out.ar(busOut, out);
}).store;


/*
@synthDef modifier
@name ringz
@param freq 0 1000
@param dectime 0 3
*/
SynthDef(\ringz, { arg busIn, busOut, freq, dectime;
	var in, out;
	in = In.ar(busIn);
	out = Ringz.ar(in, freq, dectime);
	Out.ar(busOut, out);
}).store;


/*
@synthDef modifier
@name freqShift
@param freq 0 1000
@param phase 0 6
*/
SynthDef(\freqShift, { arg busIn, busOut, freq, phase;
	var in, out;
	in = In.ar(busIn);
	out = FreqShift.ar(in, freq, phase);
	Out.ar(busOut, out);
}).store;

)