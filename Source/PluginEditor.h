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
class OtherLookAndFeel : public juce::LookAndFeel_V4 {
public:
    OtherLookAndFeel() {
        setColour(juce::Slider::thumbColourId, juce::Colours::papayawhip);
        setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::lightpink);
        setColour(juce::Slider::textBoxTextColourId, juce::Colours::dimgrey);
        setColour(juce::Slider::trackColourId, juce::Colours::forestgreen);

    }
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) override
    {
        auto radius = (float)juce::jmin(width / 2, height / 2) - 4.0f;
        auto centreX = (float) x + (float)width * 0.5f;
        auto centreY = (float)y + (float)height * 0.5f;
        auto rx = centreX - radius;
        auto ry = centreY - radius;
        auto rw = radius * 2.0f;
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

        // fill
        g.setColour(juce::Colours::skyblue);
        g.fillEllipse(rx, ry, rw, rw);

        // outline
        g.setColour(juce::Colours::crimson);
        g.drawEllipse(rx, ry, rw, rw, 1.0f);

        juce::Path p;
        auto pointerLength = radius * 0.33f;
        auto pointerThickness = 2.0f;
        p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
        p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));

        // pointer 
        g.setColour(juce::Colours::white);
        g.fillPath(p);
    }
};

class CliffLp01volumeAndBalanceAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Slider::Listener,
    public juce::Timer

{
public:
    CliffLp01volumeAndBalanceAudioProcessorEditor (CliffLp01volumeAndBalanceAudioProcessor&);
    ~CliffLp01volumeAndBalanceAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    // gets called when a slider changes
    void sliderValueChanged(juce::Slider* slider) override;

    // slider automation
    void timerCallback() override;


private:
    // add a slider to the editor
    void sliderValueChanged(juce::Slider* slider) override;

private:
    add a slider to the editor
    juce::Slider mVolumeSlider;
    juce::Slider mBalanceSlider;
    OtherLookAndFeel otherLookAndFeel;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    CliffLp01volumeAndBalanceAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CliffLp01volumeAndBalanceAudioProcessorEditor)
};
