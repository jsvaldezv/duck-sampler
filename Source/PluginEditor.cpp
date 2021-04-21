#include "PluginProcessor.h"
#include "PluginEditor.h"

Sampler_Curso_FinalAudioProcessorEditor::Sampler_Curso_FinalAudioProcessorEditor (Sampler_Curso_FinalAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (700, 500);
    createADSRComponents();
    //loadButton.onClick = [&]() {audioProcessor.loadFile();};
}

Sampler_Curso_FinalAudioProcessorEditor::~Sampler_Curso_FinalAudioProcessorEditor(){}

void Sampler_Curso_FinalAudioProcessorEditor::createADSRComponents()
{
    attackSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    attackSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    attackSlider.setBounds(0, 300, 100, 100);
    addAndMakeVisible(attackSlider);
    
    attackSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.parameters,
                                                                                                     "Attack",
                                                                                                     attackSlider);
    
    decaySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    decaySlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    decaySlider.setBounds(100, 300, 100, 100);
    addAndMakeVisible(decaySlider);
    
    decaySliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.parameters,
                                                                                                     "Decay",
                                                                                                     decaySlider);
    
    sustainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    sustainSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    sustainSlider.setBounds(200, 300, 100, 100);
    addAndMakeVisible(sustainSlider);
    
    sustainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.parameters,
                                                                                                     "Sustain",
                                                                                                     sustainSlider);
    
    releaseSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    releaseSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    releaseSlider.setBounds(300, 300, 100, 100);
    addAndMakeVisible(releaseSlider);
    
    releaseSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.parameters,
                                                                                                     "Release",
                                                                                                     releaseSlider);
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
        auto ratio = waveform.getNumSamples() / 600;
        auto buffer = waveform.getReadPointer(0);
        
        for(int sample = 0; sample < waveform.getNumSamples(); sample+=ratio)
        {
            audioPoints.push_back(buffer[sample]);
        }
        
        p.startNewSubPath(0, 200/2);
        
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
