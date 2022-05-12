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
class TwoMassCollisionNonlinearOscillatorAudioProcessorEditor  : public juce::AudioProcessorEditor, juce::Button::Listener, juce::Slider::Listener
{
public:
    TwoMassCollisionNonlinearOscillatorAudioProcessorEditor (TwoMassCollisionNonlinearOscillatorAudioProcessor&);
    ~TwoMassCollisionNonlinearOscillatorAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;
    void sliderValueChanged (juce::Slider* slider) override;

private:
    
    juce::Slider facSlider;
    juce::Label facLabel;
    juce::Slider koneSlider;
    juce::Label koneLabel;
    juce::Slider moneSlider;
    juce::Label moneLabel;
    juce::Slider ktwoSlider;
    juce::Label ktwoLabel;
    juce::Slider mtwoSlider;
    juce::Label mtwoLabel;
    juce::Slider kcSlider;
    juce::Label kcLabel;
    juce::Slider alpSlider;
    juce::Label alpLabel;
    juce::Slider ubSlider;
    juce::Label ubLabel;
    juce::Slider rcSlider;
    juce::Label rcLabel;
    juce::Slider OnOffSlider;
    juce::TextButton OnButton;
    juce::Label OnButtonLabel;
    juce::TextButton OffButton;
    juce::Label OffButtonLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> facSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> koneSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> moneSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ktwoSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mtwoSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> kcSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> alpSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ubSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rcSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> OnOffSliderAttachment;
    
    TwoMassCollisionNonlinearOscillatorAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TwoMassCollisionNonlinearOscillatorAudioProcessorEditor)
};
