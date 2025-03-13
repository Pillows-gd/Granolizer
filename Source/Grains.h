#pragma once
#include <JuceHeader.h>
#include "Parameters.h"

class Grains
{

public:

	Grains()
	{
		for (int g = 0; g < MAX_GN; g++)
		{
			grains.add(new NaiveOscillator(g));
		}
	}

	~Grains() {}

	void prepareToPlay(double sr)
	{
		for (int g = 0; g < MAX_GN; g++)
		{
			grains.getUnchecked(g)->prepareToPlay(sr);
		}
	}

	void getNextAudioBlock(AudioBuffer<double>& buffer, const int startSample, const int numSamples, AudioBuffer<double>& phaseMod = AudioBuffer<double>(0, 0))
	{
		for (int g = 0; g < MAX_GN; g++)
		{
			grains.getUnchecked(g)->getNextAudioBlock(buffer, startSample, numSamples, phaseMod);
		}
	}

	void setPhase(float ph)
	{
		for (int g = 0; g < MAX_GN; g++)
		{
			grains.getUnchecked(g)->setPhase(ph);
		}
	}

	void setWaveform(int wf)
	{
		for (int g = 0; g < MAX_GN; g++)
		{
			grains.getUnchecked(g)->setWaveform(wf);
		}
	}

	void setFrequency(float f)
	{
		for (int g = 0; g < MAX_GN; g++)
		{
			grains.getUnchecked(g)->setFrequency(f);
		}
	}

	void setPhaseDelta(int grainNumber, float timeSpread)
	{
		if (grainNumber > 1)
		{
			for (int g = 0; g < MAX_GN; g++)
			{
				// Custom behaviour for time spread values: if spread < 0.5 grains are compressed to 0, otherwise they are compressed to 1
				auto shift = timeSpread < 0.5 ?
					2 * float(g) * timeSpread / grainNumber :
					2 * timeSpread - 1 + (float(g) * (2 - 2 * timeSpread) / grainNumber);
				grains.getUnchecked(g)->setPhaseDelta(shift);
			}

		}
	}

	void setPhaseModAmt(float newValue, int numGrains)
	{
		for (int g = 0; g < MAX_GN; g++)
		{
			grains.getUnchecked(g)->setPhaseModAmt(newValue, numGrains);
		}
	}

	float getFrequency()
	{
		return grains.getUnchecked(0)->getFrequency();
	}

	void resetGrains(int n)
	{
		/*for (int g = n; g < MAX_GN; g++)
		{
			grains.getUnchecked(g)->setPhase(0);
			grains.getUnchecked(g)->setPhaseDelta(0);
		}*/
	}

private:

	OwnedArray<NaiveOscillator> grains;

};
