#pragma once
#include <JuceHeader.h>
#include "Parameters.h"

class NaiveOscillator
{
public:
	NaiveOscillator(int destCh = 0)
	{
		ch = destCh;
		for (int f = 0; f < MAX_NUM_CH; ++f)
			filters.add(new IIRFilter());
	};

	~NaiveOscillator()
	{};

	void prepareToPlay(double sr)
	{
		sampleRate = sr;
		samplePeriod = 1.0 / sr;
		phaseDelta.reset(sr, SMOOTHING_TIME);
		phaseModAmt.reset(sr, SMOOTHING_TIME);
		for (int f = filters.size(); --f >= 0;)
			filters.getUnchecked(f)->reset();
	}

	void setPhase(float newValue)
	{
		currentPhase = newValue;
		currentSample = 0;
	}

	void setFrequency(float newValue)
	{
		frequency = newValue;
		if (frequency * LFO_SMOOTH_RED > 40 && frequency * LFO_SMOOTH_RED <= 0.5 * sampleRate)
		{
			iirCoeffs = IIRCoefficients::makeLowPass(1 / samplePeriod, LFO_SMOOTH_RED * frequency, FILTER_Q);
			for (int f = filters.size(); --f >= 0;)
				filters.getUnchecked(f)->setCoefficients(iirCoeffs);
		}
	}

	void setWaveform(float newValue)
	{
		waveform = roundToInt(newValue);
	}

	void setMul(float newValue)
	{
		mul = newValue;
	}

	void setPhaseDelta(float newValue)
	{
		phaseDelta.setTargetValue(newValue);
	}

	void setWaveform(int newValue)
	{
		waveform = newValue;
	}

	void setPhaseModAmt(float newValue, int numGrains)
	{
		phaseModAmt.setTargetValue(newValue * ch / numGrains);
	}

	void getNextAudioBlock(AudioBuffer<double>& buffer, const int startSample, const int numSamples, AudioBuffer<double>& phaseModBuffer = AudioBuffer<double>(0, 0))
	{
		auto bufferData = buffer.getArrayOfWritePointers();
		auto modBuffer = phaseModBuffer.getArrayOfReadPointers();

		for (int smp = startSample; smp < startSample + numSamples; ++smp)
			if (phaseModBuffer.getNumChannels() != 0)
				bufferData[ch][smp] = getNextAudioSample(true, modBuffer[0][smp]);
			else
				bufferData[ch][smp] = getNextAudioSample(false);
	}

	double getNextAudioSample(bool hasMod, double phaseModValue = 0)
	{
		double sampleValue = 0.0f;

		//if (hasMod)
		//	phase = currentPhase + phaseDelta.getNextValue() + (phaseModValue + 1) * phaseModAmt.getNextValue() / 2;
		//else
		//	phase = currentPhase + phaseDelta.getNextValue();

		double phase = currentPhase + phaseDelta.getNextValue() + hasMod * ((phaseModValue + 1.0) * phaseModAmt.getNextValue() * 0.5);
		
		phase -= static_cast<int>(phase);

		//samplesPerPeriod = 1.0 / (frequency * samplePeriod);
		samplesPerPeriod = roundToInt(sampleRate / frequency);
		currentSample++;

		switch (waveform)
		{
		case 0: // Sine
			sampleValue = mul * sin(MathConstants<double>::twoPi * phase);
			break;
		case 1: // Triangular
			//sampleValue = mul * phase < 0.5 ? ((4 * phase) - 1) : (4 * (1 - phase) - 1);
			sampleValue = mul * (4 * abs(phase - 0.5) - 1.0);
			break;
		case 2: // Saw up
			sampleValue = mul * (2.0f * phase - 1.0f);
			break;
		case 3: // Saw down
			sampleValue = mul * (-2.0f * phase + 1.0f);
			break;
		case 4: // Square
			//if (frequency != 0)
			//{
			//	if (currentSample < samplesPerPeriod)
			//		sampleValue = oldValue;
			//	else
			//	{
			//		if (oldValue < 0)
			//			sampleValue = 1;
			//		else
			//			sampleValue = -1;
			//	}
			//} 
			//else
			//	sampleValue = oldValue;
			sampleValue = (phase > 0.5) - (phase < 0.5);
			break;
		case 5: // Sample and hold
			sampleValue = (currentSample < samplesPerPeriod) ? oldValue : (oldValue = (randVal.nextDouble() * 2.0) - 1.0);
			//if (frequency != 0)
			//	sampleValue = currentSample < samplesPerPeriod ? oldValue : (randVal.nextDouble() * 2) - 1;
			//else
			//	sampleValue = oldValue;
			break;
		case 6: // Window
			sampleValue = mul * (-0.5 * cos(MathConstants<double>::twoPi * phase) + 0.5);
			break;
		default:
			break;
		}

		//oldValue = sampleValue;
		currentSample %= samplesPerPeriod;

		//phaseIncrement = frequency.isSmoothing() ? frequency.getNextValue() * samplePeriod : phaseIncrement;
		phaseIncrement = frequency * samplePeriod;
		currentPhase += phaseIncrement;
		currentPhase -= static_cast<int>(currentPhase);

		return sampleValue;
	}

