/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CliffLp01volumeAndBalanceAudioProcessorEditor::CliffLp01volumeAndBalanceAudioProcessorEditor (CliffLp01volumeAndBalanceAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 400);

    // create a pointer to audio parameters in audio processor
    auto& processorParams = processor.getParameters();



    // set attributes for volume slider
    mVolumeSlider.setBounds(150, 250, 300, 100); // x, y, width, height 
    mVolumeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    mVolumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, mVolumeSlider.getTextBoxHeight()); // read only, w h
    mVolumeSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::snow);
    mVolumeSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::goldenrod);
   

    // set attributes for the balance slider
    mBalanceSlider.setBounds(200, 50, 200, 200);
    mBalanceSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    mBalanceSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 80, mBalanceSlider.getTextBoxHeight());
    mBalanceSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::snow);
    mBalanceSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::goldenrod);
    mBalanceSlider.setMouseDragSensitivity(400);
    setLookAndFeel(&otherLookAndFeel);

    // set range of slider to range of audio parameter
    juce::AudioParameterFloat* procParam = (juce::AudioParameterFloat*)processorParams.getUnchecked(0);
    mVolumeSlider.setRange(procParam->range.start, procParam->range.end);
    mVolumeSlider.setValue(*procParam);
    mVolumeSlider.setSkewFactorFromMidPoint(1.0);

    juce::AudioParameterFloat* secParam = (juce::AudioParameterFloat*)processorParams.getUnchecked(1);
    mBalanceSlider.setRange(secParam->range.start, secParam->range.end);
    mBalanceSlider.setValue(*secParam);

    // set processorEditor to be a listener for the slider
    mVolumeSlider.addListener(this);
    mBalanceSlider.addListener(this);

    // add the slider. now a child of the audio processor editor object
    addAndMakeVisible(mVolumeSlider);
    addAndMakeVisible(mBalanceSlider);

    // timer to update sliders
    startTimer(100);
}

CliffLp01volumeAndBalanceAudioProcessorEditor::~CliffLp01volumeAndBalanceAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void CliffLp01volumeAndBalanceAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Volume", 100, 100, 400, 550, juce::Justification::centred, 1);
    g.drawFittedText("Balance", 100, 30, 400, 1, juce::Justification::centred, 1);
}

void CliffLp01volumeAndBalanceAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void  CliffLp01volumeAndBalanceAudioProcessorEditor::sliderValueChanged(juce::Slider* slider) {
    // A) will point to audio parameters in audio processor
    auto& processorParams = processor.getParameters();

    // B) Compare the pointer to the slider that was changed with the address of our slider
    if (slider == &mVolumeSlider) {
        // C) Get a pointer to the first parameter in processor
        juce::AudioParameterFloat* volumeParam = (juce::AudioParameterFloat*)processorParams.getUnchecked(0);

        // D) use value from slider to set parameter in processor
        float sliderValue = mVolumeSlider.getValue();
        *volumeParam = sliderValue;
        float gainValue = pow(10, sliderValue / 20);

        // E) can use DBG() for simple print debugging
        DBG("Volume Slider Changed to " << sliderValue << ", the gain is " << gainValue);

    }

    if (slider == &mBalanceSlider) {
        juce::AudioParameterFloat* balanceParam = (juce::AudioParameterFloat*)processorParams.getUnchecked(1);

        float balanceValue = mBalanceSlider.getValue();
        *balanceParam = balanceValue;

        DBG("Balance slider changed to " << balanceValue);
    }
}

void CliffLp01volumeAndBalanceAudioProcessorEditor::timerCallback()
{
    auto& params = processor.getParameters();

    //Update the value of each slider to match the value in the Processor
    juce::AudioParameterFloat* gainParam = (juce::AudioParameterFloat*)params.getUnchecked(0);
    mVolumeSlider.setValue(gainParam->get(), juce::dontSendNotification);

    juce::AudioParameterFloat* balanceParam = (juce::AudioParameterFloat*)params.getUnchecked(1);
    mBalanceSlider.setValue(balanceParam->get(), juce::dontSendNotification);
}