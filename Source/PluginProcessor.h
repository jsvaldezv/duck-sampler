#pragma once
#include <JuceHeader.h>
#include "Helpers/Parameters.h"
#include "DSP/Volume.h"
#include "DSP/LFO.h"
#include "DSP/Distortion.h"

class DuckSamplerAudioProcessor : public juce::AudioProcessor
{
public:

    DuckSamplerAudioProcessor();
    ~DuckSamplerAudioProcessor() override;

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
    
    juce::AudioProcessorValueTreeState& getAPVTS() { return apvts; }
    
    juce::AudioBuffer<float>& getWaveform() { return waveForm; }
    
    std::atomic<bool>& isNotPlayed() { return isNotePlayed; }
    std::atomic<int>& getSampleCount() { return sampleCount; }
    int getNumSamplerSounds() { return mySampler.getNumSounds(); }
    
    void loadFile (const juce::String& path);
    
private:
    
    juce::AudioProcessorValueTreeState apvts;
    void updateParameters();
    
    juce::Synthesiser mySampler;
    
    std::atomic<bool> isNotePlayed { false };
    std::atomic<int> sampleCount { 0 };
    
    void updateADSR();
    juce::ADSR::Parameters myADSRParameters;
    
    Volume volume;
    LFO lfo;
    Distortion distortion;
    
    juce::AudioBuffer<float> waveForm;
    
    juce::AudioFormatManager formatManager;
    juce::AudioFormatReader* formatReader { nullptr };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DuckSamplerAudioProcessor)
};
