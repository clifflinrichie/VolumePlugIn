/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/
#define _USE_MATH_DEFINES

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath>
#include <math.h>

//==============================================================================
CliffLp01volumeAndBalanceAudioProcessor::CliffLp01volumeAndBalanceAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    // Mu45: Instanstiate each audio parameter
    addParameter(mVolumeParam = new juce::AudioParameterFloat("gain", // Parameter ID 
        "Gain", // ParameterName
        -60.0f, //minvalue
        10.0f, //maxValue
        1.0f)); //default value

    addParameter(mBalanceParam = new juce::AudioParameterFloat("balance", // Parameter ID 
        "Balance", // ParameterName
        -50.0f, //minvalue
        50.0f, //maxValue
        0.0f)); //default value
}

CliffLp01volumeAndBalanceAudioProcessor::~CliffLp01volumeAndBalanceAudioProcessor()
{
}

//==============================================================================
const juce::String CliffLp01volumeAndBalanceAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool CliffLp01volumeAndBalanceAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool CliffLp01volumeAndBalanceAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool CliffLp01volumeAndBalanceAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double CliffLp01volumeAndBalanceAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int CliffLp01volumeAndBalanceAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int CliffLp01volumeAndBalanceAudioProcessor::getCurrentProgram()
{
    return 0;
}

void CliffLp01volumeAndBalanceAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String CliffLp01volumeAndBalanceAudioProcessor::getProgramName (int index)
{
    return {};
}

void CliffLp01volumeAndBalanceAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void CliffLp01volumeAndBalanceAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mGainLinear = 1.0;
}

void CliffLp01volumeAndBalanceAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool CliffLp01volumeAndBalanceAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void CliffLp01volumeAndBalanceAudioProcessor::calcAlgorithmParams() {
    // volume function
    mGainLinear = pow(10, (mVolumeParam->get())/20);

    // balance
    mBalanceLinear = mBalanceParam->get();
    // add 50 and divide by 100 to get range 0-1, then multiply pi/2 to get radians
    mBalanceLinear = ((mBalanceLinear + 50) / 100) * (M_PI / 2);
    
}

void CliffLp01volumeAndBalanceAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // update algo params from user params before processing audio
    calcAlgorithmParams();

    // get the left and right audio buffers
    auto* channelDataLeft = buffer.getWritePointer(0);
    auto* channelDataRight = buffer.getWritePointer(1);

    // inner loop, steps through each sample in bugger
    // apply volume and balance algorithms
    for (int samp = 0; samp < buffer.getNumSamples(); samp++) {
        channelDataLeft[samp] = mGainLinear * cos(mBalanceLinear) * channelDataLeft[samp];
        channelDataRight[samp] = mGainLinear * sin(mBalanceLinear) * channelDataRight[samp];
    }
}

//==============================================================================
bool CliffLp01volumeAndBalanceAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* CliffLp01volumeAndBalanceAudioProcessor::createEditor()
{
    return new CliffLp01volumeAndBalanceAudioProcessorEditor (*this);
}

//==============================================================================
void CliffLp01volumeAndBalanceAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void CliffLp01volumeAndBalanceAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CliffLp01volumeAndBalanceAudioProcessor();
}
