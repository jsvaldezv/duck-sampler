#include "sampler_LFO.h"

sampler_LFO::sampler_LFO()
{
    
}

sampler_LFO::~sampler_LFO()
{
    
}

void sampler_LFO::prepareLFO(double inSampleRate)
{
    mySampleRate = inSampleRate;
    wtSize = 512;
    
    waveTableSin.clear();
    
    //CREACION DE WAVETABLES
    for (int i = 0; i < wtSize; i++)
    {
        float value = (float)i/wtSize;
        waveTableSin.insert(i, sinf((2.0f*valorPI*value)));
    }
}

void sampler_LFO::processLFO(float* inAudio,
                             float* outAudio,
                             float inRate,
                             int inNumSamples)
{
    increment = inRate * wtSize / mySampleRate;
    
    for(int i = 0; i < inNumSamples; i++)
    {
        outAudio[i] = inAudio[i] * (waveTableSin[(int)phase] * 1.0f);
        
        phase = fmod((phase + increment), wtSize);
    }
}
