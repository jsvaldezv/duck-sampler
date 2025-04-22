#include "PluginProcessor.h"
#include "PluginEditor.h"

DuckSamplerAudioProcessor::DuckSamplerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts (*this, nullptr, "Parameters", createParameters())
#endif
{
    formatManager.registerBasicFormats();
    
    for (int i = 0; i < NUM_VOICES; i++)
        mySampler.addVoice (new juce::SamplerVoice());
}

DuckSamplerAudioProcessor::~DuckSamplerAudioProcessor()
{
    formatReader = nullptr;
}

const juce::String DuckSamplerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DuckSamplerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DuckSamplerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DuckSamplerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DuckSamplerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DuckSamplerAudioProcessor::getNumPrograms()
{
    return 1;
}

int DuckSamplerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DuckSamplerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DuckSamplerAudioProcessor::getProgramName (int index)
{
    return {};
}

void DuckSamplerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

void DuckSamplerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mySampler.setCurrentPlaybackSampleRate (sampleRate);
    
    lfo.prepare (sampleRate);
    
    updateADSR();
}

void DuckSamplerAudioProcessor::releaseResources() {}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DuckSamplerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void DuckSamplerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    for (const auto metadata : midiMessages)
    {
        const auto msg = metadata.getMessage();
        
        if (msg.isNoteOn())
            isNotePlayed = true;
        else if (msg.isNoteOff())
            isNotePlayed = false;
    }
    
    sampleCount = isNotePlayed ? sampleCount += buffer.getNumSamples() : 0;
    
    updateParameters();
    updateADSR();
    
    mySampler.renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());
    
    lfo.process (buffer);
    distortion.process (buffer);
    volume.process (buffer);
}

void DuckSamplerAudioProcessor::updateParameters()
{
    myADSRParameters.attack = apvts.getRawParameterValue(ATTACK)->load();
    myADSRParameters.decay = apvts.getRawParameterValue(DECAY)->load();
    myADSRParameters.sustain = apvts.getRawParameterValue(SUSTAIN)->load();
    myADSRParameters.release = apvts.getRawParameterValue(RELEASE)->load();
    
    lfo.setParameters (apvts.getRawParameterValue(RATE)->load());
    distortion.setParameters (!apvts.getRawParameterValue(DISTORTION)->load());
    volume.setParameters (apvts.getRawParameterValue(VOLUME)->load());
}

void DuckSamplerAudioProcessor::updateADSR()
{
    for (int i = 0; i < mySampler.getNumSounds(); ++i)
    {
        if (auto sound = dynamic_cast<juce::SamplerSound*> (mySampler.getSound(i).get()))
            sound->setEnvelopeParameters(myADSRParameters);
    }
}

void DuckSamplerAudioProcessor::loadFile (const juce::String& path)
{
    mySampler.clearSounds();
    
    auto file = juce::File (path);
    formatReader = formatManager.createReaderFor (file);
    
    auto sampleLength = static_cast<int> (formatReader->lengthInSamples);
    waveForm.setSize (1, sampleLength);
    
    formatReader->read (&waveForm, 0, sampleLength, 0, true, false);
    
    juce::BigInteger range;
    range.setRange (0, 128, true);
    
    mySampler.addSound (new juce::SamplerSound ("Sample", *formatReader, range, 60, 0.1, 0.1, 10));
    
    delete formatReader;
}

bool DuckSamplerAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* DuckSamplerAudioProcessor::createEditor()
{
    return new DuckSamplerAudioProcessorEditor (*this);
}

void DuckSamplerAudioProcessor::getStateInformation (juce::MemoryBlock& destData) {}

void DuckSamplerAudioProcessor::setStateInformation (const void* data, int sizeInBytes) {}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DuckSamplerAudioProcessor();
}
