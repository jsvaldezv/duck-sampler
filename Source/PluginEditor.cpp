#include "PluginProcessor.h"
#include "PluginEditor.h"

Sampler_Curso_FinalAudioProcessorEditor::Sampler_Curso_FinalAudioProcessorEditor (Sampler_Curso_FinalAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (700, 500);
    createADSRComponents();
    createGeneralComponents();
}

Sampler_Curso_FinalAudioProcessorEditor::~Sampler_Curso_FinalAudioProcessorEditor(){}

void Sampler_Curso_FinalAudioProcessorEditor::createGeneralComponents()
{
    rateSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    rateSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    rateSlider.setBounds(250, 320, 100, 100);
    addAndMakeVisible(rateSlider);
    
    rateSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.parameters,
                                                                                                   "Rate",
                                                                                                   rateSlider);
    
    volumenSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    volumenSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    volumenSlider.setBounds(350, 320, 100, 100);
    addAndMakeVisible(volumenSlider);
    
    volumenSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.parameters,
                                                                                                      "Volume",
                                                                                                      volumenSlider);
}

void Sampler_Curso_FinalAudioProcessorEditor::createADSRComponents()
{
    attackSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    attackSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    attackSlider.setBounds(150, 220, 100, 100);
    addAndMakeVisible(attackSlider);
    
    attackSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.parameters,
                                                                                                     "Attack",
                                                                                                     attackSlider);
    
    decaySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    decaySlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    decaySlider.setBounds(250, 220, 100, 100);
    addAndMakeVisible(decaySlider);
    
    decaySliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.parameters,
                                                                                                     "Decay",
                                                                                                     decaySlider);
    
    sustainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    sustainSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    sustainSlider.setBounds(350, 220, 100, 100);
    addAndMakeVisible(sustainSlider);
    
    sustainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.parameters,
                                                                                                     "Sustain",
                                                                                                     sustainSlider);
    
    releaseSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    releaseSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    releaseSlider.setBounds(450, 220, 100, 100);
    addAndMakeVisible(releaseSlider);
    
    releaseSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.parameters,
                                                                                                     "Release",
                                                                                                     releaseSlider);
}

void Sampler_Curso_FinalAudioProcessorEditor::paint (juce::Graphics& g)
{    
    g.fillAll(juce::Colours::cadetblue.darker());
    auto waveform = audioProcessor.getWaveform();
    
    if(waveform.getNumSamples() > 0)
    {
        juce::Path p;
        audioPoints.clear();
        
        auto ratio = waveform.getNumSamples() / 700;
        auto buffer = waveform.getReadPointer(0);
        
        for(int sample = 0; sample < waveform.getNumSamples(); sample+=ratio)
        {
            audioPoints.push_back(buffer[sample]);
        }
        
        g.setColour(juce::Colours::yellow);
        p.startNewSubPath(0, 200/2);
        
        for(int sample = 0; sample < audioPoints.size(); ++sample)
        {
            auto point = juce::jmap(audioPoints[sample], -1.0f, 1.0f, 0.0f, 200.0f);
            p.lineTo(sample, point);
        }
        
        g.strokePath(p, juce::PathStrokeType(1));
        
        g.setColour(juce::Colours::white);
        g.setFont(15.0f);
        auto textBounds = getLocalBounds().reduced(10, 10);
        g.drawFittedText(fileName, textBounds, juce::Justification::topRight, 1);
    }
    else
    {
        g.setColour(juce::Colours::white);
        g.setFont(40.0f);
        g.drawFittedText("Jala un audio", 250, 50, 200, 40, juce::Justification::centred, 1);
    }
}

void Sampler_Curso_FinalAudioProcessorEditor::resized(){}

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
            auto myFile = std::make_unique<juce::File>(file);
            fileName = myFile->getFileNameWithoutExtension() + myFile->getFileExtension();
            audioProcessor.loadFile(file);
        }
    }
    
    repaint();
}
