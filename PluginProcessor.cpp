/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
# include <iostream>
# include <math.h>

//==============================================================================
TwoMassCollisionNonlinearOscillatorAudioProcessor::TwoMassCollisionNonlinearOscillatorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),mValueTreeState(*this, nullptr, "Parameters", CreateParameters())
#endif
{
    um1 = 0.00f;
    um2 = 0.00f;
    u1 = 0.00f;
    u2 = 0.00f;
    up1 = 0.00f;
    up2 = 0.00f;
    ubm = 0.00f;
    ub = 0.00f;
    ubp = 0.00f;
    psim1 = 0.00f;
    psim2 = 0.00f;
    psi1 = 0.00f;
    psi2 = 0.00f;
    psip1 = 0.00f;
    psip2 = 0.00f;
    g = 0.00f;
}

TwoMassCollisionNonlinearOscillatorAudioProcessor::~TwoMassCollisionNonlinearOscillatorAudioProcessor()
{
}

//==============================================================================
const juce::String TwoMassCollisionNonlinearOscillatorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TwoMassCollisionNonlinearOscillatorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TwoMassCollisionNonlinearOscillatorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TwoMassCollisionNonlinearOscillatorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TwoMassCollisionNonlinearOscillatorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TwoMassCollisionNonlinearOscillatorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TwoMassCollisionNonlinearOscillatorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TwoMassCollisionNonlinearOscillatorAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String TwoMassCollisionNonlinearOscillatorAudioProcessor::getProgramName (int index)
{
    return {};
}

void TwoMassCollisionNonlinearOscillatorAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void TwoMassCollisionNonlinearOscillatorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getMainBusNumOutputChannels();
        
    fs = 44100;
    EPSIL = 0.0000000001;
    r = 0.1;
    
    CT = 80; //cut off
    CR = 3; // ratio
    CS = 1-(1/CR);
    tav = 0.01;
    at = 0.03;
    rt = 0.03;
    rmsl = 1;
    gl = 1;
    rmsr = 1;
    gr = 1;
}

void TwoMassCollisionNonlinearOscillatorAudioProcessor::releaseResources()
{
    
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TwoMassCollisionNonlinearOscillatorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void TwoMassCollisionNonlinearOscillatorAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    const int numSamples = buffer.getNumSamples();
    
    auto OnOffParameterValue = mValueTreeState.getRawParameterValue("onoff");
        
    if (*OnOffParameterValue == false){//off, stop
        juce::ScopedNoDenormals noDenormals;
        auto totalNumInputChannels  = getTotalNumInputChannels();
        auto totalNumOutputChannels = getTotalNumOutputChannels();

        for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
            buffer.clear (i, 0, buffer.getNumSamples());
        
        um1 = 0;
        um2 = 0;
        u1 = 0;
        u2 = 0;
        up1 = 0;
        up2 = 0;
        psim1 = 0;
        psim2 = 0;
        psi1 = 0;
        psi2 = 0;
        psip1 = 0;
        psip2 = 0;
        g = 0;
    }
        
    if (*OnOffParameterValue == true){//on, start
        
        juce::ScopedNoDenormals noDenormals;
        
        auto totalNumInputChannels  = getTotalNumInputChannels();
        auto totalNumOutputChannels = getTotalNumOutputChannels();

        for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
            buffer.clear (i, 0, buffer.getNumSamples());
        
        float* leftChannel = buffer.getWritePointer(0);
        float* rightChannel = buffer.getWritePointer(1);
        
        for (int i = 0; i < buffer.getNumSamples(); i++){
                
            auto facvalue = mValueTreeState.getRawParameterValue("fac");
            auto monevalue1000 = mValueTreeState.getRawParameterValue("mone");
            auto konevalue = mValueTreeState.getRawParameterValue("kone");
            auto mtwovalue1000 = mValueTreeState.getRawParameterValue("mtwo");
            auto ktwovalue = mValueTreeState.getRawParameterValue("ktwo");
            auto kcvalue = mValueTreeState.getRawParameterValue("kc");
            auto alpvalue = mValueTreeState.getRawParameterValue("alp");
            auto ubvalueparam = mValueTreeState.getRawParameterValue("ub");
                
            monevalue = *monevalue1000 * 0.001;
            mtwovalue = *mtwovalue1000 * 0.001;

            k = (*konevalue * *ktwovalue) / (*konevalue + *ktwovalue);
            ub = *ubvalueparam / sqrt(k);
            
            auto rcvalue = mValueTreeState.getRawParameterValue("rc");
            rc = *rcvalue;
            rvalue = r + rc;
            
            a1 = (*konevalue * *facvalue * 0.25) / (monevalue * fs * fs);
            a2 = 0.5 / (monevalue * fs * fs);
            a3 = (*konevalue * *facvalue * 0.25) / (mtwovalue * fs * fs);
            a4 = (*ktwovalue * *facvalue * 0.25) / (mtwovalue * fs * fs);
            ar1 = (0.5 * rvalue) / (monevalue * fs);
            ar2 = (0.5 * rvalue) / (mtwovalue * fs);
                
            b0 = (1+a1+ar1) * (1+a3+a4+ar2) - (a1 * a3);
            s01 = ((2*(1+a1)*u1+2*a1*u2)*(1+a3+a4+ar2) + (2*a3*u1+2*(1-a3-a4)*u2)*a1 + (-2*(1+a1)*um1+2*a1*um2)*(1+a3+a4+ar2) + (2*a3*um1-2*(1+a3+a4)*um2)*a1) / b0;
            s02 = ((2*(1-a1)*u1+2*a1*u2)*a3 + (2*a3*u1+2*(1-a3-a4)*u2)*(1+a1+ar1) + (-2*(1+a1)*um1+2*a1*um2)*a3 + (2*a3*um1-2*(1+a3+a4)*um2)*(1+a1+ar1)) / b0;
                
            if (u1 - ub >= 0 && *kcvalue > 0){
                g = sqrt(0.5* *kcvalue * (*alpvalue+1) * pow(u1-ub, *alpvalue-1));
            }
            else{
                g = (2*(-psim1) * s01) / (EPSIL + (s01 * s01));
            }
                
            b = (1+a1+0.5*g*g*a2+ar1) * (1+a3+a4+ar2) - (a1*a3);
            s1 = ((2*(1-a1)*u1+2*a1*u2)*(1+a3+a4+ar2) + (2*a3*u1+2*(1-a3-a4)*u2)*a1 + (-2*(1+a1)*um1+2*a1*um2)*(1+a3+a4+ar2) + (2*a3*um1-2*(1+a3+a4)*um2)*a1 - a2*(2*psim1-0.5*g*(ubp-ubm))*g*(1+a3+a4+ar2)) / (b);
            s2 = ((2*(1-a1)*u1+2*a1*u2)*a3 + (2*a3*u1+2*(1-a3-a4)*u2)*(1+a1+0.5*g*g*a2+ar1) + (-2*(1-a1)*um1+2*a1*um2)*a3 + (2*a3*um1-2*(1+a3+a4)*um2)*(1+a1+0.5*g*g*a2+ar1) - a2*(2*psim1-0.5*g*(ubp-ubm))*g*a3) / (b);
                    
            up1 = s1 + um1;
            up2 = s2 + um2;
            psip1 = psim1 + 0.5*g*(s1-ubp+ubm);
                
            ac1 = 0.5*((0.5*(up1-u1)*fs)-(0.5*(u1-um1)*fs))*fs;
            ac2 = 0.5*((0.5*(up2-u2)*fs)-(0.5*(u2-um2)*fs))*fs;
                    
            outl = 0.5*(up1-um1)*fs;
            outr = 0.5*(up2-um2)*fs;
                
            rmsl = (1-tav) * rmsl + tav * std::pow(outl,2.0f);
            XL = 10*log(rmsl);
            GL = std::min(0.0f, CS*(CT-XL));
            fl = pow(10, GL/20);
            if(fl<gl){
                coeffl = at;
            }
            else{
                coeffl = rt;
            }
            gl = (1-coeffl) * gl + coeffl * fl;
            outL = gl * outl;
                
            rmsr = (1-tav) * rmsr + tav * std::pow(outr,2.0f);
            XR = 10*log(rmsr);
            GR = std::min(0.0f, CS*(CT-XR));
            fr = pow(10, GR/20);
            if(fr<gr){
                coeffr = at;
            }
            else{
                coeffr = rt;
            }
            gr = (1-coeffr) * gr + coeffr * fr;
            outR = gr * outr;
            
            std::cout<<outL;
            std::cout<<"  ";
                    
            buffer.setSample(0, i, outL);
            buffer.setSample(1, i, outR);
                    
            um1 = u1;
            um2 = u2;
            u1 = up1;
            u2 = up2;
            psim1 = psi1;
            psim2 = psi2;
            psi1 = psip1;
            psi2 = psip2;
            ubm = ub;
            ub = ubp;
                
        }
    }
}

