/*
Usage

(
SynthDef("test", {arg freq = 440, amp = 1, dur = 1;
	var env = DynEnv.ar(amp, dur);
	var fenv = DynEnv.ar(freq, dur);
	Out.ar(0, SinOsc.ar(fenv) * env)
}).add

t = Synth("test");
)

t.set(\amp, 0.5)
t.set(\dur, 2)
t.set(\freq, 440 * 3)
t.set(\freq, 440 / 4)

*/

DynEnv : UGen {
  *ar { arg v = 0.0, dur = 1.0;
    ^this.multiNew('audio', v, dur)
  }
  *kr { arg v = 0.0, dur = 1.0;
    ^this.multiNew('control', v, dur)
  }
}