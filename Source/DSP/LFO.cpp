#include "LFO.h"

void LFO::prepare (double samplerate)
{
    mySampleRate = samplerate;
    wtSize = 512;
    
    waveTableSin.clear();
    
    // Wavetable creation
    for (int i = 0; i < wtSize; i++)
    {
        float value = (float) (i / wtSize);
        waveTableSin.insert (i, std::sinf ((juce::MathConstants<float>::twoPi * value)));
    }
}

void LFO::setParameters (float rate)
{
    parameters.rate = rate;
}

void LFO::process (juce::AudioBuffer<float>& buffer)
{
    increment = parameters.rate * wtSize / mySampleRate;
    
    for (int ch = 0; ch < buffer.getNumChannels(); ch++)
    {
        for (int i = 0; i < buffer.getNumSamples(); i++)
        {
            float sample = buffer.getSample (ch, i);
            float out = sample * (waveTableSin[(int)phase[ch]] * 1.0f);
            
            phase[ch] = std::fmod ((phase[ch] + increment), wtSize);
            
            buffer.setSample (ch, i, out);
        }
    }
}
