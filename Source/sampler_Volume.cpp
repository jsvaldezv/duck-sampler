#include "sampler_Volume.h"

//CONSTRUCTOR
sampler_Volume::sampler_Volume()
{
    
}

//DESTRUCTOR
sampler_Volume::~sampler_Volume()
{
    
}

//METODO PARA MODIFICAR EL VOLUMEN
void sampler_Volume::processVolume(float* inAudio,
                                   float* outAudio,
                                   float inGain,
                                   int inNumSamples)
{
    for(int i = 0; i < inNumSamples; i++)
    {
        outAudio[i] = inAudio[i] * inGain;
    }
}
