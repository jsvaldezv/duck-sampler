#include "PluginProcessor.h"
#include "PluginEditor.h"

Sampler_Curso_FinalAudioProcessorEditor::Sampler_Curso_FinalAudioProcessorEditor (Sampler_Curso_FinalAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    auto& params = processor.getParameters();
    setSize (200, 200);
    
    loadButton.onClick = [&]() {audioProcessor.loadFile();};
    addAndMakeVisible(loadButton);
}

Sampler_Curso_FinalAudioProcessorEditor::~Sampler_Curso_FinalAudioProcessorEditor()
{
}

void Sampler_Curso_FinalAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Sampler", getLocalBounds(), juce::Justification::centred, 1);
}

void Sampler_Curso_FinalAudioProcessorEditor::resized()
{
    loadButton.setBounds(getWidth()/2-50, getHeight()/2-50, 100, 100);
}
