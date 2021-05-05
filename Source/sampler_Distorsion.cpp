#include "sampler_Distorsion.h"

//CONSTRUCTOR
sampler_Distorsion::sampler_Distorsion()
{
    
}

//DESTRUCTOR
sampler_Distorsion::~sampler_Distorsion()
{
    
}

//VOLUMEN PROCESS
void sampler_Distorsion::processDistor(float* inAudio, float* outAudio, float inDistor, int inNumSamples)
{
    for(int i = 0; i < inNumSamples; i++)
    {
        inAudio[i] *= inDistor;
        outAudio[i] = 1.0f / (1.0f + powf(M_E,-inAudio[i]));
    }
}
