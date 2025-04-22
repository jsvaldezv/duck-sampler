#pragma once
#include <JuceHeader.h>

class Volume
{
public:

    Volume() = default;
    
    struct Parameters
    {
        float gain = 1.0f;
    };
    
    void setParameters (float gain)
    {
        parameters.gain = gain;
    }
    
    void process (juce::AudioBuffer<float>& buffer)
    {
        for (int ch = 0; ch < buffer.getNumChannels(); ch++)
        {
            for (int i = 0; i < buffer.getNumSamples(); i++)
            {
                float sample = buffer.getSample (ch, i);
                float out = sample * parameters.gain;
                
                buffer.setSample (ch, i, out);
            }
        }
    }
    
private:
    
    Parameters parameters;
    
};
