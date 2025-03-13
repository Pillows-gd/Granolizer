#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Parameters.h"
#include "SynthSound.h"
#include "SynthVoice.h"
#include "CustomPluginEditor.h"

GranulizerAudioProcessor::GranulizerAudioProcessor()
    : parameters(*this, &undoManager, "GranulizerParameters", Parameters::createParameterLayout())
{
    synth.addSound(new SynthSound());

    for (int i = 0; i < MAX_VOICES; i++)
    {
        synth.addVoice(new SynthVoice());
    }

    parameters.addParameterListener(NAME_P, this);
    parameters.addParameterListener(NAME_SPD, this);
    parameters.addParameterListener(NAME_A, this);
    parameters.addParameterListener(NAME_D, this);
    parameters.addParameterListener(NAME_S, this);
    parameters.addParameterListener(NAME_R, this);
    parameters.addParameterListener(NAME_TR, this);
    parameters.addParameterListener(NAME_G, this);
    parameters.addParameterListener(NAME_GN, this);
    parameters.addParameterListener(NAME_GDMA, this);
    parameters.addParameterListener(NAME_GDMF, this);
    parameters.addParameterListener(NAME_GDMWF, this);
    parameters.addParameterListener(NAME_RPMA, this);
    parameters.addParameterListener(NAME_RPMF, this);
    parameters.addParameterListener(NAME_RPMWF, this);
    parameters.addParameterListener(NAME_GD, this);
    parameters.addParameterListener(NAME_RPS, this);
    parameters.addParameterListener(NAME_SC, this);
    parameters.addParameterListener(NAME_EC, this);
}

GranulizerAudioProcessor::~GranulizerAudioProcessor()
{
    
}

void GranulizerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    sr = sampleRate;
    loader.prepareToPlay(sampleRate);
    synth.setCurrentPlaybackSampleRate(sampleRate);
    for (int i = 0; i < synth.getNumVoices(); i++)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            //voice->setPos(DEFAULT_P);
            //voice->setSpeed(DEFAULT_SPD);
            //voice->setGain(DEFAULT_G);
            voice->prepareToPlay(sr, samplesPerBlock);
        }
    }
}

void GranulizerAudioProcessor::releaseResources()
{
    loader.releaseResources();
    for (int i = 0; i < synth.getNumVoices(); i++)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            voice->releaseResources();
        }
    }
}

void GranulizerAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{

    buffer.clear();

    juce::ScopedNoDenormals noDenormals;

    if (isFileLoaded() && isSynthReady)
    {
        synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    }

    for (int i = 0; i < MAX_VOICES; i++)
    {
        auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i));
        
        for (int j = 0; j < MAX_GN; j++)
        {
            pos[i][j] = voice->getPosValue(j);
        }
    }

}

juce::AudioProcessorEditor* GranulizerAudioProcessor::createEditor()
{
    return new CustomPluginEditor (*this, parameters);
}

void GranulizerAudioProcessor::getStateInformation(MemoryBlock& destData)
{
    juce::XmlElement xmlParent("parent");
    auto state = parameters.copyState();
    std::unique_ptr<XmlElement> xmlParameters = state.createXml();
    xmlParent.addChildElement(xmlParameters.release());
    juce::XmlElement* xmlFile = xmlParent.createNewChildElement("file");
    xmlFile->setAttribute(juce::Identifier(juce::String("path")), sample.getFullPathName());
    juce::XmlElement* xmlLastSampleDir = xmlParent.createNewChildElement("lastSampleDir");
    xmlLastSampleDir->setAttribute(juce::Identifier(juce::String("path")), lastSampleDir.getFullPathName());
    juce::XmlElement* xmlLastPresetDir = xmlParent.createNewChildElement("lastPresetDir");
    xmlLastPresetDir->setAttribute(juce::Identifier(juce::String("path")), lastPresetDir.getFullPathName());
    juce::XmlElement* xmlKeySync = xmlParent.createNewChildElement("keySync");
    xmlKeySync->setAttribute(juce::Identifier(juce::String("value")), getKeySync());
    copyXmlToBinary(xmlParent, destData);
}

void GranulizerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlParent(getXmlFromBinary(data, sizeInBytes));
    juce::XmlElement* xmlParameters = xmlParent->getChildByName(parameters.state.getType());
    juce::XmlElement* xmlFile = xmlParent->getChildByName("file");
    juce::XmlElement* xmlLastSampleDir = xmlParent->getChildByName("lastSampleDir");
    juce::XmlElement* xmlLastPresetDir = xmlParent->getChildByName("lastPresetDir");
    juce::XmlElement* xmlKeySync = xmlParent->getChildByName("keySync");

    if (xmlParent.get() != nullptr)
    {
        if (xmlParameters->hasTagName(parameters.state.getType()))
        {
            parameters.replaceState(juce::ValueTree::fromXml(*xmlParameters));
        }
        if (xmlFile != nullptr)
        {
            if (xmlFile->hasTagName(juce::StringRef("file")))
            {
                setSample(File(xmlFile->getStringAttribute(juce::String("path"))));
                if (!loadSample())
                {
                    clearSample();
                }
            }
        }
        if (xmlLastSampleDir != nullptr)
        {
            if (xmlLastSampleDir->hasTagName(juce::StringRef("lastSampleDir")))
            {
                setLastSampleDirectory(xmlLastSampleDir->getStringAttribute(juce::String("path")));
            }
        }
        if (xmlLastPresetDir != nullptr)
        {
            if (xmlLastPresetDir->hasTagName(juce::StringRef("lastPresetDir")))
            {
                setLastPresetDirectory(xmlLastPresetDir->getStringAttribute(juce::String("path")));
            }
        }
        if (xmlKeySync != nullptr)
        {
            if (xmlKeySync->hasTagName(juce::StringRef("keySync")))
            {
                String value = xmlKeySync->getStringAttribute(juce::String("value"));
                if (value == "1")
                    setKeySync(true);
                else
                    setKeySync(false);
            }
        }
    }
}

