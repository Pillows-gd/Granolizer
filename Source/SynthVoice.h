#pragma once
#include <JuceHeader.h>
#include "SynthSound.h"

class SynthVoice : public juce::SynthesiserVoice
{
public:

	SynthVoice()
	{
		setGain(DEFAULT_G);				//added
		setTimeMovementAmt(timeMovAmt);	//added
		setStartCut(DEFAULT_SC);		//moved
		setEndCut(DEFAULT_EC);			//moved
		adsrParams.attack = DEFAULT_A;	//moved
		adsrParams.decay = DEFAULT_D;	//moved
		adsrParams.sustain = DEFAULT_S;	//moved
		adsrParams.release = DEFAULT_R;	//moved
		adsr.setParameters(adsrParams);	//moved
	}

	void prepareToPlay(double sampleRate, int samplesPerBlock)
	{
		synthBuffer.setSize(MAX_NUM_CH, samplesPerBlock);
		sr = sampleRate;
		gain.reset(sr, SMOOTHING_TIME);
		spaceMovManager.prepareToPlay(sr);
		spaceMovement.prepareToPlay(sr);
		timeMovement.prepareToPlay(sr);
		spaceMovementSignal.setSize(1, samplesPerBlock);
		timeMovementSignal.setSize(MAX_GN, samplesPerBlock);
		spaceMovement.setWaveform(spaceMovementWaveform);
		spaceMovement.setFrequency(spaceMovementFreq);
		timeMovement.setWaveform(timeMovementWaveform);
		timeMovement.setFrequency(timeMovementFreq);
		posHFOSignal.setSize(MAX_GN, samplesPerBlock);
		windowSignal.setSize(MAX_GN, samplesPerBlock);
		posHFO.prepareToPlay(sr);
		posHFO.setWaveform(WF_SAW_UP);
		windowHFO.prepareToPlay(sr);
		windowHFO.setWaveform(WF_COS_WINDOW);
		posAdapter.prepareToPlay(sr, samplesPerBlock);
		grains.prepareToPlay(sr);
		adsr.setSampleRate(sr);
	}

	void releaseResources()
	{
		timeMovementSignal.setSize(0, 0);
		spaceMovementSignal.setSize(0, 0);
		posHFOSignal.setSize(0, 0);
		windowSignal.setSize(0, 0);
		posAdapter.releaseResources();
	}

	bool canPlaySound(SynthesiserSound* sound) override
	{
		return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
	}

