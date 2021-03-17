#pragma once
#include "math.h"
#include <JuceHeader.h>

class sampler_LFO
{
public:
    
    sampler_LFO();
    ~sampler_LFO();
    
    void prepareLFO(double inSampleRate);
    
    void processLFO(float* inAudio,
                    float* outAudio,
                    float inRate,
                    int inNumSamples);
    
private:
    
    float valorPI {3.14159265359};
    double mySampleRate, increment, phase;
    int wtSize;
    
    juce::Array<float> waveTableSin;
};
