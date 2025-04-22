#include "WaveformComponent.h"

WaveformComponent::WaveformComponent (DuckSamplerAudioProcessor& p) : audioProcessorMine (p) {}

WaveformComponent::~WaveformComponent() {}

void WaveformComponent::paint (juce::Graphics& g)
{
    auto waveform = audioProcessorMine.getWaveform();
    
    if (waveform.getNumSamples() > 0)
    {
        juce::Path p;
        audioPoints.clear();
        
        auto ratio = waveform.getNumSamples() / 700;
        auto buffer = waveform.getReadPointer(0);
        
        for (int sample = 0; sample < waveform.getNumSamples(); sample+=ratio)
            audioPoints.push_back (buffer[sample]);
        
        g.setColour (juce::Colours::yellow);
        p.startNewSubPath (0, 200/2);
        
        for (int sample = 0; sample < audioPoints.size(); ++sample)
        {
            auto point = juce::jmap (audioPoints[sample], -1.0f, 1.0f, 0.0f, 200.0f);
            p.lineTo (sample, point);
        }
        
        g.strokePath (p, juce::PathStrokeType(1));
        g.setColour (juce::Colours::white);
        g.setFont (15.0f);
        
        auto textBounds = getLocalBounds().reduced(10, 10);
        g.drawFittedText (fileName, textBounds, juce::Justification::topRight, 1);
        
        auto playHeadPositon = juce::jmap<int> (audioProcessorMine.getSampleCount(),
                                                0,
                                                audioProcessorMine.getWaveform().getNumSamples(),
                                                0,
                                                getWidth());
        
        g.setColour (juce::Colours::white);
        g.drawLine (playHeadPositon, 0, playHeadPositon, getHeight(), 0.5f);
        g.setColour (juce::Colours::black.withAlpha(0.2f));
        g.fillRect (0, 0, playHeadPositon, getHeight());
    }
    else
    {
        g.setColour (juce::Colours::white);
        g.setFont (40.0f);
        g.drawFittedText ("Drop an audio", 240, 50, 230, 40, juce::Justification::centred, 1);
    }
}

void WaveformComponent::resized() {}
