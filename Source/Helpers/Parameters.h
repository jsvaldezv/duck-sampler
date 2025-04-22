#pragma once
#include <JuceHeader.h>

static const int NUM_VOICES = 5;

static juce::String VOLUME = "Volume";
static juce::String RATE = "Rate";
static juce::String ATTACK = "Attack";
static juce::String DECAY = "Decay";
static juce::String SUSTAIN = "Sustain";
static juce::String RELEASE = "Release";
static juce::String DISTORTION = "Distortion";

inline juce::AudioProcessorValueTreeState::ParameterLayout createParameters()
{
    juce::AudioProcessorValueTreeState::ParameterLayout params;
    
    params.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID (VOLUME, 1), VOLUME, 0.0f, 2.0f, 1.0f));
    params.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID (RATE, 1), RATE, 0.1f, 20.0f, 1.0f));
    
    params.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID (ATTACK, 1), ATTACK, 0.1f, 1.0f, 0.1f));
    params.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID (DECAY, 1), DECAY, 0.1f, 1.0f, 0.1f));
    params.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID (SUSTAIN, 1), SUSTAIN, 0.1f, 1.0f, 0.1f));
    params.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID (RELEASE, 1), RELEASE, 0.1f, 3.0f, 0.1f));
    
    params.add (std::make_unique<juce::AudioParameterBool> (juce::ParameterID (DISTORTION, 1), DISTORTION, false));
    
    return params;
}
