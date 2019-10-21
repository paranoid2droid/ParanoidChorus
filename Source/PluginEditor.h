/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class ParanoidChorusAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    ParanoidChorusAudioProcessorEditor (ParanoidChorusAudioProcessor&);
    ~ParanoidChorusAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ParanoidChorusAudioProcessor& processor;

	Slider mDryWetSlider;
	Label mDryWetLable;
	Slider mDepthSlider;
	Label mDepthLabel;
	Slider mRateSlider;
	Label mRateLabel;
	Slider mPhaseOffsetSlider;
	Label mPhaseOffsetLabel;
	Slider mFeedbackSlider;
	Label mFeedbackLabel;
	Slider mMasterSlider;
	Label mMasterLabel;
	Slider mModulateSlider;
	Label mModulateLabel;

	ComboBox mType;
	Label mTypeLabel;
	ComboBox mOsc;
	Label mOscLabel;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParanoidChorusAudioProcessorEditor)
};
