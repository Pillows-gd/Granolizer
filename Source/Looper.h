#pragma once
#include <JuceHeader.h>

class Looper
{
public:

	void processBlock(AudioBuffer<float>& buffer, AudioBuffer<float>& bufferToLoop)
	{
		//We are outputting the end of the loop (or the loop is shorter than the output buffer)
		if (bufferToLoop.getNumSamples() - startSample < buffer.getNumSamples())
		{
			//Buffer to loop is shorter then output buffer
			if (bufferToLoop.getNumSamples() < buffer.getNumSamples())
			{
				fillingProgress = 0;

				//This while cicles untill the output buffer has space for less than 1 loop
				while (fillingProgress < buffer.getNumSamples() - bufferToLoop.getNumSamples())
				{

					//The current loop to put in the buffer has already been outputted partially
					if (startSample != 0)
					{
						//Fill the buffer with the remaining part of the loop
						for (int ch = buffer.getNumChannels(); --ch >= 0; )
						{
							buffer.addFrom(ch, 0, bufferToLoop, ch, startSample, bufferToLoop.getNumSamples() - startSample);
						}
						fillingProgress += bufferToLoop.getNumSamples() - startSample;
						startSample = 0;
						continue;
					}

					//Fill the output buffer with 1 loop
					for (int ch = buffer.getNumChannels(); --ch >= 0; )
					{
						buffer.addFrom(ch, fillingProgress, bufferToLoop, ch, 0, bufferToLoop.getNumSamples());
					}
					fillingProgress += bufferToLoop.getNumSamples();
				}

				//Here the output buffer is finally filled with the beginning of 1 loop
				for (int ch = buffer.getNumChannels(); --ch >= 0; )
				{
					buffer.addFrom(ch, fillingProgress, bufferToLoop, ch, 0, buffer.getNumSamples() - fillingProgress);
				}

				//Update the start sample of the next loop we are going to put in the next output buffer
				startSample = (buffer.getNumSamples() - fillingProgress) % bufferToLoop.getNumSamples();
			}
			//Buffer to loop is longer than output buffer
			else
			{
				//The number of single loop's looping samples we have not outputted yet is less than the output buffer's length
				if (bufferToLoop.getNumSamples() - startSample < buffer.getNumSamples())
				{
					//Output the end of the current loop and the beginning of the next one
					for (int ch = buffer.getNumChannels(); --ch >= 0; )
					{
						buffer.addFrom(ch, 0, bufferToLoop, ch, startSample, bufferToLoop.getNumSamples() - startSample);
						buffer.addFrom(ch, bufferToLoop.getNumSamples() - startSample, bufferToLoop, ch, 0, buffer.getNumSamples() - (bufferToLoop.getNumSamples() - startSample));
					}
					startSample = buffer.getNumSamples() - (bufferToLoop.getNumSamples() - startSample);
				}
				//The number of single loop's looping samples we have not outputted yet is greater than the output buffer's length
				else
				{
					//Output a portion of the looping sample that is as long as the output buffer
					for (int ch = buffer.getNumChannels(); --ch >= 0; )
					{
						buffer.addFrom(ch, 0, bufferToLoop, ch, startSample, buffer.getNumSamples());
					}
					startSample += buffer.getNumSamples();
				}
			}
		}
		//We are not outputting the end of the loop (and the loop is longer than the output buffer)
		else
		{
			//Output a portion of the looping sample that is as long as the output buffer
			for (int ch = buffer.getNumChannels(); --ch >= 0; )
			{
				buffer.addFrom(ch, 0, bufferToLoop, ch, startSample, buffer.getNumSamples());
			}
			startSample += buffer.getNumSamples();
		}
	}

	void setStartSample(int newValue)
	{
		startSample = newValue;
	}

	void setFillingProgress(int newValue)
	{
		fillingProgress = newValue;
	}

private:

	int startSample = 0;
	int fillingProgress = 0;

};