//==============================================================================
bool TwoMassCollisionNonlinearOscillatorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* TwoMassCollisionNonlinearOscillatorAudioProcessor::createEditor()
{
    return new TwoMassCollisionNonlinearOscillatorAudioProcessorEditor (*this);
}

//==============================================================================
void TwoMassCollisionNonlinearOscillatorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void TwoMassCollisionNonlinearOscillatorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TwoMassCollisionNonlinearOscillatorAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout TwoMassCollisionNonlinearOscillatorAudioProcessor::CreateParameters(){
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>("fac",//Parameter ID
                                                                 "Fac",//Parameter Name
                                                                 0.01f,//Min Value
                                                                 10.00f,//Max Value
                                                                 1.00f//Default Value
                                                                 ));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("mone",
                                                                 "MOne",
                                                                 1.0f,
                                                                 100.0f,
                                                                 13.0f
                                                                 ));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("kone",
                                                                 "KOne",
                                                                 50.0f,
                                                                 1000000.0f,
                                                                 10000.0f
                                                                 ));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("mtwo",
                                                                 "MTwo",
                                                                 1.0f,
                                                                 100.0f,
                                                                 11.0f
                                                                 ));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("ktwo",
                                                                 "KTwo",
                                                                 50.0f,
                                                                 1000000.0f,
                                                                 9100.0f
                                                                 ));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("kc",
                                                                 "Kc",
                                                                 1000.0f,
                                                                 100000000.0f,
                                                                 1000000.0f
                                                                 ));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("alp",
                                                                 "Alp",
                                                                 1.0f,
                                                                 1.8f,
                                                                 1.3f
                                                                 ));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("ub",
                                                                 "Ub",
                                                                 -1.0f,
                                                                 10.0f,
                                                                 0.0f
                                                                 ));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("rc",
                                                                 "Rc",
                                                                 0.0f,
                                                                 1000000.0f,
                                                                 0.0f
                                                                 ));
    params.push_back(std::make_unique<juce::AudioParameterBool>("onoff", "OnOff", true));
    
    return {params.begin(), params.end()};
}
