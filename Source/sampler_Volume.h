#pragma once

class sampler_Volume
{
public:
    
    //CONSTRUCTOR
    sampler_Volume();
    
    //DESTRUCTOR
    ~sampler_Volume();
    
    //VOLUMEN PROCESS
    void processVolume(float* inAudio, float* outAudio, float inGain, int inNumSamples);
    
private:
    
};
