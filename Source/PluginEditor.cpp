/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GranulizerAudioProcessorEditor::GranulizerAudioProcessorEditor (GranulizerAudioProcessor& p, AudioProcessorValueTreeState& vst)
    : AudioProcessorEditor(&p), processor(p), valueTreeState(vst)
{
    msg.reset(new juce::Label("msg"));
    addAndMakeVisible(msg.get());
    msg->setColour(juce::Label::textColourId, juce::Colours::yellow);
    msg->setText("Choose a sample to hear sound", juce::NotificationType::dontSendNotification);
    msg->setBounds(100, 40, 250, 50);

    grainNumberSlider.reset(new juce::Slider("Grain Number"));
    addAndMakeVisible(grainNumberSlider.get());
    grainNumberSlider->setSliderStyle(juce::Slider::LinearHorizontal);
    grainNumberSlider->setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
    grainNumberSlider->setBounds(100, 70, 320, 32);

    posSlider.reset(new juce::Slider("Position"));
    addAndMakeVisible(posSlider.get());
    //posSlider->setRange(0, 10, 0);
    posSlider->setSliderStyle(juce::Slider::LinearHorizontal);
    posSlider->setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
    posSlider->setBounds(100, 110, 320, 32);

    speedSlider.reset(new juce::Slider("Speed"));
    addAndMakeVisible(speedSlider.get());
    speedSlider->setSliderStyle(juce::Slider::LinearHorizontal);
    speedSlider->setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
    speedSlider->setBounds(100, 150, 320, 32);

    transposeSlider.reset(new juce::Slider("Transpose"));
    addAndMakeVisible(transposeSlider.get());
    transposeSlider->setSliderStyle(juce::Slider::LinearHorizontal);
    transposeSlider->setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
    transposeSlider->setBounds(100, 190, 320, 32);

    attackSlider.reset(new juce::Slider("Attack"));
    addAndMakeVisible(attackSlider.get());
    attackSlider->setSliderStyle(juce::Slider::LinearHorizontal);
    attackSlider->setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
    attackSlider->setBounds(100, 230, 320, 32);

    decaySlider.reset(new juce::Slider("Decay"));
    addAndMakeVisible(decaySlider.get());
    decaySlider->setSliderStyle(juce::Slider::LinearHorizontal);
    decaySlider->setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
    decaySlider->setBounds(100, 270, 320, 32);

    sustainSlider.reset(new juce::Slider("Sustain"));
    addAndMakeVisible(sustainSlider.get());
    sustainSlider->setSliderStyle(juce::Slider::LinearHorizontal);
    sustainSlider->setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
    sustainSlider->setBounds(100, 310, 320, 32);

    releaseSlider.reset(new juce::Slider("Release"));
    addAndMakeVisible(releaseSlider.get());
    releaseSlider->setSliderStyle(juce::Slider::LinearHorizontal);
    releaseSlider->setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
    releaseSlider->setBounds(100, 350, 320, 32);

    timeMovementAmtSlider.reset(new juce::Slider("Grain Dispersion Movement Amount"));
    addAndMakeVisible(timeMovementAmtSlider.get());
    timeMovementAmtSlider->setSliderStyle(juce::Slider::LinearHorizontal);
    timeMovementAmtSlider->setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
    timeMovementAmtSlider->setBounds(100, 390, 320, 32);

    timeMovementFreqSlider.reset(new juce::Slider("Grain Dispersion Movement Frequency"));
    addAndMakeVisible(timeMovementFreqSlider.get());
    timeMovementFreqSlider->setSliderStyle(juce::Slider::LinearHorizontal);
    timeMovementFreqSlider->setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
    timeMovementFreqSlider->setBounds(100, 430, 320, 32);
    
    timeMovementWaveformSlider.reset(new juce::Slider("Grain Dispersion Movement Waveform"));
    addAndMakeVisible(timeMovementWaveformSlider.get());
    timeMovementWaveformSlider->setSliderStyle(juce::Slider::LinearHorizontal);
    timeMovementWaveformSlider->setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
    timeMovementWaveformSlider->setBounds(100, 470, 320, 32);

    spaceMovementAmtSlider.reset(new juce::Slider("Read Position Movement Amount"));
    addAndMakeVisible(spaceMovementAmtSlider.get());
    spaceMovementAmtSlider->setSliderStyle(juce::Slider::LinearHorizontal);
    spaceMovementAmtSlider->setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
    spaceMovementAmtSlider->setBounds(100, 510, 320, 32);

    spaceMovementFreqSlider.reset(new juce::Slider("Read Position Movement Frequency"));
    addAndMakeVisible(spaceMovementFreqSlider.get());
    spaceMovementFreqSlider->setSliderStyle(juce::Slider::LinearHorizontal);
    spaceMovementFreqSlider->setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
    spaceMovementFreqSlider->setBounds(100, 550, 320, 32);
    
    spaceMovementWaveformSlider.reset(new juce::Slider("Read Position Movement Waveform"));
    addAndMakeVisible(spaceMovementWaveformSlider.get());
    spaceMovementWaveformSlider->setSliderStyle(juce::Slider::LinearHorizontal);
    spaceMovementWaveformSlider->setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
    spaceMovementWaveformSlider->setBounds(100, 590, 320, 32);

    timeSpreadSlider.reset(new juce::Slider("Grain Dispersion"));
    addAndMakeVisible(timeSpreadSlider.get());
    timeSpreadSlider->setSliderStyle(juce::Slider::LinearHorizontal);
    timeSpreadSlider->setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
    timeSpreadSlider->setBounds(100, 630, 320, 32);

    spaceSpreadSlider.reset(new juce::Slider("Read Position Spread"));
    addAndMakeVisible(spaceSpreadSlider.get());
    spaceSpreadSlider->setSliderStyle(juce::Slider::LinearHorizontal);
    spaceSpreadSlider->setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
    spaceSpreadSlider->setBounds(100, 670, 320, 32);

    startCutSlider.reset(new juce::Slider("Start Cut"));
    addAndMakeVisible(startCutSlider.get());
    startCutSlider->setSliderStyle(juce::Slider::LinearHorizontal);
    startCutSlider->setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
    startCutSlider->setBounds(100, 710, 320, 32);

    endCutSlider.reset(new juce::Slider("End Cut"));
    addAndMakeVisible(endCutSlider.get());
    endCutSlider->setSliderStyle(juce::Slider::LinearHorizontal);
    endCutSlider->setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
    endCutSlider->setBounds(100, 750, 320, 32);

    gainSlider.reset(new juce::Slider("Gain"));
    addAndMakeVisible(gainSlider.get());
    gainSlider->setSliderStyle(juce::Slider::LinearHorizontal);
    gainSlider->setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
    gainSlider->setBounds(100, 790, 320, 32);

    halfButton.reset(new juce::TextButton("x0.5"));
    addAndMakeVisible(halfButton.get());
    halfButton->addListener(this);
    // OLD: halfButton->setBounds(100, 840, 55, 40);
    halfButton->setBounds(20, 80, 55, 40);

    fullButton.reset(new juce::TextButton("x1.0"));
    addAndMakeVisible(fullButton.get());
    fullButton->addListener(this);
    // OLD: fullButton->setBounds(200, 840, 55, 40);
    fullButton->setBounds(20, 160, 55, 40);

    doubleButton.reset(new juce::TextButton("x2.0"));
    addAndMakeVisible(doubleButton.get());
    doubleButton->addListener(this);
    // OLD: doubleButton->setBounds(300, 840, 55, 40);
    doubleButton->setBounds(20, 240, 55, 40);

    reverseButton.reset(new juce::TextButton("Reverse Sample"));
    addAndMakeVisible(reverseButton.get());
    reverseButton->addListener(this);
    // OLD: reverseButton->setBounds(300, 870, 55, 40);
    reverseButton->setBounds(20, 16, 55, 40);

    loadButton.reset(new juce::TextButton("Load Sample"));
    addAndMakeVisible(loadButton.get());
    loadButton->addListener(this);
    loadButton->setBounds(200, 16, 60, 40);

    panicButton.reset(new juce::TextButton("Panic"));
    addAndMakeVisible(panicButton.get());
    panicButton->addListener(this);
    panicButton->setBounds(100, 16, 50, 35);

    loadPresetButton.reset(new juce::TextButton("Load Preset"));
    addAndMakeVisible(loadPresetButton.get());
    loadPresetButton->addListener(this);
    loadPresetButton->setBounds(300, 16, 80, 24);

    savePresetButton.reset(new juce::TextButton("Save Preset"));
    addAndMakeVisible(savePresetButton.get());
    savePresetButton->addListener(this);
    savePresetButton->setBounds(392, 16, 80, 24);

    syncPhasesButton.reset(new juce::TextButton("Syncronize Phases"));
    addAndMakeVisible(syncPhasesButton.get());
    syncPhasesButton->addListener(this);
    syncPhasesButton->setBounds(20, 500, 55, 40);

    keySyncToggle.reset(new juce::ToggleButton("Key Sync"));
    addAndMakeVisible(keySyncToggle.get());
    keySyncToggle->addListener(this);
    keySyncToggle->setBounds(20, 350, 55, 100);


    // ATTACHMENTS
    {
        posAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, NAME_P, *posSlider));
        modAmountAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, NAME_SPD, *speedSlider));
        transposeAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, NAME_TR, *transposeSlider));
        attackAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, NAME_A, *attackSlider));
        decayAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, NAME_D, *decaySlider));
        sustainAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, NAME_S, *sustainSlider));
        releaseAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, NAME_R, *releaseSlider));
        gainAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, NAME_G, *gainSlider));
        grainNumberAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, NAME_GN, *grainNumberSlider));
        timeMovementAmtAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, NAME_GDMA, *timeMovementAmtSlider));
        timeMovementFreqAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, NAME_GDMF, *timeMovementFreqSlider));
        timeMovementWaveformAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, NAME_GDMWF, *timeMovementWaveformSlider));
        spaceMovementAmtAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, NAME_RPMA, *spaceMovementAmtSlider));
        spaceMovementFreqAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, NAME_RPMF, *spaceMovementFreqSlider));
        spaceMovementWaveformAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, NAME_RPMWF, *spaceMovementWaveformSlider));
        timeSpreadAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, NAME_GD, *timeSpreadSlider));
        spaceSpreadAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, NAME_RPS, *spaceSpreadSlider));
        startCutAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, NAME_SC, *startCutSlider));
        endCutAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, NAME_EC, *endCutSlider));
    }

    setSize (500, 850);
}

