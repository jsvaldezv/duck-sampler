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
class Sampler_Curso_FinalAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    Sampler_Curso_FinalAudioProcessorEditor (Sampler_Curso_FinalAudioProcessor&);
    ~Sampler_Curso_FinalAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Sampler_Curso_FinalAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Sampler_Curso_FinalAudioProcessorEditor)
};
