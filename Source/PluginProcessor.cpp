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
                       ), parameters(*this, nullptr, "PARAMETERS", initializeGUI())
#endif
{
    formatManager.registerBasicFormats();
    
    for(int i = 0; i < 3; i++)
    {
        mySampler.addVoice (new juce::SamplerVoice());
    }
    
    for(int i = 0; i < getTotalNumOutputChannels(); i++)
    {
        ptrVolume[i] = std::unique_ptr<sampler_Volume>(new sampler_Volume());
        ptrLFO[i] = std::unique_ptr<sampler_LFO>(new sampler_LFO());
        ptrDistor[i] = std::unique_ptr<sampler_Distorsion>(new sampler_Distorsion());
    }
}

Sampler_Curso_FinalAudioProcessor::~Sampler_Curso_FinalAudioProcessor()
{
    formatReader = nullptr;
}

juce::AudioProcessorValueTreeState::ParameterLayout Sampler_Curso_FinalAudioProcessor::initializeGUI()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("volume", 1),
                                                                 "Volume",
                                                                 0.0f,
                                                                 1.0f,
                                                                 0.5f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("rate", 1),
                                                                 "Rate",
                                                                 0.1f,
                                                                 20.0f,
                                                                 1.0f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("attack", 1),
                                                                 "Attack",
                                                                 0.1f,
                                                                 1.0f,
                                                                 0.1f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("decay", 1),
                                                                 "Decay",
                                                                 0.1f,
                                                                 1.0f,
                                                                 0.1f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("sustain", 1),
                                                                 "Sustain",
                                                                 0.1f,
                                                                 1.0f,
                                                                 0.1f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("release", 1),
                                                                 "Release",
                                                                 0.1f,
                                                                 3.0f,
                                                                 0.1f));
    
    params.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("distorsion", 1),
                                                                "Distorsion",
                                                                false));
    
    return {params.begin(), params.end()};
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
    mySampler.setCurrentPlaybackSampleRate(sampleRate);
    
    for(int i = 0; i < getTotalNumOutputChannels(); i++)
    {
        ptrLFO[i]->prepareLFO(sampleRate);
    }
    
    updateADSR();
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

void Sampler_Curso_FinalAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                                      juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    juce::MidiMessage m;
    juce::MidiBuffer::Iterator it {midiMessages};
    int sample;
    
    while(it.getNextEvent(m, sample))
    {
        if(m.isNoteOn())
            isNotePlayed = true;
        else if(m.isNoteOff())
            isNotePlayed = false;
    }
    
    sampleCount = isNotePlayed ? sampleCount += buffer.getNumSamples() : 0;
    
    myADSRParameters.attack = *parameters.getRawParameterValue("attack");
    myADSRParameters.decay = *parameters.getRawParameterValue("decay");
    myADSRParameters.sustain = *parameters.getRawParameterValue("sustain");
    myADSRParameters.release = *parameters.getRawParameterValue("release");
    updateADSR();
    
    mySampler.renderNextBlock(buffer,
                              midiMessages,
                              0,
                              buffer.getNumSamples());
    
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        ptrLFO[channel]->processLFO(buffer.getWritePointer(channel),
                                    buffer.getWritePointer(channel),
                                    *parameters.getRawParameterValue("rate"),
                                    buffer.getNumSamples());
        
        ptrDistor[channel]->processDistor(buffer.getWritePointer(channel),
                                          buffer.getWritePointer(channel),
                                          *parameters.getRawParameterValue("distorsion"),
                                          buffer.getNumSamples());
        
        ptrVolume[channel]->processVolume(buffer.getWritePointer(channel),
                                          buffer.getWritePointer(channel),
                                          *parameters.getRawParameterValue("volume"),
                                          buffer.getNumSamples());
    }
}

void Sampler_Curso_FinalAudioProcessor::loadFile()
{
    juce::FileChooser chooser {"Carga tu sonido"};
    
    if(chooser.browseForFileToOpen())
    {
        auto file = chooser.getResult();
        formatReader = formatManager.createReaderFor(file);
    }
    
    juce::BigInteger range;
    range.setRange(0, 128, true);
    
    mySampler.addSound(new juce::SamplerSound("Sample", *formatReader, range, 60, 0.1, 0.1, 10));
}

void Sampler_Curso_FinalAudioProcessor::loadFile(const juce::String& path)
{
    mySampler.clearSounds();
    auto file = juce::File(path);
    formatReader = formatManager.createReaderFor(file);
    
    auto sampleLength = static_cast<int>(formatReader->lengthInSamples);
    waveForm.setSize(1, sampleLength);
    formatReader->read(&waveForm,
                       0,
                       sampleLength,
                       0,
                       true,
                       false);
    
    juce::BigInteger range;
    range.setRange(0, 128, true);
    mySampler.addSound(new juce::SamplerSound("Sample", *formatReader, range, 60, 0.1, 0.1, 10));
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

void Sampler_Curso_FinalAudioProcessor::updateADSR()
{
    for(int i = 0; i < mySampler.getNumSounds(); ++i)
    {
        if(auto sound = dynamic_cast<juce::SamplerSound*>(mySampler.getSound(i).get()))
        {
            sound->setEnvelopeParameters(myADSRParameters);
        }
    }
}
