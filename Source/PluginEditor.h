/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class GranulizerAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                        public juce::Button::Listener,
                                        public juce::Slider::Listener
{
public:
    GranulizerAudioProcessorEditor (GranulizerAudioProcessor& p, AudioProcessorValueTreeState& vts);
    ~GranulizerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void buttonClicked(juce::Button* buttonThatWasClicked) override;
    void sliderValueChanged(juce::Slider* sliderThatWasMoved) override {};

private:
    GranulizerAudioProcessor& processor;
    AudioProcessorValueTreeState& valueTreeState;
    File lastSampleLoc = processor.getLastSampleDirectory();
    File lastPresetLoc = processor.getLastPresetDirectory();
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> posAttachment;
    std::unique_ptr<juce::Slider> posSlider;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> modAmountAttachment;
    std::unique_ptr<juce::Slider>speedSlider;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> transposeAttachment;
    std::unique_ptr<juce::Slider>transposeSlider;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> attackAttachment;
    std::unique_ptr<juce::Slider>attackSlider;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> decayAttachment;
    std::unique_ptr<juce::Slider>decaySlider;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> releaseAttachment;
    std::unique_ptr<juce::Slider>releaseSlider;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> sustainAttachment;
    std::unique_ptr<juce::Slider>sustainSlider;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> gainAttachment;
    std::unique_ptr<juce::Slider>gainSlider;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> grainNumberAttachment;
    std::unique_ptr<juce::Slider>grainNumberSlider;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> timeMovementAmtAttachment;
    std::unique_ptr<juce::Slider>timeMovementAmtSlider;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> timeMovementFreqAttachment;
    std::unique_ptr<juce::Slider>timeMovementFreqSlider;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> timeMovementWaveformAttachment;
    std::unique_ptr<juce::Slider>timeMovementWaveformSlider;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> spaceMovementAmtAttachment;
    std::unique_ptr<juce::Slider>spaceMovementAmtSlider;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> spaceMovementFreqAttachment;
    std::unique_ptr<juce::Slider>spaceMovementFreqSlider;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> spaceMovementWaveformAttachment;
    std::unique_ptr<juce::Slider>spaceMovementWaveformSlider;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> timeSpreadAttachment;
    std::unique_ptr<juce::Slider>timeSpreadSlider;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> spaceSpreadAttachment;
    std::unique_ptr<juce::Slider>spaceSpreadSlider;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> startCutAttachment;
    std::unique_ptr<juce::Slider>startCutSlider;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> endCutAttachment;
    std::unique_ptr<juce::Slider>endCutSlider;
    std::unique_ptr<juce::TextButton> halfButton;
    std::unique_ptr<juce::TextButton> fullButton;
    std::unique_ptr<juce::TextButton> doubleButton;
    std::unique_ptr<juce::TextButton> reverseButton;
    std::unique_ptr<juce::TextButton> loadButton;
    std::unique_ptr<juce::TextButton> panicButton;
    std::unique_ptr<juce::TextButton> loadPresetButton;
    std::unique_ptr<juce::TextButton> savePresetButton;
    std::unique_ptr<juce::TextButton> syncPhasesButton;
    std::unique_ptr<juce::ToggleButton> keySyncToggle;
    std::unique_ptr<juce::Label> msg;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GranulizerAudioProcessorEditor)
};
