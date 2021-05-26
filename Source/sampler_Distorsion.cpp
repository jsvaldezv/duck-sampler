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
void sampler_Distorsion::processDistor(float* inAudio, float* outAudio, bool inDistor, int inNumSamples)
{
    if(inDistor)
    {
        for(int i = 0; i < inNumSamples; i++)
        {
            if(inAudio[i] > 0)
                outAudio[i] = 0.05f;
            else if(inAudio[i] == 0)
                outAudio[i] = inAudio[i];
            else
                outAudio[i] = -0.05f;
        }
    }
}