	void loPass(AudioBuffer<double>& buffer, const int startSample, const int numSamples)
	{

		auto** b = buffer.getArrayOfWritePointers();

		if (getFrequency() != 0)
		{
			for (int ch = filters.size(); --ch >= 0;)
				for (int smp = startSample; smp < startSample + numSamples; ++smp)
				{
					//filters.getUnchecked(ch)->processSingleSampleRaw(b[ch][smp]);
					b[ch][smp] = double(filters.getUnchecked(ch)->processSingleSampleRaw(b[ch][smp]));
				}
		}
	}

	double getFrequency()
	{
		return frequency;
	}

	int getWaveform()
	{
		return waveform;
	}

private:
	int ch = 0;
	int waveform = 0;

	SmoothedValue<double, ValueSmoothingTypes::Linear> phaseDelta;
	double currentPhase = 0.0f;

	double samplePeriod = 1.0;
	double sampleRate = 1.0;
	double phaseIncrement = 0.0;
	double frequency = 440.0;
	double mul = 1.0;
	Random randVal;
	int samplesPerPeriod = roundToInt(sampleRate / frequency);
	int currentSample = 0;
	double oldValue = 0.0;
	int currentPhaseModSample = 0;
	SmoothedValue<double, ValueSmoothingTypes::Linear> phaseModAmt;
	OwnedArray<IIRFilter> filters;
	IIRCoefficients iirCoeffs;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NaiveOscillator)
};

class ParameterModulation
{
public:
	ParameterModulation() {}
	~ParameterModulation() {}

	void prepareToPlay(double sr, int samplesPerBlock)
	{
		position.reset(sr, SMOOTHING_TIME);
		speed.reset(sr, SMOOTHING_TIME);
		spaceSpread.reset(sr, SMOOTHING_TIME);
		startCut.reset(sr, SMOOTHING_TIME);
		endCut.reset(sr, SMOOTHING_TIME);
	}

	void releaseResources()
	{
		
	}

	void setPos(double newValue)
	{
		position.setTargetValue(newValue);
	}

	void setSpeed(float newValue)
	{
		speed.setTargetValue(newValue);
	}

	void setSpaceSpread(double newValue)
	{
		spaceSpread.setTargetValue(newValue);
	}

	void setStartCut(double newValue)
	{
		startCut.setTargetValue(newValue);
	}

	void setEndCut(double newValue)
	{
		endCut.setTargetValue(newValue);
	}

	float getSpeed()
	{
		return speed.getCurrentValue();
	}

	double getRelativePos(int grain)
	{
		return relativePos[grain];
	}