	void startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition) override
	{
		adsr.noteOn();
		vel = velocity;
		currentMidiNoteNumber = midiNoteNumber;
		currentNote = juce::MidiMessage::getMidiNoteInHertz(currentMidiNoteNumber + transpose);
		scaleFactor = sr / currentNote;

		if (keySync)
		{
			posHFO.setPhase(0);
			windowHFO.setPhase(0);
			spaceMovement.setPhase(0);
			timeMovement.setPhase(0);
		}

		posHFO.setFrequency(currentNote);
		windowHFO.setFrequency(currentNote);

	}

	void stopNote(float velocity, bool allowTailOff) override
	{
		adsr.noteOff();
		if (!allowTailOff || !adsr.isActive())
			clearCurrentNote();
	}

	void pitchWheelMoved(int newPitchWheelValue) override
	{

	}

	void controllerMoved(int controllerNumber, int newControllerValue) override
	{
	
	}

	void renderNextBlock(AudioBuffer<float>& buffer, int startSample, int numSamples) override
	{
		synthBuffer.clear();

		// Generate modulations
		spaceMovement.getNextAudioBlock(spaceMovementSignal, 0, numSamples);
		timeMovement.getNextAudioBlock(timeMovementSignal, 0, numSamples);

		if (timeMovement.getWaveform() != WF_SINE && timeMovement.getWaveform() != WF_TRI)
		{
			timeMovement.loPass(timeMovementSignal, 0, numSamples);
		}

		posHFO.getNextAudioBlock(posHFOSignal, 0, numSamples, timeMovementSignal);
		windowHFO.getNextAudioBlock(windowSignal, 0, numSamples, timeMovementSignal);

		// Scale modulation
		posAdapter.processBlock(posHFOSignal, 0, numSamples, sb.getNumSamples(), grainNumber, scaleFactor);
		spaceMovManager.processBlock(posHFOSignal, 0, numSamples, sb.getNumSamples(), spaceMovementSignal);

		// Process every single grain
		grains.processBlock(synthBuffer, sb, vel, posHFOSignal, windowSignal, grainNumber, startCut * sb.getNumSamples(), endCut * sb.getNumSamples());

		if (transpose != previousNote)
		{
			currentNote = juce::MidiMessage::getMidiNoteInHertz(currentMidiNoteNumber + transpose);

			// Set scaleFactor equals to the number of samples in 1 grain with the current frequency
			scaleFactor = sr / currentNote;
			
			posHFO.setFrequency(currentNote);
			windowHFO.setFrequency(currentNote);
		}

		previousNote = transpose;

		// Apply ADSR
		adsr.applyEnvelopeToBuffer(synthBuffer, 0, numSamples);

		//Apply gain
		gain.applyGain(synthBuffer, numSamples);

		if (!isVoiceActive())
			return;
		
		for (int ch = 0; ch < buffer.getNumChannels(); ch++)
			buffer.addFrom(ch, startSample, synthBuffer, ch, 0, numSamples);

		if (!adsr.isActive())
			clearCurrentNote();
	}

	void setPos(float newValue)
	{
		posAdapter.setPos(newValue);
	}

	void setSpeed(float newValue)
	{
		speed = newValue;
		if (startCut < endCut)
			posAdapter.setSpeed(speed);
		else
			posAdapter.setSpeed(-speed);
	}

	void setTranspose(float newValue)
	{
		transpose = newValue;
	}

	void setAttack(float newValue)
	{
		adsrParams.attack = newValue;
		adsr.setParameters(adsrParams);
	}

	void setDecay(float newValue)
	{
		adsrParams.decay = newValue;
		adsr.setParameters(adsrParams);
	}

	void setRelease(float newValue)
	{
		adsrParams.release = newValue;
		adsr.setParameters(adsrParams);
	}

	void setSustain(float newValue)
	{
		adsrParams.sustain = newValue;
		adsr.setParameters(adsrParams);
	}

	void setGain(float newValue)
	{
		gain.setTargetValue(Decibels::decibelsToGain(newValue));
	}

	void setGrainNumber(float newValue)
	{
		grainNumber = newValue;
		posHFO.setPhaseDelta(newValue, timeSpread);
		posHFO.setPhaseModAmt(timeMovAmt, grainNumber);		//added
		windowHFO.setPhaseModAmt(timeMovAmt, grainNumber);	//added
		posAdapter.setSpaceSpread(spaceSpread);
		windowHFO.setPhaseDelta(newValue, timeSpread);
	}

	void setSpaceMovementAmt(double newValue)
	{
		if (spaceMovement.getFrequency() != 0)
			spaceMovManager.setSpaceMovementAmt(newValue);
	}

	void setSpaceMovementFreq(float newValue)
	{
		spaceMovementFreq = newValue;
		spaceMovement.setFrequency(spaceMovementFreq);
	}

	void setSpaceMovementWaveform(int newValue)
	{
		spaceMovementWaveform = newValue;
		spaceMovement.setWaveform(spaceMovementWaveform);
	}

	void setTimeMovementAmt(float newValue)
	{
		timeMovAmt = newValue;
		if (timeMovement.getFrequency() != 0)
		{
			posHFO.setPhaseModAmt(timeMovAmt, grainNumber);
			windowHFO.setPhaseModAmt(timeMovAmt, grainNumber);
		}
	}

	void setTimeMovementFreq(float newValue)
	{
		timeMovementFreq = newValue;
		timeMovement.setFrequency(timeMovementFreq);
	}

	void setTimeMovementWaveform(int newValue)
	{
		timeMovementWaveform = newValue;
		timeMovement.setWaveform(timeMovementWaveform);
	}

	void setTimeSpread(float newValue)
	{
		timeSpread = newValue;
		posHFO.setPhaseDelta(grainNumber, timeSpread);
		windowHFO.setPhaseDelta(grainNumber, timeSpread);
	}

	void setSpaceSpread(double newValue)
	{
		spaceSpread = newValue;
		posAdapter.setSpaceSpread(newValue);
	}

	void setSample(AudioBuffer<float>& sampleBuffer)
	{
		sb = sampleBuffer;
	}

	void setStartCut(double newValue)
	{
		startCut = newValue;
		posAdapter.setStartCut(startCut);
		spaceMovManager.setStartCut(startCut);
		setSpeed(speed);
	}

	void setEndCut(double newValue)
	{
		endCut = newValue;
		posAdapter.setEndCut(endCut);
		spaceMovManager.setEndCut(endCut);
		setSpeed(speed);
	}

	void setKeySync(bool newValue)
	{
		keySync = newValue;
	}

	void syncPhases()
	{
		posHFO.setPhase(0);
		windowHFO.setPhase(0);
		spaceMovement.setPhase(0);
		timeMovement.setPhase(0);
	}

	bool getKeySync()
	{
		return keySync;
	}

	float getPosValue(int grain)
	{
		if (grain < grainNumber && isVoiceActive())
			return grains.getPos(grain);
		else
			//return posAdapter.getRelativePos(grain);
			return -1.0f;
	}

	void resetGrains(int n)
	{
		posHFO.resetGrains(n);
	}

private:
	Granulizer grains;
	AudioBuffer<double> posHFOSignal;
	AudioBuffer<double> windowSignal;
	Grains posHFO;
	Grains windowHFO;
	ParameterModulation posAdapter;
	double scaleFactor = 1.0;
	double sr = 44100;
	int grainNumber = DEFAULT_GN;
	AudioBuffer<float> sb;
	float vel = 1.0f;
	float currentNote = 440;
	ADSR adsr;
	ADSR::Parameters adsrParams;
	int currentMidiNoteNumber = 81;
	int transpose = DEFAULT_TR;
	int previousNote = transpose;
	SmoothedValue<float, ValueSmoothingTypes::Linear> gain;
	AudioBuffer<float> synthBuffer;
	float timeSpread = DEFAULT_GD;
	float spaceSpread = DEFAULT_RPS;
	NaiveOscillator spaceMovement;
	AudioBuffer<double> spaceMovementSignal;
	float spaceMovementFreq = DEFAULT_RPMF;
	ModulationManager spaceMovManager;
	int spaceMovementWaveform = DEFAULT_RPMWF;
	NaiveOscillator timeMovement;
	AudioBuffer<double> timeMovementSignal;
	int timeMovementWaveform = DEFAULT_GDMWF;
	float timeMovementFreq = DEFAULT_GDMF;
	double startCut = 0.0000;
	double endCut = 1.0000;
	bool keySync = DEFAULT_KEYSYNC;
	double speed = DEFAULT_SPD;
	float timeMovAmt = DEFAULT_GDMA;

};