/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TwoMassCollisionNonlinearOscillatorAudioProcessorEditor::TwoMassCollisionNonlinearOscillatorAudioProcessorEditor (TwoMassCollisionNonlinearOscillatorAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    OnButton.addListener(this);
    OffButton.addListener(this);
    OnOffSlider.addListener(this);
        
    facSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    facSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 50);
    addAndMakeVisible(facSlider);
    facSlider.setBounds(350, 0, 150, 150);
    facSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.mValueTreeState, "fac", facSlider);
    facLabel.setText("fac", juce::dontSendNotification);
    facLabel.attachToComponent(&facSlider, true);
    addAndMakeVisible(facLabel);
        
    koneSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    koneSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 50);
    addAndMakeVisible(koneSlider);
    koneSlider.setBounds(100, 300, 150, 150);
    koneSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.mValueTreeState, "kone", koneSlider);
    koneLabel.setText("kone", juce::dontSendNotification);
    koneLabel.attachToComponent(&koneSlider, true);
    addAndMakeVisible(koneLabel);
        
    moneSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    moneSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 50);
    addAndMakeVisible(moneSlider);
    moneSlider.setBounds(450, 300, 150, 150);
    moneSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.mValueTreeState, "mone", moneSlider);
    moneLabel.setText("mone (*0.001)", juce::dontSendNotification);
    moneLabel.attachToComponent(&moneSlider, true);
    addAndMakeVisible(moneLabel);
        
    ktwoSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    ktwoSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 50);
    addAndMakeVisible(ktwoSlider);
    ktwoSlider.setBounds(100, 500, 150, 150);
    ktwoSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.mValueTreeState, "ktwo", ktwoSlider);
    ktwoLabel.setText("ktwo", juce::dontSendNotification);
    ktwoLabel.attachToComponent(&ktwoSlider, true);
    addAndMakeVisible(ktwoLabel);
        
    mtwoSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    mtwoSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 50);
    addAndMakeVisible(mtwoSlider);
    mtwoSlider.setBounds(450, 500, 150, 150);
    mtwoSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.mValueTreeState, "mtwo", mtwoSlider);
    mtwoLabel.setText("mtwo (*0.001)", juce::dontSendNotification);
    mtwoLabel.attachToComponent(&mtwoSlider, true);
    addAndMakeVisible(mtwoLabel);
        
    kcSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    kcSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 50);
    addAndMakeVisible(kcSlider);
    kcSlider.setBounds(850, 500, 150, 150);
    kcSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.mValueTreeState, "kc", kcSlider);
    kcLabel.setText("kc", juce::dontSendNotification);
    kcLabel.attachToComponent(&kcSlider, true);
    addAndMakeVisible(kcLabel);
        
    alpSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    alpSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 50);
    addAndMakeVisible(alpSlider);
    alpSlider.setBounds(850, 0, 150, 150);
    alpSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.mValueTreeState, "alp", alpSlider);
    alpLabel.setText("alp", juce::dontSendNotification);
    alpLabel.attachToComponent(&alpSlider, true);
    addAndMakeVisible(alpLabel);
        
    ubSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    ubSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 50);
    addAndMakeVisible(ubSlider);
    ubSlider.setBounds(600, 0, 150, 150);
    ubSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.mValueTreeState, "ub", ubSlider);
    ubLabel.setText("ub", juce::dontSendNotification);
    ubLabel.attachToComponent(&ubSlider, true);
    addAndMakeVisible(ubLabel);
    
    rcSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    rcSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 50);
    addAndMakeVisible(rcSlider);
    rcSlider.setBounds(200, 0, 150, 150);
    rcSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.mValueTreeState, "rc", rcSlider);
    rcLabel.setText("rc", juce::dontSendNotification);
    rcLabel.attachToComponent(&rcSlider, true);
    addAndMakeVisible(rcLabel);
    
    OnOffSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    OnOffSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 50);
    OnOffSlider.setBounds(100, 0, 150, 150);
    OnOffSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.mValueTreeState, "onoff", OnOffSlider);
    addAndMakeVisible(OnButton);
    OnButton.setBounds(20,20,70,70);
    OnButton.setButtonText("Start");
    addAndMakeVisible(OffButton);
    OffButton.setBounds(100,20,70,70);
    OffButton.setButtonText("Stop");
    
    setSize (1000, 1000);
}

TwoMassCollisionNonlinearOscillatorAudioProcessorEditor::~TwoMassCollisionNonlinearOscillatorAudioProcessorEditor()
{
}

//==============================================================================
void TwoMassCollisionNonlinearOscillatorAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void TwoMassCollisionNonlinearOscillatorAudioProcessorEditor::resized()
{
    
}

void TwoMassCollisionNonlinearOscillatorAudioProcessorEditor::buttonClicked(juce::Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == &OnButton)
    {
        OnOffSlider.setValue(true);
    }
    if (buttonThatWasClicked == &OffButton)
    {
        OnOffSlider.setValue(false);
    }
}

void TwoMassCollisionNonlinearOscillatorAudioProcessorEditor::sliderValueChanged (juce::Slider* slider)
{
    if (slider == &OnOffSlider)
    {
        auto state = slider->getValue();
        if(state == true)
        {
            OnButton.setColour(0x1000100, juce::Colours::red);
            OffButton.setColour(0x1000100, juce::Colours::grey);
        }
        if(state == false)
        {
            OnButton.setColour(0x1000100, juce::Colours::grey);
            OffButton.setColour(0x1000100, juce::Colours::red);
        }
    }
}
