/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class TwoMassCollisionNonlinearOscillatorAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    TwoMassCollisionNonlinearOscillatorAudioProcessor();
    ~TwoMassCollisionNonlinearOscillatorAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    juce::AudioProcessorValueTreeState mValueTreeState;

private:
    
    //juce::dsp::Compressor<float> compressor;
    juce::AudioProcessorValueTreeState::ParameterLayout CreateParameters();
    //juce::dsp::Gain<float> gain;
        
    int fs;
    float T;
        
    float um1;
    float um2;
    float u1;
    float u2;
    float up1;
    float up2;
    float psim1;
    float psim2;
    float psi1;
    float psi2;
    float psip1;
    float psip2;
    float g;
    float EPSIL;
    float r;
    float rc;
    float rvalue;
        
    float k;
    float monevalue;
    float mtwovalue;
    float ub;
    float ubm;
    float ubp;
    float a1;
    float a2;
    float a3;
    float a4;
    //float af;
    float ar1;
    float ar2;
        
    float b0;
    float b;
    float s01;
    float s02;
    float s1;
    float s2;
    
    float ac1;
    float ac2;
    float outl;
    float outr;
    float outl2;
    float outr2;
    
    float CT;
    float CR;
    float CS;
    float rmsl;
    float XL;
    float GL;
    float fl;
    float gl;
    float coeffl;
    float outL;
    float rmsr;
    float XR;
    float GR;
    float fr;
    float gr;
    float coeffr;
    float outR;
    float at;
    float rt;
    float tav;
        
    juce::AudioParameterInt* feParameter;
    juce::AudioParameterFloat* facParameter;
    juce::AudioParameterInt* kOneParameter;
    juce::AudioParameterFloat* mOneParameter;
    juce::AudioParameterInt* kTwoParameter;
    juce::AudioParameterFloat* mTwoParameter;
    juce::AudioParameterInt* kcParameter;
    juce::AudioParameterFloat* alpParameter;
    juce::AudioParameterInt* ubParameter;
    juce::AudioParameterInt* rcParameter;
    juce::AudioParameterBool* OnOffParameter;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TwoMassCollisionNonlinearOscillatorAudioProcessor)
};
