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
    if(inDistor >= 0.3)
    {
        for(int i = 0; i < inNumSamples; i++)
        {
            inAudio[i] *= inDistor;
            outAudio[i] = sinh(inAudio[i])/sqrt(1+(powf(sinh(inAudio[i]),2)));
        }
    }
}
