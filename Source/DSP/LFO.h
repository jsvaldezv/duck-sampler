#pragma once
#include <JuceHeader.h>

class LFO
{
public:
    
    LFO() = default;
    
    struct Parameters
    {
        float rate = 1.0f;
    };
    
    void prepare (double samplerate);
    
    void setParameters (float rate);
    
    void process (juce::AudioBuffer<float>& buffer);
    
private:
    
    Parameters parameters;
    
    float wtSize { 512 };
    double mySampleRate { 44100 };
    
    double increment { 0.0 };
    double phase[2] { 0.0 };
    
    juce::Array<float> waveTableSin;
};
