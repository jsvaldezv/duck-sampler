#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class sampler_Waveform  : public juce::Component
{
public:
    sampler_Waveform(Sampler_Curso_FinalAudioProcessor&);
    ~sampler_Waveform() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    std::vector<float> audioPoints;
    juce::String fileName {""};
    
    Sampler_Curso_FinalAudioProcessor& audioProcessorMine;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (sampler_Waveform)
};
