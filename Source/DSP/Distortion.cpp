#include "Distortion.h"

void Distortion::setParameters (bool bypass)
{
    parameters.bypass = bypass;
}

void Distortion::process (juce::AudioBuffer<float>& buffer)
{
    if (parameters.bypass)
        return;
    
    for (int ch = 0; ch < buffer.getNumChannels(); ch++)
    {
        for (int i = 0; i < buffer.getNumSamples(); i++)
        {
            float sample = buffer.getSample (ch, i);
            float out = 0.0f;
            
            if (sample > 0)
                out = 0.05f;
            
            else if (sample == 0)
                out = sample;
            
            else
                out = -0.05f;
            
            buffer.setSample (ch, i, out);
        }
    }
}
