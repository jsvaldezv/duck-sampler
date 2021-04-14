#include "PluginProcessor.h"
#include "PluginEditor.h"

Sampler_Curso_FinalAudioProcessorEditor::Sampler_Curso_FinalAudioProcessorEditor (Sampler_Curso_FinalAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (600, 200);
    loadButton.onClick = [&]() {audioProcessor.loadFile();};
}

Sampler_Curso_FinalAudioProcessorEditor::~Sampler_Curso_FinalAudioProcessorEditor()
{
}

void Sampler_Curso_FinalAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    g.setColour (juce::Colours::white);
    
    if(shouldBePaint)
    {
        juce::Path p;
        audioPoints.clear();
        
        auto waveform = audioProcessor.getWaveform();
        auto ratio = waveform.getNumSamples() / getWidth();
        auto buffer = waveform.getReadPointer(0);
        
        for(int sample = 0; sample < waveform.getNumSamples(); sample+=ratio)
        {
            audioPoints.push_back(buffer[sample]);
        }
        
        p.startNewSubPath(0, getHeight()/2);
        
        for(int sample = 0; sample < audioPoints.size(); ++sample)
        {
            auto point = juce::jmap(audioPoints[sample], -1.0f, 1.0f, 0.0f, 200.0f);
            p.lineTo(sample, point);
        }
        
        g.strokePath(p, juce::PathStrokeType(1));
        shouldBePaint = false;
    }
    
    /*if(audioProcessor.getNumSamplerSounds() > 0)
    {
        g.fillAll(juce::Colours::green);
        g.drawFittedText ("LOADED", getLocalBounds(), juce::Justification::centred, 1);
    }
    else{
        g.drawFittedText ("LOAD A FILE", getLocalBounds(), juce::Justification::centred, 1);
    }*/
}

void Sampler_Curso_FinalAudioProcessorEditor::resized()
{
    //loadButton.setBounds(getWidth()/2-50, getHeight()/2-50, 100, 100);
}

bool Sampler_Curso_FinalAudioProcessorEditor::isInterestedInFileDrag (const juce::StringArray& files)
{
    for(auto file : files)
    {
        if(file.contains(".wav") || file.contains(".mp3") || file.contains(".aiff"))
        {
            return true;
        }
    }
    
    return false;
}

void Sampler_Curso_FinalAudioProcessorEditor::filesDropped (const juce::StringArray& files,
                                                            int x,
                                                            int y)
{
    for(auto file : files)
    {
        if(isInterestedInFileDrag(file))
        {
            shouldBePaint = true;
            audioProcessor.loadFile(file);
        }
    }
    
    repaint();
    
}