GranulizerAudioProcessorEditor::~GranulizerAudioProcessorEditor()
{
    posAttachment.reset();
    modAmountAttachment.reset();
    transposeAttachment.reset();
    attackAttachment.reset();
    decayAttachment.reset();
    sustainAttachment.reset();
    releaseAttachment.reset();
    gainAttachment.reset();
    grainNumberAttachment.reset();
    timeMovementAmtAttachment.reset();
    timeMovementFreqAttachment.reset();
    timeMovementWaveformAttachment.reset();
    spaceMovementAmtAttachment.reset();
    spaceMovementFreqAttachment.reset();
    spaceMovementWaveformAttachment.reset();
    timeSpreadAttachment.reset();
    spaceSpreadAttachment.reset();
    startCutAttachment.reset();
    endCutAttachment.reset();

    posSlider = nullptr;
    speedSlider = nullptr;
    transposeSlider = nullptr;
    attackSlider = nullptr;
    decaySlider = nullptr;
    sustainSlider = nullptr;
    releaseSlider = nullptr;
    gainSlider = nullptr;
    grainNumberSlider = nullptr;
    timeMovementAmtSlider = nullptr;
    timeMovementFreqSlider = nullptr;
    timeMovementWaveformSlider = nullptr;
    spaceMovementAmtSlider = nullptr;
    spaceMovementFreqSlider = nullptr;
    spaceMovementWaveformSlider = nullptr;
    timeSpreadSlider = nullptr;
    spaceSpreadSlider = nullptr;
    startCutSlider = nullptr;
    endCutSlider = nullptr;
    keySyncToggle = nullptr;
    halfButton = nullptr;
    fullButton = nullptr;
    doubleButton = nullptr;
    reverseButton = nullptr;
    loadButton = nullptr;
    panicButton = nullptr;
    loadPresetButton = nullptr;
    savePresetButton = nullptr;
    syncPhasesButton = nullptr;
}

