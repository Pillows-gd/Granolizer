#pragma once

#include <JuceHeader.h>
#include "Granulizer.h"
#include "Oscillators.h"
#include "Grains.h"

class GranulizerAudioProcessor  : public juce::AudioProcessor, public AudioProcessorValueTreeState::Listener
{
public:
    GranulizerAudioProcessor();
    ~GranulizerAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override                                             { return true; }

    const juce::String getName() const override                                 { return JucePlugin_Name; }

    bool acceptsMidi() const override                                           { return true; }
    bool producesMidi() const override                                          { return false; }
    bool isMidiEffect() const override                                          { return false; }
    double getTailLengthSeconds() const override                                { return 0.0; }

    int getNumPrograms() override                                               { return 1; }
    int getCurrentProgram() override                                            { return 0; }
    void setCurrentProgram (int index) override                                 {  }
    const juce::String getProgramName (int index) override                      { return {}; }
    void changeProgramName (int index, const juce::String& newName) override    {  }

    int getFileNumSamples();

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;
    
    void setKeySync(bool newValue);
    void syncPhases();
    void setSample(File file);
    bool loadSample();
    void clearSample();
    String getFileName();
    void setLastSampleDirectory(File dir);
    File getLastSampleDirectory();
    void setLastPresetDirectory(File dir);
    File getLastPresetDirectory();
    bool getKeySync();
    bool isFileLoaded();
    AudioFormatManager& getFormatManager();
    File getFile();

    float pos[MAX_VOICES][MAX_GN] = { {-1.0f} };
    int gn = DEFAULT_GN;
    bool keySync = DEFAULT_KEYSYNC;

private:
    
    void parameterChanged(const String& paramID, float newValue) override;

    double sr = 44100;
    AudioProcessorValueTreeState parameters; 
    UndoManager undoManager;
    File sample;
    File lastSampleDir = DEFAULT_DIR;
    File lastPresetDir = DEFAULT_DIR;
    Synthesiser synth;
    FileLoader loader;
    bool isSynthReady = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GranulizerAudioProcessor)
};
