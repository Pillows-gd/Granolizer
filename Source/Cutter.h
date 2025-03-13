#pragma once
#include <JuceHeader.h>

class Cutter
{
public:
	AudioBuffer<float> getBufferRegion(const AudioBuffer<float>& sourceBuffer, int startPosition, int endPosition)
	{

		int numSamples;

		if (startPosition > endPosition)
		{
			numSamples = sourceBuffer.getNumSamples() - startPosition + endPosition;
			bufferRegion.setSize(sourceBuffer.getNumChannels(), numSamples);
			for (int ch = sourceBuffer.getNumChannels(); --ch >= 0; )
			{
				bufferRegion.copyFrom(ch, 0, sourceBuffer, ch, startPosition, sourceBuffer.getNumSamples() - startPosition);
				bufferRegion.copyFrom(ch, sourceBuffer.getNumSamples() - startPosition, sourceBuffer, ch, 0, endPosition);
			}
		}
		else
		{
			numSamples = endPosition - startPosition;
			bufferRegion.setSize(sourceBuffer.getNumChannels(), numSamples);
			for (int ch = sourceBuffer.getNumChannels(); --ch >= 0; )
			{
				bufferRegion.copyFrom(ch, 0, sourceBuffer, ch, startPosition, numSamples);
			}
		}

		smoothRegion(bufferRegion);

		return bufferRegion;

	}

private:

	void smoothRegion(AudioBuffer<float>& buffer)
	{

		//AddFromWithRamp

		AudioBuffer<float> nonSmoothedBuffer = buffer;
		auto* nonSmoothedBufferPointers = nonSmoothedBuffer.getArrayOfReadPointers();

		buffer.clear();

		for (int ch = buffer.getNumChannels(); --ch >= 0; )
		{
			buffer.addFromWithRamp(ch, 0, nonSmoothedBufferPointers[ch], (int)(ceil(buffer.getNumSamples() / 2)), 0, 1);
			buffer.addFromWithRamp(ch, (int)(ceil(buffer.getNumSamples() / 2)), nonSmoothedBufferPointers[ch], (int)(buffer.getNumSamples() / 2), 1, 0);
		}

		//Naive
		//int length = buffer.getNumSamples();
		//int numChannels = buffer.getNumChannels();

		//auto regionData = buffer.getArrayOfWritePointers();

		//auto window = makeWindow(length).getArrayOfReadPointers();

		//if (length > 2)
		//{
		//	for (int ch = numChannels; --ch >= 0; )
		//	{
		//		for (int smp = length; --smp >= 0; )
		//		{
		//			regionData[ch][smp] *= window[0][smp];
		//		}
		//	}
		//}

	}

	//AudioBuffer<float> makeWindow(int numSamples)
	//{
	//	AudioBuffer<float> window;
	//	window.setSize(1, numSamples);

	//	int peak = (int)(numSamples / 2);
	//	float value = 1.0f;
	//	
	//	for (int smp = numSamples; --smp >= 0; )
	//	{
	//		if (smp < peak)
	//		{	
	//			value = (float)smp / (float)peak;
	//		}
	//		else
	//		{
	//			value = 2 - ((float)smp / (float)peak);
	//		}
	//		window.addSample(0, smp, value);
	//	}

	//	return window;
	//}

	AudioBuffer<float> bufferRegion;

};