void GranulizerAudioProcessor::parameterChanged(const String& paramID, float newValue)
{
    if (paramID == NAME_P)
    {
        for (int i = 0; i < synth.getNumVoices(); i++)
        {
            if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
            {
                voice->setPos(newValue);
            }
        }
    }
        

    if (paramID == NAME_SPD)
    {
        for (int i = 0; i < synth.getNumVoices(); i++)
        {
            if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
            {
                voice->setSpeed(newValue);
            }
        }
    }


    if (paramID == NAME_TR)
    {
        for (int i = 0; i < synth.getNumVoices(); i++)
        {
            if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
            {
                voice->setTranspose(newValue);
            }
        }
    }

    if (paramID == NAME_A)
    {
        for (int i = 0; i < synth.getNumVoices(); i++)
        {
            if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
            {
                voice->setAttack(newValue);
            }
        }
    }

    if (paramID == NAME_D)
    {
        for (int i = 0; i < synth.getNumVoices(); i++)
        {
            if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
            {
                voice->setDecay(newValue);
            }
        }
    }

    if (paramID == NAME_S)
    {
        for (int i = 0; i < synth.getNumVoices(); i++)
        {
            if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
            {
                voice->setSustain(newValue);
            }
        }
    }

    if (paramID == NAME_R)
    {
        for (int i = 0; i < synth.getNumVoices(); i++)
        {
            if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
            {
                voice->setRelease(newValue);
            }
        }
    }

    if (paramID == NAME_G)
    {
        for (int i = 0; i < synth.getNumVoices(); i++)
        {
            if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
            {
                voice->setGain(newValue);
            }
        }
    }

    if (paramID == NAME_GN)
    {
        for (int i = 0; i < synth.getNumVoices(); i++)
        {
            if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
            {
                voice->setGrainNumber(newValue);
                voice->resetGrains(newValue);
            }
        }
    }

    if (paramID == NAME_GDMA)
    {
        for (int i = 0; i < synth.getNumVoices(); i++)
        {
            if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
            {
                voice->setTimeMovementAmt(newValue);
            }
        }
    }

    if (paramID == NAME_GDMF)
    {
        for (int i = 0; i < synth.getNumVoices(); i++)
        {
            if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
            {
                voice->setTimeMovementFreq(newValue);
            }
        }
    }

    if (paramID == NAME_GDMWF)
    {
        for (int i = 0; i < synth.getNumVoices(); i++)
        {
            if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
            {
                voice->setTimeMovementWaveform(newValue);
            }
        }
    }

    if (paramID == NAME_RPMA)
    {
        for (int i = 0; i < synth.getNumVoices(); i++)
        {
            if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
            {
                voice->setSpaceMovementAmt(newValue);
            }
        }
    }

    if (paramID == NAME_RPMF)
    {
        for (int i = 0; i < synth.getNumVoices(); i++)
        {
            if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
            {
                voice->setSpaceMovementFreq(newValue);
            }
        }
    }

    if (paramID == NAME_RPMWF)
    {
        for (int i = 0; i < synth.getNumVoices(); i++)
        {
            if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
            {
                voice->setSpaceMovementWaveform(newValue);
            }
        }
    }

    if (paramID == NAME_GD)
    {
        for (int i = 0; i < synth.getNumVoices(); i++)
        {
            if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
            {
                voice->setTimeSpread(newValue);
            }
        }
    }

    if (paramID == NAME_RPS)
    {
        for (int i = 0; i < synth.getNumVoices(); i++)
        {
            if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
            {
                voice->setSpaceSpread(newValue);
            }
        }
    }

    if (paramID == NAME_SC)
    {
        for (int i = 0; i < synth.getNumVoices(); i++)
        {
            if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
            {
                voice->setStartCut(newValue);
            }
        }
    }

    if (paramID == NAME_EC)
    {
        for (int i = 0; i < synth.getNumVoices(); i++)
        {
            if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
            {
                voice->setEndCut(newValue);
            }
        }
    }
}

void GranulizerAudioProcessor::setKeySync(bool newValue)
{
    keySync = newValue;

    for (int i = 0; i < synth.getNumVoices(); i++)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            voice->setKeySync(keySync);
        }
    }
}

void GranulizerAudioProcessor::syncPhases()
{
    for (int i = 0; i < synth.getNumVoices(); i++)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            voice->syncPhases();
        }
    }
}

void GranulizerAudioProcessor::setSample(File file)
{
    sample = file;
}

bool GranulizerAudioProcessor::loadSample()
{
    if (loader.reloadFile(sample))
    {
        for (int i = 0; i < synth.getNumVoices(); i++)
        {
            if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
            {
                voice->setSample(loader.getSampleBuffer());
            }
        }
        isSynthReady = true;
        return true;
    }
    else
    {
        isSynthReady = false;
        return false;
    }
}

void GranulizerAudioProcessor::clearSample()
{
    loader.reset();
}

String GranulizerAudioProcessor::getFileName()
{
    return sample.getFileName();
}

void GranulizerAudioProcessor::setLastSampleDirectory(File dir)
{
    lastSampleDir = dir;
}

File GranulizerAudioProcessor::getLastSampleDirectory()
{
    return lastSampleDir;
}

void GranulizerAudioProcessor::setLastPresetDirectory(File dir)
{
    lastPresetDir = dir;
}

File GranulizerAudioProcessor::getLastPresetDirectory()
{
    return lastPresetDir;
}

bool GranulizerAudioProcessor::getKeySync()
{
    return keySync;
}

int GranulizerAudioProcessor::getFileNumSamples()
{
    return loader.getFileNumSamples();
}

bool GranulizerAudioProcessor::isFileLoaded()
{
    return loader.isFileLoaded();
}

AudioFormatManager& GranulizerAudioProcessor::getFormatManager()
{
    return loader.getFormatManager();
}

File GranulizerAudioProcessor::getFile()
{
    return sample;
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GranulizerAudioProcessor();
}