	void processBlock(AudioBuffer<double>& buffer, const int startSample, const int numSamples, int sampleSize, int numGrains, double scaleFactor = 1.0)
	{
		auto bufferData = buffer.getArrayOfWritePointers();
		auto numCh = buffer.getNumChannels();

		// Scale modulation from 0 to scaleFactor (default = 1)
		for (int ch = 0; ch < numCh; ++ch)
		{
			FloatVectorOperations::add(bufferData[ch] + startSample, 1.0, numSamples);
			FloatVectorOperations::multiply(bufferData[ch] + startSample, 0.5 * scaleFactor, numSamples);
			//FloatVectorOperations::multiply(bufferData[ch] + startSample, scaleFactor, numSamples);
		}

		// Scale modulation with current speed value
		//speed.applyGain(buffer, numSamples); // fatto nel loop sample

		// Sum modulation with current position and position spread value
		const auto ng = 1.0 / double(numGrains);
		for (int smp = startSample; smp < startSample + numSamples; ++smp)
		{
			const auto spd = speed.getNextValue();
			const auto newPos = position.getNextValue()/* * abs((endCut.getNextValue() - startCut.getNextValue())) + jmin(startCut.getNextValue(), endCut.getNextValue())*/;
			const auto newSpr = spaceSpread.getNextValue();
			const auto newStc = startCut.getNextValue();
			const auto newEnc = endCut.getNextValue();
			const auto cutLen = abs(newStc - newEnc);
			const auto actualStart = jmin(newStc, newEnc);
			// Here every channel represents a grain
			for (int ch = 0; ch < numCh; ++ch)
			{
				bufferData[ch][smp] *= spd;

				bufferData[ch][smp] += ((newPos + (newSpr * ch/* * abs((endCut.getNextValue() - startCut.getNextValue())) */ * ng)) * cutLen
					+ actualStart) * sampleSize;

				//relativePos.set(ch, ((newPos + newSpr * ch / numGrains) * cutLen + actualStart));
				relativePos[ch] = (newPos + newSpr * ch * ng) * cutLen + actualStart;
			}
		}
	}

private:

	SmoothedValue<double, ValueSmoothingTypes::Linear> speed;
	SmoothedValue<double, ValueSmoothingTypes::Linear> position;
	SmoothedValue<double, ValueSmoothingTypes::Linear> spaceSpread;
	SmoothedValue<double, ValueSmoothingTypes::Linear> startCut;
	SmoothedValue<double, ValueSmoothingTypes::Linear> endCut;
	//Array<double> relativePos = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
	double relativePos[7] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParameterModulation)

};

class ModulationManager
{
public:
	ModulationManager() {}
	~ModulationManager() {}

	void prepareToPlay(double sr)
	{
		spaceMovementAmt.reset(sr, SMOOTHING_TIME);
		startCut.reset(sr, SMOOTHING_TIME);
		endCut.reset(sr, SMOOTHING_TIME);
	}

	void processBlock(AudioBuffer<double>& buffer, const int startSample, const int numSamples, int sampleSize, AudioBuffer<double>& modulation)
	{
		auto bufferData = buffer.getArrayOfWritePointers();
		auto modulationData = modulation.getArrayOfWritePointers();

		for (int ch = 0; ch < modulation.getNumChannels(); ++ch)
		{
			FloatVectorOperations::add(modulationData[ch] + startSample, 1.0, numSamples);
			FloatVectorOperations::multiply(modulationData[ch] + startSample, 0.5, numSamples);
		}

		//spaceMovementAmt.applyGain(modulation, numSamples);
		for (int smp = startSample; smp < startSample + numSamples; ++smp)
		{
			const auto amt = spaceMovementAmt.getNextValue();
			for (int ch = 0; ch < modulation.getNumChannels(); ++ch)
			{
				modulationData[ch][smp] *= amt;
			}
		}

		for (int smp = startSample; smp < startSample + numSamples; ++smp)
		{
			for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
			{
				bufferData[ch][smp] += modulationData[jmin(ch, modulation.getNumChannels() - 1)][smp] * sampleSize;
			}
		}
			
	}

	void setStartCut(double newValue)
	{
		currentStartCut = newValue;
		startCut.setTargetValue(currentStartCut);
		setSpaceMovementAmt(currentSpaceMovementAmt);
	}

	void setEndCut(double newValue)
	{
		currentEndCut = newValue;
		endCut.setTargetValue(currentEndCut);
		setSpaceMovementAmt(currentSpaceMovementAmt);
	}

	void setSpaceMovementAmt(double newValue)
	{
		currentSpaceMovementAmt = newValue;
		spaceMovementAmt.setTargetValue(currentSpaceMovementAmt * (currentEndCut - currentStartCut));
	}

private:

	SmoothedValue<double, ValueSmoothingTypes::Linear> spaceMovementAmt;
	double currentSpaceMovementAmt = 0.0;
	SmoothedValue<double, ValueSmoothingTypes::Linear> startCut;
	double currentStartCut = 0.0;
	SmoothedValue<double, ValueSmoothingTypes::Linear> endCut;
	double currentEndCut = 0.0;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModulationManager)
};
