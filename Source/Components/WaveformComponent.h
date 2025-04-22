#pragma once
#include <JuceHeader.h>
#include "../PluginProcessor.h"

class WaveformComponent : public juce::Component
{
public:
    
    WaveformComponent (DuckSamplerAudioProcessor&);
    ~WaveformComponent() override;

    void paint (juce::Graphics&) override;
    
    void resized() override;

private:
    
    DuckSamplerAudioProcessor& audioProcessorMine;
    
    std::vector<float> audioPoints;
    
    juce::String fileName { "" };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformComponent)
};
