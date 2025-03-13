#pragma once
#include <JuceHeader.h>
#include "Parameters.h"
#include "Cutter.h"
#include "Looper.h"
#include "FileLoader.h"

class Granulizer
{
public:

	Granulizer()
	{

	}

	~Granulizer() {}

	void prepareToPlay(double sampleRate)
	{

	}

	void releaseResources()
	{

	}

	void processBlock(AudioBuffer<float>& buffer, AudioBuffer<float>& sampleBuffer, float velocity, AudioBuffer<double>& posModulationSignal, AudioBuffer<double>& window, int numGrains, int start, int end)
	{

		const auto numOutputSamples = buffer.getNumSamples();
		const auto numChannels = buffer.getNumChannels();

		auto** outputData = buffer.getArrayOfWritePointers();
		auto** windowArray = window.getArrayOfReadPointers();
		auto** sampleData = sampleBuffer.getArrayOfReadPointers();
		auto** posArray = posModulationSignal.getArrayOfReadPointers();

		gainReduction = numGrains > 1 ? (2.0f / float(numGrains)) : 1.0f;

		for (int g = 0; g < numGrains; g++)
		{

			for (int smp = 0; smp < numOutputSamples; ++smp)
			{

				for (int ch = 0; ch < numChannels; ++ch)
				{
					// Get position value for this sample
					auto readIndex = posArray[g][smp];

					// Bound position value into start and end points
					if (start != end)
					{
						while (readIndex > jmax(start, end))
							readIndex -= abs(end - start);
						while (readIndex < jmin(start, end))
							readIndex += abs(end - start);
					}
					else
						readIndex = start;

					// Set interpolation up
					auto integerPart = (int)readIndex;
					auto fractionalPart = readIndex - (int)readIndex;
					auto A = (integerPart + sampleBuffer.getNumSamples()) % sampleBuffer.getNumSamples();
					auto B = (A + 1) % sampleBuffer.getNumSamples();

					const auto alpha = fractionalPart / (2.0 - fractionalPart);

					// Return position value to draw lines on GUI
					posValue[g] = (A + fractionalPart) / sampleBuffer.getNumSamples();
					
					// NO INTERPOLATION
					//auto sampleValue = sampleData[ch][integerPart]; 

					// LINEAR INTERPOLATION
					//auto sampleValue = sampleData[ch][A] * (1.0 - fractionalPart) + sampleData[ch][B] * fractionalPart;

					// ALLPASS 
					auto sampleValue = alpha * (sampleData[ch][B] - oldSample[g][ch]) + sampleData[ch][A];
					oldSample[g][ch] = sampleValue;

					// OUTPUT POSITION (set position to 0)
					//outputData[ch][smp] = 100 * readIndex / grainBuffer.getNumSamples();

					// OUTPUT WINDOW
					//outputData[ch][smp] = (windowArray[jmin(ch, numWindowChannels - 1)][smp] - 0.5) * 2;

					// Apply velocity
					sampleValue *= velocity;

					// Write onto output buffer and apply window and apply gain reduction not to make the sum of grain super loud
					outputData[ch][smp] += sampleValue * windowArray[g][smp] * gainReduction;


					//WINDOW FRACTIONAL: Write into output buffer and apply window using allpass filter for the window too (due to non integer shift)

					//auto wIntegerPart = (int)wReadIndex;
					//auto wFractionalPart = wReadIndex - wIntegerPart;
					//auto wA = (wIntegerPart + window.getNumSamples()) % window.getNumSamples();
					//auto wB = (wA + 1) % window.getNumSamples();
					//const auto wAlpha = wFractionalPart / (2.0f - wFractionalPart);
					//auto windowValue = wAlpha * (windowArray[ch][wB] - wOldSample[ch]) + windowArray[ch][wA];
					//wOldSample[ch] = windowValue;
					//outputData[ch][smp] += sampleValue * windowValue;


					// OUTPUT WINDOW
					//outputData[ch][smp] = 2*(windowArray[g][smp]-0.5);

				}

			}

		}

	}

	float getPos(int grain)
	{
		return posValue[grain];
	}

private:

	float oldSample[MAX_GN][2] = {};
	float gainReduction = 1;

	float posValue[MAX_GN] = {};

};