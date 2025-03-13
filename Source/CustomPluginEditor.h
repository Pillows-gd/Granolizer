/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.1.6

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "MyTheme.h"
#include "Bar.h"

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class CustomPluginEditor  : public juce::AudioProcessorEditor,
                            public juce::Button::Listener,
                            public juce::Slider::Listener,
                            public juce::ChangeListener
{
public:
    //==============================================================================
    CustomPluginEditor (GranulizerAudioProcessor &p, AudioProcessorValueTreeState& vts);
    ~CustomPluginEditor() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void buttonClicked(juce::Button* buttonThatWasClicked) override;
    void sliderValueChanged(juce::Slider* sliderThatWasMoved) override;
    void updateThumbnail();
    void paintThumbnail(juce::Graphics& g);
    void paintIfNoFileLoaded(juce::Graphics& g);
    void paintIfFileLoaded(juce::Graphics& g);
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    void thumbnailChanged();
    void paintGrains();
    void paintCuts();
    void paintShape(Slider* slider, ImageComponent* image);
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;

    // Binary resources:
    static const char* adsrScaled_png;
    static const int adsrScaled_pngSize;
    static const char* button_png;
    static const int button_pngSize;
    static const char* buttonDown_png;
    static const int buttonDown_pngSize;
    static const char* earScaled_png;
    static const int earScaled_pngSize;
    static const char* frameScaled_png;
    static const int frameScaled_pngSize;
    static const char* grain1_png;
    static const int grain1_pngSize;
    static const char* grain2_png;
    static const int grain2_pngSize;
    static const char* grain3_png;
    static const int grain3_pngSize;
    static const char* grain4_png;
    static const int grain4_pngSize;
    static const char* grain5_png;
    static const int grain5_pngSize;
    static const char* grain6_png;
    static const int grain6_pngSize;
    static const char* grain7_png;
    static const int grain7_pngSize;
    static const char* keySync_png;
    static const int keySync_pngSize;
    static const char* knob_png;
    static const int knob_pngSize;
    static const char* modsScaled_png;
    static const int modsScaled_pngSize;
    static const char* nameScaled_png;
    static const int nameScaled_pngSize;
    static const char* slider_png;
    static const int slider_pngSize;
    static const char* wallpaperScaled_png;
    static const int wallpaperScaled_pngSize;
    static const char* lim_png;
    static const int lim_pngSize;
    static const char* limScaled_png;
    static const int limScaled_pngSize;
    static const char* tinyButton_png;
    static const int tinyButton_pngSize;
    static const char* tinyButtonDown_png;
    static const int tinyButtonDown_pngSize;
    static const char* sine_png;
    static const int sine_pngSize;
    static const char* tri_png;
    static const int tri_pngSize;
    static const char* sawUp_png;
    static const int sawUp_pngSize;
    static const char* sawDown_png;
    static const int sawDown_pngSize;
    static const char* rect_png;
    static const int rect_pngSize;
    static const char* snh_png;
    static const int snh_pngSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    GranulizerAudioProcessor& processor;
    AudioProcessorValueTreeState& valueTreeState;
    MyLookAndFeel theme;
    MyLookAndFeel themeStart;
    MyLookAndFeel themeEnd;
    Array<float> alphas[MAX_GN - 1] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
    juce::Rectangle<int> thumbnailBounds{ 164, 93, 471, 71 };

    //File lastSampleLoc = processor.getLastSampleDirectory();
    //File lastPresetLoc = processor.getLastPresetDirectory();
    juce::AudioThumbnailCache thumbnailCache;
    juce::AudioThumbnail thumbnail;
    juce::AudioDeviceManager deviceManager;

    std::unique_ptr<SliderAttachment> posAttachment;
    std::unique_ptr<SliderAttachment> modAmountAttachment;
    std::unique_ptr<SliderAttachment> transposeAttachment;
    std::unique_ptr<SliderAttachment> attackAttachment;
    std::unique_ptr<SliderAttachment> decayAttachment;
    std::unique_ptr<SliderAttachment> releaseAttachment;
    std::unique_ptr<SliderAttachment> sustainAttachment;
    std::unique_ptr<SliderAttachment> gainAttachment;
    std::unique_ptr<SliderAttachment> grainNumberAttachment;
    std::unique_ptr<SliderAttachment> dispersionModAmtAttachment;
    std::unique_ptr<SliderAttachment> dispersionModFreqAttachment;
    std::unique_ptr<SliderAttachment> dispersionModWaveformAttachment;
    std::unique_ptr<SliderAttachment> posModAmtAttachment;
    std::unique_ptr<SliderAttachment> posModFreqAttachment;
    std::unique_ptr<SliderAttachment> posModWaveformAttachment;
    std::unique_ptr<SliderAttachment> dispersionAttachment;
    std::unique_ptr<SliderAttachment> posSpreadAttachment;
    std::unique_ptr<SliderAttachment> startCutAttachment;
    std::unique_ptr<SliderAttachment> endCutAttachment;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<CutBar> leftCut;
    std::unique_ptr<Meter> bar;
    std::unique_ptr<juce::ImageComponent> grain7;
    std::unique_ptr<juce::ToggleButton> keySyncToggle;
    std::unique_ptr<juce::Slider> speedSlider;
    std::unique_ptr<juce::Slider> posSpreadSlider;
    std::unique_ptr<juce::Slider> dispersionSlider;
    std::unique_ptr<juce::Slider> posModFreqSlider;
    std::unique_ptr<juce::Slider> posModAmtSlider;
    std::unique_ptr<juce::Slider> dispersionModAmtSlider;
    std::unique_ptr<juce::Slider> dispersionModFreqSlider;
    std::unique_ptr<juce::Slider> grainNumberSlider;
    std::unique_ptr<juce::Slider> gainSlider;
    std::unique_ptr<juce::Slider> attackSlider;
    std::unique_ptr<juce::Slider> decaySlider;
    std::unique_ptr<juce::Slider> sustainSlider;
    std::unique_ptr<juce::Slider> releaseSlider;
    std::unique_ptr<juce::Slider> posModWFSlider;
    std::unique_ptr<juce::Slider> dispersionModWFSlider;
    std::unique_ptr<juce::Slider> posSlider;
    std::unique_ptr<juce::ImageButton> syncVoicesButton;
    std::unique_ptr<juce::ImageButton> loadButton;
    std::unique_ptr<juce::ImageButton> reverseButton;
    std::unique_ptr<juce::ImageButton> panicButton;
    std::unique_ptr<juce::ImageButton> savePresetButton;
    std::unique_ptr<juce::ImageButton> loadPresetButton;
    std::unique_ptr<juce::ImageButton> undoButton;
    std::unique_ptr<juce::ImageButton> redoButton;
    std::unique_ptr<juce::ImageButton> halfButton;
    std::unique_ptr<juce::ImageButton> fullButton;
    std::unique_ptr<juce::ImageButton> doubleButton;
    std::unique_ptr<juce::Slider> transposeSlider;
    std::unique_ptr<juce::ImageComponent> grain1;
    std::unique_ptr<juce::ImageComponent> grain2;
    std::unique_ptr<juce::ImageComponent> grain3;
    std::unique_ptr<juce::ImageComponent> grain4;
    std::unique_ptr<juce::ImageComponent> grain5;
    std::unique_ptr<juce::ImageComponent> grain6;
    std::unique_ptr<juce::ImageComponent> ear;
    std::unique_ptr<CutBar> rightCut;
    std::unique_ptr<juce::Slider> endCutSlider;
    std::unique_ptr<juce::Slider> startCutSlider;
    std::unique_ptr<juce::ImageComponent> dispersionShape;
    std::unique_ptr<juce::ImageComponent> positionShape;
    std::unique_ptr<juce::Drawable> drawable1;
    std::unique_ptr<juce::Drawable> drawable2;
    std::unique_ptr<juce::Drawable> drawable3;
    std::unique_ptr<juce::Drawable> drawable4;
    std::unique_ptr<juce::Drawable> drawable5;
    std::unique_ptr<juce::Drawable> drawable6;
    std::unique_ptr<juce::Drawable> drawable7;
    std::unique_ptr<juce::Drawable> drawable8;
    std::unique_ptr<juce::Drawable> drawable9;
    std::unique_ptr<juce::Drawable> drawable10;
    std::unique_ptr<juce::Drawable> drawable11;
    std::unique_ptr<juce::Drawable> drawable12;
    std::unique_ptr<juce::Drawable> drawable13;
    std::unique_ptr<juce::Drawable> drawable14;
    std::unique_ptr<juce::Drawable> drawable15;
    std::unique_ptr<juce::Drawable> drawable16;
    std::unique_ptr<juce::Drawable> drawable17;
    std::unique_ptr<juce::Drawable> drawable18;
    std::unique_ptr<juce::Drawable> drawable19;
    std::unique_ptr<juce::Drawable> drawable20;
    std::unique_ptr<juce::Drawable> drawable21;
    std::unique_ptr<juce::Drawable> drawable22;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomPluginEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

