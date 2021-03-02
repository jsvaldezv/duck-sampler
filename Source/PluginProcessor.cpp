#include "PluginProcessor.h"
#include "PluginEditor.h"

Sampler_Curso_FinalAudioProcessor::Sampler_Curso_FinalAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    for(int i = 0; i < getTotalNumInputChannels(); i++)
    {
        ptrVolume[i] = std::unique_ptr<sampler_Volume>(new sampler_Volume());
    }
}

Sampler_Curso_FinalAudioProcessor::~Sampler_Curso_FinalAudioProcessor()
{
}

const juce::String Sampler_Curso_FinalAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Sampler_Curso_FinalAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Sampler_Curso_FinalAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Sampler_Curso_FinalAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Sampler_Curso_FinalAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Sampler_Curso_FinalAudioProcessor::getNumPrograms()
{
    return 1;
}

int Sampler_Curso_FinalAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Sampler_Curso_FinalAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Sampler_Curso_FinalAudioProcessor::getProgramName (int index)
{
    return {};
}

void Sampler_Curso_FinalAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

void Sampler_Curso_FinalAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{

}

void Sampler_Curso_FinalAudioProcessor::releaseResources()
{

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Sampler_Curso_FinalAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void Sampler_Curso_FinalAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        ptrVolume[channel]->processVolume(buffer.getWritePointer(channel),
                                          buffer.getWritePointer(channel),
                                          0.5f,
                                          buffer.getNumSamples());
    }
}

bool Sampler_Curso_FinalAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* Sampler_Curso_FinalAudioProcessor::createEditor()
{
    return new Sampler_Curso_FinalAudioProcessorEditor (*this);
}

void Sampler_Curso_FinalAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{

}

void Sampler_Curso_FinalAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{

}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Sampler_Curso_FinalAudioProcessor();
}
