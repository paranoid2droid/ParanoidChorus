/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ParanoidChorusAudioProcessor::ParanoidChorusAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
	//construct and add parameters
	addParameter(mDryWetParameter = new AudioParameterFloat("drywet", "Dry Wet", 0.0, 1.0, 0.5));
	addParameter(mDepthParameter = new AudioParameterFloat("depth", "Depth", 0.0, 1.0, 0.5));
	addParameter(mRateParameter = new AudioParameterFloat("rate", "Rate", 0.1f, 20.f, 10.f));
	addParameter(mPhaseOffsetParameter = new AudioParameterFloat("phaseoffset", "Phase Offset", 0.0f, 1.f, 0.5f));
	addParameter(mFeedbackParameter = new AudioParameterFloat("feedback", "Feedback", 0, 0.98, 0.5));
	addParameter(mType = new AudioParameterInt("type", "Type", 0, 2, 0));
	addParameter(mMasterParameter = new AudioParameterFloat("master", "Master", 0, 1.0, 0.5));
	addParameter(mOsc = new AudioParameterInt("osc", "Osc", 0, 3, 0));
	addParameter(mModulateParameter = new AudioParameterFloat("modulate", "Modulate", 1, 2000.0, 1));

	//initialize variables
	mCircularBufferLeft = nullptr;
	mCircularBufferRight = nullptr;
	mCircularBufferWriteHead = 0;
	mCircularBufferLength = 0;

	mFeedbackLeft = 0;
	mFeedbackRight = 0;

	mLFOPhase = 0;
}

ParanoidChorusAudioProcessor::~ParanoidChorusAudioProcessor()
{
}

//==============================================================================
const String ParanoidChorusAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ParanoidChorusAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ParanoidChorusAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ParanoidChorusAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ParanoidChorusAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ParanoidChorusAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ParanoidChorusAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ParanoidChorusAudioProcessor::setCurrentProgram (int index)
{
}

const String ParanoidChorusAudioProcessor::getProgramName (int index)
{
    return {};
}

void ParanoidChorusAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void ParanoidChorusAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

	//LFO phase
	mLFOPhase = 0;

	//calculate circular buffer length
	mCircularBufferLength = sampleRate * MAX_DELAY_TIME;

	//initialize buffers and clean junks
	if (mCircularBufferLeft == nullptr) {
		mCircularBufferLeft = new float[mCircularBufferLength];
	}
	zeromem(mCircularBufferLeft, mCircularBufferLength * sizeof(float));

	if (mCircularBufferRight == nullptr) {
		mCircularBufferRight = new float[mCircularBufferLength];
	}
	zeromem(mCircularBufferRight, mCircularBufferLength * sizeof(float));

	//initialize write head
	mCircularBufferWriteHead = 0;
}

void ParanoidChorusAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ParanoidChorusAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void ParanoidChorusAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

	/* monitor parameters at console */
	DBG("DRY WET: " << *mDryWetParameter);
	DBG("DEPTH: " << *mDepthParameter);
	DBG("RATE: " << *mRateParameter);
	DBG("PHASE: " << *mPhaseOffsetParameter);
	DBG("FEEDBACK: " << *mFeedbackParameter);
	DBG("TYPE: " << (int)*mType);

	/* clear buffers */
	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

	// obtain left and right audio data pointers
	float* leftChannel = buffer.getWritePointer(0);
	float* rightChannel = buffer.getWritePointer(1);

	// iterate all samples in buffer
	for (int i = 0; i < buffer.getNumSamples(); i++) {
		// wirte feedback into buffer
		mCircularBufferLeft[mCircularBufferWriteHead] = leftChannel[i] + mFeedbackLeft;
		mCircularBufferRight[mCircularBufferWriteHead] = rightChannel[i] + mFeedbackRight;

		//calculate left LFO output
		float lfoOutLeft = 0;
		if (*mOsc == 1) {  lfoOutLeft = saw(mLFOPhase); }
		else if (*mOsc == 2) {  lfoOutLeft = tri(mLFOPhase); }
		else if (*mOsc == 3) {  lfoOutLeft = sqr(mLFOPhase); }
		else {  lfoOutLeft = sin(2 * PI * mLFOPhase); }

		//calculate right LFO output with offset
		float lfoPhaseRight = mLFOPhase + *mPhaseOffsetParameter;
		if (lfoPhaseRight > 1) { lfoPhaseRight -= 1; }

		float lfoOutRight = 0;
		if (*mOsc == 1) {  lfoOutRight = saw(lfoPhaseRight); }
		else if (*mOsc == 2) {  lfoOutRight = tri(lfoPhaseRight); }
		else if (*mOsc == 3) {  lfoOutRight = sqr(lfoPhaseRight); }
		else {  lfoOutRight = sin(2 * PI * lfoPhaseRight); }

		//move lfo phase forward
		mLFOPhase += *mRateParameter * *mModulateParameter / getSampleRate();
		if (mLFOPhase > 1) { mLFOPhase -= 1; }

		//control lfo depth
		lfoOutLeft *= *mDepthParameter;
		lfoOutRight *= *mDepthParameter;

		/* initialize mapping */
		float lfoOutMappedLeft = 0;
		float lfoOutMappedRight = 0;

		/* map lfo output to desired delay times */
		// chorus/panic
		if (*mType == 0) {
			lfoOutMappedLeft = jmap(lfoOutLeft, -1.f, 1.f, 0.001f, 0.005f);
			lfoOutMappedRight = jmap(lfoOutRight, -1.f, 1.f, 0.001f, 0.005f);
		}
		// flanger/paranoid
		else if (*mType == 1) {
			lfoOutMappedLeft = jmap(lfoOutLeft, -1.f, 1.f, 0.005f, 0.03f);
			lfoOutMappedRight = jmap(lfoOutRight, -1.f, 1.f, 0.005f, 0.03f);
		}
		// my effect
		else {
			lfoOutMappedLeft = jmap(lfoOutLeft, -1.f, 1.f, 0.f, 1.f);
			lfoOutMappedRight = jmap(lfoOutRight, -1.f, 1.f, 0.f, 1.f);
		}

		// calculate delay time in samples
		float delayTimeSamplesLeft = getSampleRate() * lfoOutMappedLeft;
		float delayTimeSamplesRight = getSampleRate() * lfoOutMappedRight;

		// calculate read head postion
		float delayReadHeadLeft = mCircularBufferWriteHead - delayTimeSamplesLeft;
		if (delayReadHeadLeft < 0) {
			delayReadHeadLeft += mCircularBufferLength;
		}
		float delayReadHeadRight = mCircularBufferWriteHead - delayTimeSamplesRight;
		if (delayReadHeadRight < 0) {
			delayReadHeadRight += mCircularBufferLength;
		}

		/* calculate linear intopolation for left channel */
		int readHeadLeft_x = (int)delayReadHeadLeft;
		int readHeadLeft_x1 = readHeadLeft_x + 1;
		float readHeadFloatLeft = delayReadHeadLeft - readHeadLeft_x;
		if (readHeadLeft_x1 >= mCircularBufferLength) {
			readHeadLeft_x1 -= mCircularBufferLength;
		}
		/* calculate linear intopolation for right channel */
		int readHeadRight_x = (int)delayReadHeadRight;
		int readHeadRight_x1 = readHeadRight_x + 1;
		float readHeadFloatRight = delayReadHeadRight - readHeadRight_x;
		if (readHeadRight_x1 >= mCircularBufferLength) {
			readHeadRight_x1 -= mCircularBufferLength;
		}

		/* generate left and right delay samples*/
		float delay_sample_left = lin_interp(mCircularBufferLeft[readHeadLeft_x], mCircularBufferLeft[readHeadLeft_x1], readHeadFloatLeft);
		float delay_sample_right = lin_interp(mCircularBufferRight[readHeadRight_x], mCircularBufferRight[readHeadRight_x1], readHeadFloatRight);

		// caculate feedback delay samples
		mFeedbackLeft = delay_sample_left * *mFeedbackParameter;
		mFeedbackRight = delay_sample_right * *mFeedbackParameter;

		//move forward write head
		mCircularBufferWriteHead++;

		if (mCircularBufferWriteHead >= mCircularBufferLength) {
			mCircularBufferWriteHead = 0;
		}

		//calculate dry and wet
		float dryAmount = 1 - *mDryWetParameter;
		float wetAmount = *mDryWetParameter;

		//calculate new samples
		float newSampleLeft = (buffer.getSample(0, i) * dryAmount + delay_sample_left * wetAmount) * pow(*mMasterParameter, 2);
		float newSampleRight = (buffer.getSample(1, i) * dryAmount + delay_sample_right * wetAmount) * pow(*mMasterParameter, 2);

		//write sample to buffer
		buffer.setSample(0, i, newSampleLeft);
		buffer.setSample(1, i, newSampleRight);
	}
}

//==============================================================================
bool ParanoidChorusAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* ParanoidChorusAudioProcessor::createEditor()
{
    return new ParanoidChorusAudioProcessorEditor (*this);
}

//==============================================================================
void ParanoidChorusAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
	std::unique_ptr<XmlElement> xml(new XmlElement("FlangerChorus"));
	xml->setAttribute("DryWet", *mDryWetParameter);
	xml->setAttribute("Depth", *mDepthParameter);
	xml->setAttribute("Rate", *mRateParameter);
	xml->setAttribute("PhaseOffset", *mPhaseOffsetParameter);
	xml->setAttribute("Feedback", *mFeedbackParameter);
	xml->setAttribute("Type", *mType);

	copyXmlToBinary(*xml, destData);
}

void ParanoidChorusAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
	std::unique_ptr<XmlElement> xml(getXmlFromBinary(data, sizeInBytes));

	if (xml.get() != nullptr && xml->hasTagName("FlangerChorus")) {
		*mDryWetParameter = xml->getDoubleAttribute("DryWet");
		*mDepthParameter = xml->getDoubleAttribute("Depth");
		*mRateParameter = xml->getDoubleAttribute("Rate");
		*mPhaseOffsetParameter = xml->getDoubleAttribute("PhaseOffset");
		*mFeedbackParameter = xml->getDoubleAttribute("Feedback");

		*mType = xml->getIntAttribute("Type");
	}
}

float ParanoidChorusAudioProcessor::lin_interp(float sample_x, float sample_x1, float inPhase)
{
	return (1 - inPhase) * sample_x + inPhase * sample_x1;
}

float ParanoidChorusAudioProcessor::saw(float phase)
{
	float result;
	if (phase < 0.5) { result = 2.0 * phase; }
	else { result = 2.0 * phase - 2.0; }

	return result;
}

float ParanoidChorusAudioProcessor::tri(float phase)
{
	float result;
	if (phase < 0.25) { result = 4.0 * phase; }
	else if(phase < 0.75) { result = -4.0 * phase + 2.0; }
	else { result = 4.0 * phase - 4.0; }

	return result;
}

float ParanoidChorusAudioProcessor::sqr(float phase)
{
	float result;
	if (phase < 0.5) { result = 1.0; }
	else { result = -1.0; }

	return result;
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ParanoidChorusAudioProcessor();
}
