// Variation on sample and hold liquidities from SuperCollider book, run through Comb filter.
//  -> http://ecmc.rochester.edu/ecmc/docs/supercollider/scexamples.1.html
// finger x and y position controls the pitch and clock rate respectively.


///////////////////////////////////
/////  1) SETUP BUSSES
(
~h1x = Bus.control(s, 1);   // leap motion, first finger x position
~h1y = Bus.control(s, 1);   // leap motion, first finger y position
~h1z = Bus.control(s, 1);   // leap motion, first finger y position
~h1w = Bus.control(s, 1);   // leap motion, first finger y position
~h1a = Bus.control(s, 1);   // leap motion, first finger y position
)


///////////////////////////////////
/////  2) SETUP SYNTH
(
SynthDef(\sahliquid, {
	arg out = 0, gate = 1, atk = 1.0;
	var env, clockRate, clockTime, clock, centerFreq, freq, panPos, patch, deltime, dectime, sig;
	deltime = In.kr(~h1z);
	dectime = 0.2;
	env = EnvGen.kr(Env.adsr(atk, 1, 1), gate, doneAction: 2);
	centerFreq = 2000 * 2.asWarp.map(In.kr(~h1x)) + 100;
	clockRate = 300 * In.kr(~h1y) + 1;
	clockTime = 2.0*clockRate.reciprocal;
	clock = Impulse.kr(clockRate, 0.4);
	freq = Latch.kr(WhiteNoise.kr(centerFreq * 0.5, centerFreq), clock);
	panPos = Latch.kr(WhiteNoise.kr, clock);
	patch = CombN.ar(
		Pan2.ar(
			SinOsc.ar(freq, 0, Decay2.kr(clock, 0.1 * clockTime, 0.9 * clockTime)),
			panPos
		), 1.0, deltime, dectime);
	sig = patch;
	2.do({ sig = AllpassC.ar(sig, 0.5, DC.kr(Rand()).range(0.001, 0.5*(3.asWarp.map(In.kr(~h1a)+0.001))), 8)});

	Out.ar(out, env * sig);
}).add;
)

///////////////////////////////////
/////  3) LAUNCH SYNTH

~sahliquid = Synth(\sahliquid);


///////////////////////////////////
/////  4) SETUP OSC CONTROL

// first finger position
(
OSCFunc({
	|msg|
	~h1x.set(msg[1]);
},  '/h1x/', nil);
OSCFunc({
	|msg|
	~h1y.set(msg[1]);
},  '/h1y/', nil);
OSCFunc({
	|msg|
	~h1z.set(msg[1]);
},  '/h1z/', nil);
OSCFunc({
	|msg|
	~h1w.set(msg[1]);
},  '/h1w/', nil);
OSCFunc({
	|msg|
	~h1a.set(msg[1]);
},  '/h1a/', nil);
)




(
OSCFunc({
	|msg|
	~h1x.set(msg[1]);
	~h1y.set(msg[2]);
},  '/h1/', nil);
)









///////////////////////



// Filtered Ring Modulator
// a ring modulator going through an allpass filter with variable delay time
// first setup busses, synths, and osc control.
// then, using the Processing leap OSC sketch, the x, y, and z position of
// your first finger control the carrier freq, modulator freq, and allpass delay
// time respectively. be careful, can get loud


///////////////////////////////////
/////  1) SETUP BUSSES
(
~h1x = Bus.control(s, 1);   // leap motion, first finger x position
~h1y = Bus.control(s, 1);   // leap motion, first finger y position
~h1z = Bus.control(s, 1);   // leap motion, first finger z position
~h1w = Bus.control(s, 1);   // leap motion, first finger z position
~h1a = Bus.control(s, 1);   // leap motion, first finger z position
)


///////////////////////////////////
/////  2) SETUP SYNTH
(
SynthDef(\ringMod, {
	arg outBus = 0, gate = 1, atk = 5.0;
	var env, freq, ratio, modulator, carrier, sig;
	env = EnvGen.kr(Env.adsr(atk, 1, 1), gate, doneAction: 2);
	freq = LFNoise0.kr(65*In.kr(~h1x)+5, 100*In.kr(~h1a), 60).round(24).midicps;
	ratio = -2.0+14.0*In.kr(~h1z);
	modulator = SinOsc.ar(freq * ratio, 0, 4.0);
	carrier = SinOsc.ar(freq, modulator, 0.5);
	sig = carrier!2;
	//8.do({ out = AllpassC.ar(out, 0.5, { Rand(0.001, 0.03) }.dup, 8)});
	2.do({ sig = AllpassC.ar(sig, 0.5, DC.kr(Rand()).range(0.001, 0.5*(3.asWarp.map(In.kr(~h1y)+0.001))), 12.0*In.kr(~h1w))});
	Out.ar(outBus, 0.2 * env * sig);
}).add;
)

///////////////////////////////////
/////  3) LAUNCH SYNTH

~ringmod = Synth(\ringMod);


///////////////////////////////////
/////  4) SETUP OSC CONTROL

// first finger position
(
OSCFunc({
	|msg|
	~h1x.set(msg[1]);
	~h1y.set(msg[2]);
	~h1z.set(msg[3]);
},  '/h1/', nil);
)


// first finger position
(
OSCFunc({
	|msg|
	~h1x.set(msg[1]);
},  '/h1x/', nil);
OSCFunc({
	|msg|
	~h1y.set(msg[1]);
},  '/h1y/', nil);
OSCFunc({
	|msg|
	~h1z.set(msg[1]);
},  '/h1z/', nil);
OSCFunc({
	|msg|
	~h1w.set(msg[1]);
},  '/h1w/', nil);
OSCFunc({
	|msg|
	~h1a.set(msg[1]);
},  '/h1a/', nil);
)
