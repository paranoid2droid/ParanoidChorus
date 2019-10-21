/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ParanoidChorusAudioProcessorEditor::ParanoidChorusAudioProcessorEditor (ParanoidChorusAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

	auto& params = processor.getParameters();

	// set dry wet slider
	AudioParameterFloat* dryWetParameter = (AudioParameterFloat*)params.getUnchecked(0);

	mDryWetSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	mDryWetSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
	mDryWetSlider.setRange(dryWetParameter->range.start, dryWetParameter->range.end);
	mDryWetSlider.setValue(*dryWetParameter);
	addAndMakeVisible(mDryWetSlider);

	mDryWetLable.setText("mix", dontSendNotification);
	mDryWetLable.attachToComponent(&mDryWetSlider, false);
	addAndMakeVisible(mDryWetLable);

	mDryWetSlider.onValueChange = [this, dryWetParameter] { *dryWetParameter = mDryWetSlider.getValue(); };
	mDryWetSlider.onDragStart = [dryWetParameter] { dryWetParameter->beginChangeGesture(); };
	mDryWetSlider.onDragEnd = [dryWetParameter] { dryWetParameter->endChangeGesture(); };

	// set depth slider
	AudioParameterFloat* depthParameter = (AudioParameterFloat*)params.getUnchecked(1);

	mDepthSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	mDepthSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
	mDepthSlider.setRange(depthParameter->range.start, depthParameter->range.end);
	mDepthSlider.setValue(*depthParameter);
	addAndMakeVisible(mDepthSlider);

	mDepthLabel.setText("depth", dontSendNotification);
	mDepthLabel.attachToComponent(&mDepthSlider, false);
	addAndMakeVisible(mDepthLabel);

	mDepthSlider.onValueChange = [this, depthParameter] { *depthParameter = mDepthSlider.getValue();  };
	mDepthSlider.onDragStart = [depthParameter] { depthParameter->beginChangeGesture();  };
	mDepthSlider.onDragEnd = [depthParameter] { depthParameter->endChangeGesture();  };

	// set rate slider
	AudioParameterFloat* rateParameter = (AudioParameterFloat*)params.getUnchecked(2);

	mRateSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	mRateSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
	mRateSlider.setRange(rateParameter->range.start, rateParameter->range.end);
	mRateSlider.setValue(*rateParameter);
	addAndMakeVisible(mRateSlider);

	mRateLabel.setText("rate", dontSendNotification);
	mRateLabel.attachToComponent(&mRateSlider, false);
	addAndMakeVisible(mRateLabel);

	mRateSlider.onValueChange = [this, rateParameter] { *rateParameter = mRateSlider.getValue();  };
	mRateSlider.onDragStart = [rateParameter] { rateParameter->beginChangeGesture();  };
	mRateSlider.onDragEnd = [rateParameter] { rateParameter->endChangeGesture();  };

	// set phase slider
	AudioParameterFloat* phaseParameter = (AudioParameterFloat*)params.getUnchecked(3);

	mPhaseOffsetSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	mPhaseOffsetSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
	mPhaseOffsetSlider.setRange(phaseParameter->range.start, phaseParameter->range.end);
	mPhaseOffsetSlider.setValue(*phaseParameter);
	addAndMakeVisible(mPhaseOffsetSlider);

	mPhaseOffsetLabel.setText("offset", dontSendNotification);
	mPhaseOffsetLabel.attachToComponent(&mPhaseOffsetSlider, false);
	addAndMakeVisible(mPhaseOffsetLabel);

	mPhaseOffsetSlider.onValueChange = [this, phaseParameter] { *phaseParameter = mPhaseOffsetSlider.getValue();  };
	mPhaseOffsetSlider.onDragStart = [phaseParameter] { phaseParameter->beginChangeGesture();  };
	mPhaseOffsetSlider.onDragEnd = [phaseParameter] { phaseParameter->endChangeGesture();  };

	// set feedback slider
	AudioParameterFloat* feedbackParameter = (AudioParameterFloat*)params.getUnchecked(4);

	mFeedbackSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	mFeedbackSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
	mFeedbackSlider.setRange(feedbackParameter->range.start, feedbackParameter->range.end);
	mFeedbackSlider.setValue(*feedbackParameter);
	addAndMakeVisible(mFeedbackSlider);

	mFeedbackLabel.setText("feedback", dontSendNotification);
	mFeedbackLabel.attachToComponent(&mFeedbackSlider, false);
	addAndMakeVisible(mFeedbackLabel);

	mFeedbackSlider.onValueChange = [this, feedbackParameter] { *feedbackParameter = mFeedbackSlider.getValue();  };
	mFeedbackSlider.onDragStart = [feedbackParameter] { feedbackParameter->beginChangeGesture();  };
	mFeedbackSlider.onDragEnd = [feedbackParameter] { feedbackParameter->endChangeGesture();  };

	// set type combobox
	AudioParameterInt* typeParameter = (AudioParameterInt*)params.getUnchecked(5);

	mType.addItem("paraniod", 1);
	mType.addItem("panic", 2);
	mType.addItem("pill", 3);
	addAndMakeVisible(mType);

	mTypeLabel.setText("mood", dontSendNotification);
	mTypeLabel.attachToComponent(&mType, false);
	addAndMakeVisible(mTypeLabel);

	mType.onChange = [this, typeParameter] {
		typeParameter->beginChangeGesture();
		*typeParameter = mType.getSelectedItemIndex();
		typeParameter->endChangeGesture();
	};

	mType.setSelectedItemIndex(*typeParameter);

	//new features

	// set master slider
	AudioParameterFloat* masterParameter = (AudioParameterFloat*)params.getUnchecked(6);

	mMasterSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	mMasterSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
	mMasterSlider.setRange(masterParameter->range.start, masterParameter->range.end);
	mMasterSlider.setValue(*masterParameter);
	addAndMakeVisible(mMasterSlider);

	mMasterLabel.setText("master", dontSendNotification);
	mMasterLabel.attachToComponent(&mMasterSlider, false);
	addAndMakeVisible(mMasterLabel);

	mMasterSlider.onValueChange = [this, masterParameter] { *masterParameter = mMasterSlider.getValue();  };
	mMasterSlider.onDragStart = [masterParameter] { masterParameter->beginChangeGesture();  };
	mMasterSlider.onDragEnd = [masterParameter] { masterParameter->endChangeGesture();  };

	// set osc combobox
	AudioParameterInt* oscParameter = (AudioParameterInt*)params.getUnchecked(7);

	mOsc.addItem("sin", 1);
	mOsc.addItem("saw", 2);
	mOsc.addItem("tri", 3);
	mOsc.addItem("sqr", 4);
	addAndMakeVisible(mOsc);

	mOscLabel.setText("mode", dontSendNotification);
	mOscLabel.attachToComponent(&mOsc, false);
	addAndMakeVisible(mOscLabel);

	mOsc.onChange = [this, oscParameter] {
		oscParameter->beginChangeGesture();
		*oscParameter = mOsc.getSelectedItemIndex();
		oscParameter->endChangeGesture();
	};

	mOsc.setSelectedItemIndex(*oscParameter);

	// set modulate slider
	AudioParameterFloat* modulateParameter = (AudioParameterFloat*)params.getUnchecked(8);

	//mModulateSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	mModulateSlider.setRange(modulateParameter->range.start, modulateParameter->range.end);
	mModulateSlider.setValue(*modulateParameter);
	mModulateSlider.setTextValueSuffix(" x");
	mModulateSlider.setTextBoxStyle(Slider::TextBoxLeft, false, 100, mModulateSlider.getTextBoxHeight());
	mModulateSlider.setSkewFactorFromMidPoint(500);
	addAndMakeVisible(mModulateSlider);

	mModulateLabel.setText("mod", dontSendNotification);
	mModulateLabel.attachToComponent(&mModulateSlider, true);
	addAndMakeVisible(mModulateLabel);

	mModulateSlider.onValueChange = [this, modulateParameter] { *modulateParameter = mModulateSlider.getValue();  };
	mModulateSlider.onDragStart = [modulateParameter] { modulateParameter->beginChangeGesture();  };
	mModulateSlider.onDragEnd = [modulateParameter] { modulateParameter->endChangeGesture();  };
}

