#pragma once
#include <JuceHeader.h>
#include "Parameters.h"

class FileLoader
{

public:

	FileLoader()
	{

	}

	~FileLoader() {}

	void prepareToPlay(double sampleRate)
	{
		hostSampleRate = sampleRate;
		if (!isFileLoaded())
		{
			sr = sampleRate;
			sampleBuffer.clear();
			sampleBuffer.setSize(0, 0);
		}
		//added
		else
			reloadFile(file);
	}

	void releaseResources()
	{
		sampleBuffer.setSize(0, 0);
	}

	bool reloadFile(File source)
	{
		sampleBuffer.clear();

		file = source;

		//Set up the file reading
		if (audioManager.getNumKnownFormats() == 0)
		{
			audioManager.registerBasicFormats();
		}
		audioReader = audioManager.createReaderFor(file);

		// Check if file exists in the current file system
		if (!file.existsAsFile())
			return false;

		std::unique_ptr<AudioFormatReaderSource> tempSource(new AudioFormatReaderSource(audioReader, true));

		//Resize the buffer
		int nCh = audioReader->numChannels;
		int nSmp = audioReader->lengthInSamples;
		sampleBuffer.setSize(nCh, nSmp);

		//Match the sample rates
		double sourceSampleRate = audioReader->sampleRate;

		//Store the sample into the buffer
		audioReader->read(&sampleBuffer, 0, nSmp, 0, true, true);

		if (hostSampleRate != sourceSampleRate)
		{
			double ratio = sourceSampleRate / hostSampleRate;

			AudioBuffer<float> resampledBuffer;
			resampledBuffer.setSize(nCh, (int)((double)nSmp / ratio));

			const float** inputs = sampleBuffer.getArrayOfReadPointers();
			float** outputs = resampledBuffer.getArrayOfWritePointers();

			for (int ch = nCh; --ch >= 0;)
			{
				ScopedPointer<LagrangeInterpolator> resampler = new LagrangeInterpolator();
				resampler->reset();
				resampler->process(ratio, inputs[ch], outputs[ch], resampledBuffer.getNumSamples());
			}


			sampleBuffer.setSize(nCh, (int)(((double)nSmp / ratio)));
			sampleBuffer.clear();

			for (int ch = nCh; --ch >= 0;)
			{
				sampleBuffer.addFrom(ch, 0, outputs[ch], (int)(((double)nSmp / ratio)));
			}
		}

		fileLoaded = true;

		return true;

	}

	void reset()
	{
		fileLoaded = false;
		sampleBuffer.clear();
		sampleBuffer.setSize(0, 0);
	}

	bool isFileLoaded()
	{
		return fileLoaded;
	}

	int getFileNumSamples()
	{
		return sampleBuffer.getNumSamples();
	}

	const String getFile()
	{
		return file.getFullPathName();
	}

	AudioBuffer<float>& getSampleBuffer()
	{
		return sampleBuffer;
	}

	AudioFormatManager& getFormatManager()
	{
		return audioManager;
	}

private:

	double hostSampleRate = 44100;
	bool fileLoaded = false;
	AudioFormatManager audioManager;
	AudioFormatReader* audioReader = nullptr;
	AudioBuffer<float> sampleBuffer;
	double sr = 44100;
	File file;

};