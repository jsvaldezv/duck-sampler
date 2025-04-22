#pragma once
#include <JuceHeader.h>

class Distortion
{
public:
    
    Distortion() = default;
    
    struct Parameters
    {
        bool bypass = false;
    };
    
    void setParameters (bool bypass);
    
    void process (juce::AudioBuffer<float>& buffer);
    
private:
    
    Parameters parameters;
    
    float smoothDistortion { 0.0f };
    
};

