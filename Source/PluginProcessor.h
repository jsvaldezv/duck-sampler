#pragma once
#include <JuceHeader.h>
#include "sampler_Volume.h"
#include "sampler_LFO.h"

class Sampler_Curso_FinalAudioProcessor  : public juce::AudioProcessor
{
public:

    Sampler_Curso_FinalAudioProcessor();
    ~Sampler_Curso_FinalAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
    const juce::String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    juce::AudioProcessorValueTreeState parameters;
    juce::AudioProcessorValueTreeState::ParameterLayout initializeGUI();
    
    void loadFile();
    void loadFile(const juce::String& path);
    int getNumSamplerSounds(){return mySampler.getNumSounds();};

private:
    
    juce::Synthesiser mySampler;
    
    juce::AudioFormatManager formatManager;
    juce::AudioFormatReader* formatReader {nullptr};

    std::unique_ptr<sampler_Volume> ptrVolume[2];
    std::unique_ptr<sampler_LFO> ptrLFO[2];
        
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Sampler_Curso_FinalAudioProcessor)
};