ParanoidChorusAudioProcessorEditor::~ParanoidChorusAudioProcessorEditor()
{
}

//==============================================================================
void ParanoidChorusAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::black);
    g.setFont (12.0f);
    g.drawFittedText ("ParanoidChorus", getLocalBounds(), Justification::centredBottom, 1);
	g.drawFittedText("paranoid2droid", getLocalBounds(), Justification::bottomLeft, 1, 0.7);
	g.drawFittedText("by zhe", getLocalBounds(), Justification::bottomRight, 1, 0.7);
}

void ParanoidChorusAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
	auto sliderLeft = 20;
	auto sliderTop = 30;
	auto size = 90;
	auto margin = 20;
	mDryWetSlider.setBounds(sliderLeft, sliderTop, size, size);
	mDepthSlider.setBounds(sliderLeft+size, sliderTop, size, size);
	mRateSlider.setBounds(sliderLeft+2*size, sliderTop, size, size);
	mPhaseOffsetSlider.setBounds(sliderLeft, sliderTop+size+margin, size, size);
	mFeedbackSlider.setBounds(sliderLeft+size, sliderTop+size+margin, size, size);
	mMasterSlider.setBounds(sliderLeft+2*size, sliderTop+size+margin, size, size);

	mType.setBounds(sliderLeft+3*size+10, sliderTop, size, 20);
	mOsc.setBounds(sliderLeft + 3 * size + 10, sliderTop + size + margin, size, 20);
	mModulateSlider.setBounds(sliderLeft+20, sliderTop + 2*size + margin+10, getWidth() - sliderLeft - 30, 30);

}
