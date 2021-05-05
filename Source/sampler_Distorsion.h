#pragma once
#include "math.h"

class sampler_Distorsion
{
public:
    
    //CONSTRUCTOR
    sampler_Distorsion();
    
    //DESTRUCTOR
    ~sampler_Distorsion();
    
    //VOLUMEN PROCESS
    void processDistor(float* inAudio, float* outAudio, float inDistor, int inNumSamples);
    
private:
    
    float smoothDistortion{0.0f};
    
};