//==============================================================================
void GranulizerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    Rectangle<int> grainNumberRect;
    grainNumberRect.setBounds(100, 70, 200, 50);
    Rectangle<int> posRect;
    posRect.setBounds(100, 110, 200, 50);
    Rectangle<int> modAmountRect;
    modAmountRect.setBounds(100, 150, 200, 50);
    Rectangle<int> transposeRect;
    transposeRect.setBounds(100, 190, 200, 50);
    Rectangle<int> attackRect;
    attackRect.setBounds(100, 230, 200, 50);
    Rectangle<int> decayRect;
    decayRect.setBounds(100, 270, 200, 50);
    Rectangle<int> sustainRect;
    sustainRect.setBounds(100, 310, 200, 50);
    Rectangle<int> releaseRect;
    releaseRect.setBounds(100, 350, 200, 50);
    Rectangle<int> timeMovementAmtRect;
    timeMovementAmtRect.setBounds(100, 390, 200, 50);
    Rectangle<int> timeMovementFreqRect;
    timeMovementFreqRect.setBounds(100, 430, 200, 50);
    Rectangle<int> timeMovementWaveformRect;
    timeMovementWaveformRect.setBounds(100, 470, 200, 50);
    Rectangle<int> spaceMovementAmtRect;
    spaceMovementAmtRect.setBounds(100, 510, 200, 50);
    Rectangle<int> spaceMovementFreqRect;
    spaceMovementFreqRect.setBounds(100, 550, 200, 50);
    Rectangle<int> spaceMovementWaveformRect;
    spaceMovementWaveformRect.setBounds(100, 590, 200, 50);
    Rectangle<int> timeSpreadRect;
    timeSpreadRect.setBounds(100, 630, 200, 50);
    Rectangle<int> spaceSpreadRect;
    spaceSpreadRect.setBounds(100, 670, 200, 50);
    Rectangle<int> startCutRect;
    startCutRect.setBounds(100, 710, 200, 50);
    Rectangle<int> endCutRect;
    endCutRect.setBounds(100, 750, 200, 50);
    Rectangle<int> gainRect;
    gainRect.setBounds(100, 790, 200, 50);
    g.drawFittedText("Position", posRect, juce::Justification::centred, 1);
    g.drawFittedText("Speed", modAmountRect, juce::Justification::centred, 1);
    g.drawFittedText("Transpose [semitones]", transposeRect, juce::Justification::centred, 1);
    g.drawFittedText("Attack", attackRect, juce::Justification::centred, 1);
    g.drawFittedText("Decay", decayRect, juce::Justification::centred, 1);
    g.drawFittedText("Sustain", sustainRect, juce::Justification::centred, 1);
    g.drawFittedText("Release", releaseRect, juce::Justification::centred, 1);
    g.drawFittedText("Grain Spread Movement Amount [%]", timeMovementAmtRect, juce::Justification::centred, 1);
    g.drawFittedText("Grain Spread Movement Frequency [Hz]", timeMovementFreqRect, juce::Justification::centred, 1);
    g.drawFittedText("Grain Spread Movement Waveform", timeMovementWaveformRect, juce::Justification::centred, 1);
    g.drawFittedText("Read Position Movement Amount [%]", spaceMovementAmtRect, juce::Justification::centred, 1);
    g.drawFittedText("Read Position Movement Frequency [Hz]", spaceMovementFreqRect, juce::Justification::centred, 1);
    g.drawFittedText("Read Position Movement Waveform", spaceMovementWaveformRect, juce::Justification::centred, 1);
    g.drawFittedText("Grain Dispersion [%]", timeSpreadRect, juce::Justification::centred, 1);
    g.drawFittedText("Read Position Spread [%]", spaceSpreadRect, juce::Justification::centred, 1);
    g.drawFittedText("Start Cut", startCutRect, juce::Justification::centred, 1);
    g.drawFittedText("End Cut", endCutRect, juce::Justification::centred, 1);
    g.drawFittedText("Gain [dB]", gainRect, juce::Justification::centred, 1);
    g.drawFittedText("Grain Number", grainNumberRect, juce::Justification::centred, 1);
}

void GranulizerAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void GranulizerAudioProcessorEditor::buttonClicked(juce::Button* buttonThatWasClicked)
{

    if (buttonThatWasClicked == loadButton.get())
    {
        //[UserButtonCode_loadButton] -- add your button handler code here..

        juce::FileChooser chooser("Select sample to load...", lastSampleLoc, "*.wav");

        if (chooser.browseForFileToOpen())
        {
            processor.clearSample();
            auto fileToLoad = chooser.getResult();
            processor.setSample(fileToLoad);
            processor.loadSample();
            processor.setLastSampleDirectory(chooser.getResult().getParentDirectory());
            lastSampleLoc = chooser.getResult().getParentDirectory();
        }

        if (processor.isFileLoaded())
            msg->setVisible(false);
        else
            msg->setVisible(true);

        //[/UserButtonCode_loadButton]
    }

    if (buttonThatWasClicked == panicButton.get())
    {
        //[UserButtonCode_loadButton] -- add your button handler code here..
        posSlider->setValue(DEFAULT_P);
        speedSlider->setValue(DEFAULT_S);
        transposeSlider->setValue(DEFAULT_TR);
        attackSlider->setValue(DEFAULT_A);
        decaySlider->setValue(DEFAULT_D);
        sustainSlider->setValue(DEFAULT_S);
        releaseSlider->setValue(DEFAULT_R);
        gainSlider->setValue(DEFAULT_G);
        grainNumberSlider->setValue(DEFAULT_GN);
        timeMovementAmtSlider->setValue(DEFAULT_GDMA);
        timeMovementFreqSlider->setValue(DEFAULT_GDMF);
        timeMovementWaveformSlider->setValue(DEFAULT_GDMWF);
        spaceMovementAmtSlider->setValue(DEFAULT_RPMA);
        spaceMovementFreqSlider->setValue(DEFAULT_RPMF);
        spaceMovementWaveformSlider->setValue(DEFAULT_RPMWF);
        timeSpreadSlider->setValue(DEFAULT_GD);
        spaceSpreadSlider->setValue(DEFAULT_RPS);
        startCutSlider->setValue(DEFAULT_SC);
        endCutSlider->setValue(DEFAULT_EC);
        //[/UserButtonCode_loadButton]
    }

    if (buttonThatWasClicked == halfButton.get())
    {
        //[UserButtonCode_loadButton] -- add your button handler code here..
        spaceMovementFreqSlider->setValue(processor.getSampleRate() / (2 * processor.getFileNumSamples() * abs(endCutSlider->getValue() - startCutSlider->getValue())));
        spaceMovementAmtSlider->setValue(1);
        spaceMovementWaveformSlider->setValue(WF_SAW_UP);
        grainNumberSlider->setValue(jmax((int)grainNumberSlider->getValue(), 2));
        spaceSpreadSlider->setValue(0.0f);
        timeSpreadSlider->setValue(0.5f);
        timeMovementAmtSlider->setValue(0);
        //[/UserButtonCode_loadButton]
    }

    if (buttonThatWasClicked == fullButton.get())
    {
        //[UserButtonCode_loadButton] -- add your button handler code here..
        spaceMovementFreqSlider->setValue(processor.getSampleRate() / (processor.getFileNumSamples() * abs(endCutSlider->getValue() - startCutSlider->getValue())));
        spaceMovementAmtSlider->setValue(1);
        spaceMovementWaveformSlider->setValue(WF_SAW_UP);
        grainNumberSlider->setValue(jmax((int)grainNumberSlider->getValue(), 2));
        spaceSpreadSlider->setValue(0.0f);
        timeSpreadSlider->setValue(0.5f);
        timeMovementAmtSlider->setValue(0);
        //[/UserButtonCode_loadButton]
    }

    if (buttonThatWasClicked == doubleButton.get())
    {
        //[UserButtonCode_loadButton] -- add your button handler code here..
        spaceMovementFreqSlider->setValue(2 * processor.getSampleRate() / (processor.getFileNumSamples() * abs(endCutSlider->getValue() - startCutSlider->getValue())));
        spaceMovementAmtSlider->setValue(1);
        spaceMovementWaveformSlider->setValue(WF_SAW_UP);
        grainNumberSlider->setValue(jmax((int)grainNumberSlider->getValue(), 2));
        spaceSpreadSlider->setValue(0.0f);
        timeSpreadSlider->setValue(0.5f);
        timeMovementAmtSlider->setValue(0);
        //[/UserButtonCode_loadButton]
    }

    if (buttonThatWasClicked == reverseButton.get())
    {
        //[UserButtonCode_loadButton] -- add your button handler code here..
        auto newStart = endCutSlider->getValue();
        auto newEnd = startCutSlider->getValue();
        startCutSlider->setValue(newStart);
        endCutSlider->setValue(newEnd);
        //[/UserButtonCode_loadButton]
    }

    if (buttonThatWasClicked == loadPresetButton.get())
    {
        //[UserButtonCode_loadButton] -- add your button handler code here..
        juce::FileChooser chooser("Select preset to load...", lastPresetLoc, "*.xml");

        if (chooser.browseForFileToOpen())
        {
            auto fileToLoad = chooser.getResult();
            MemoryBlock sourceData;
            fileToLoad.loadFileAsData(sourceData);
            processor.setStateInformation(sourceData.getData(), sourceData.getSize());
            processor.setLastPresetDirectory(chooser.getResult().getParentDirectory());
            lastPresetLoc = chooser.getResult().getParentDirectory();
        }

        if (processor.isFileLoaded())
            msg->setVisible(false);
        else
            msg->setVisible(true);
        //[/UserButtonCode_loadButton]
    }

    if (buttonThatWasClicked == savePresetButton.get())
    {
        //[UserButtonCode_saveButton] -- add your button handler code here..
        juce::FileChooser chooser("Select save position...", lastPresetLoc, "*.xml");

        if (chooser.browseForFileToSave(true))
        {
            auto file = chooser.getResult();

            if (file.existsAsFile())
                file.deleteFile();

            juce::FileOutputStream outputStream(file);

            if (outputStream.openedOk())
            {
                MemoryBlock destData;
                processor.getStateInformation(destData);
                outputStream.write(destData.getData(), destData.getSize());
                processor.setLastPresetDirectory(chooser.getResult().getParentDirectory());
                lastPresetLoc = chooser.getResult().getParentDirectory();
            }
        }
        //[/UserButtonCode_saveButton]
    }

    if (buttonThatWasClicked == keySyncToggle.get())
    {
        if (keySyncToggle->getToggleStateValue() == true)
            processor.setKeySync(true);
        else
            processor.setKeySync(false);
    }

    if (buttonThatWasClicked == syncPhasesButton.get())
    {
        processor.syncPhases();
    }
}
