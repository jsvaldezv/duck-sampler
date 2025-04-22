#include "PluginProcessor.h"
#include "PluginEditor.h"

DuckSamplerAudioProcessorEditor::DuckSamplerAudioProcessorEditor (DuckSamplerAudioProcessor& p) : AudioProcessorEditor (&p), audioProcessor (p), myWave(p)
{
    // Images
    backgroundImage = juce::ImageCache::getFromMemory (BinaryData::back_jpg, BinaryData::back_jpgSize);
    on_Distor = juce::ImageCache::getFromMemory (BinaryData::on_png, BinaryData::on_pngSize);
    off_Distor = juce::ImageCache::getFromMemory(BinaryData::off_png, BinaryData::off_pngSize);

    // Background
    backgroundComponent.setImage (backgroundImage, juce::RectanglePlacement::stretchToFit);
    
    // Components
    addAndMakeVisible (backgroundComponent);
    addAndMakeVisible (myWave);
    
    createADSRComponents();
    createGeneralComponents();
    
    // L&F
    getLookAndFeel().setColour (juce::Slider::thumbColourId, juce::Colours::yellow);
    getLookAndFeel().setColour (juce::Slider::rotarySliderFillColourId, juce::Colour(226, 223, 0));
    
    startTimer (60);
    setSize (700, 500);
}

DuckSamplerAudioProcessorEditor::~DuckSamplerAudioProcessorEditor()
{
    stopTimer();
}

void DuckSamplerAudioProcessorEditor::resized()
{
    backgroundComponent.setBounds (0, 0, getWidth(), getHeight());
    myWave.setBounds (0, 0, getWidth(), getHeight() * 0.4f);
    
    rateSlider.setBounds (250, 370, 100, 100);
    volumenSlider.setBounds (350, 370, 100, 100);
    attackSlider.setBounds (150, 230, 100, 100);
    decaySlider.setBounds (250, 230, 100, 100);
    sustainSlider.setBounds (350, 230, 100, 100);
    releaseSlider.setBounds (450, 230, 100, 100);
    
    distorButton.setBounds (getWidth() - 90, getHeight() - 90, 80, 80);
}

void DuckSamplerAudioProcessorEditor::prepareSlider (juce::Slider& slider, juce::Label& label, juce::String sliderID)
{
    slider.setSliderStyle (juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle (juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    addAndMakeVisible (slider);
    
    label.setText (sliderID, juce::dontSendNotification);
    label.attachToComponent (&slider, false);
    label.setJustificationType (juce::Justification::centred);
    label.setColour (juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible (label);
}

void DuckSamplerAudioProcessorEditor::timerCallback()
{
    repaint();
}

void DuckSamplerAudioProcessorEditor::createGeneralComponents()
{
    prepareSlider (rateSlider, rateLabel, RATE);
    prepareSlider (volumenSlider, volumenLabel, VOLUME);
    
    rateSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.getAPVTS(), RATE, rateSlider);
    volumenSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.getAPVTS(), VOLUME, volumenSlider);
    
    // Distortion button
    distorButton.setClickingTogglesState (true);
    distorButton.setImages (false, true, false, off_Distor, 1.0f, {}, off_Distor, 1.0f, {}, on_Distor, 1.0f, {});
    addAndMakeVisible (distorButton);
    
    distorLabel.setText ("Distortion", juce::dontSendNotification);
    distorLabel.attachToComponent (&distorButton, false);
    distorLabel.setJustificationType (juce::Justification::centred);
    distorLabel.setColour (juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible (distorLabel);
    
    distorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (audioProcessor.getAPVTS(), DISTORTION, distorButton);
}

void DuckSamplerAudioProcessorEditor::createADSRComponents()
{
    prepareSlider (attackSlider, attackLabel, ATTACK);
    prepareSlider (decaySlider, decayLabel, DECAY);
    prepareSlider (sustainSlider, sustainLabel, SUSTAIN);
    prepareSlider (releaseSlider, releaseLabel, RELEASE);
    
    attackSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.getAPVTS(), ATTACK, attackSlider);
    decaySliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.getAPVTS(), DECAY, decaySlider);
    sustainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.getAPVTS(), SUSTAIN, sustainSlider);
    releaseSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.getAPVTS(), RELEASE, releaseSlider);
}

void DuckSamplerAudioProcessorEditor::paint (juce::Graphics& g) {}

bool DuckSamplerAudioProcessorEditor::isInterestedInFileDrag (const juce::StringArray& files)
{
    for (auto file : files)
    {
        if (file.contains(".wav") || file.contains(".mp3") || file.contains(".aiff"))
            return true;
    }
    
    return false;
}

void DuckSamplerAudioProcessorEditor::filesDropped (const juce::StringArray& files, int x, int y)
{
    for (auto file : files)
    {
        if (isInterestedInFileDrag(file))
        {
            auto myFile = std::make_unique<juce::File> (file);
            fileName = myFile->getFileNameWithoutExtension() + myFile->getFileExtension();
            audioProcessor.loadFile (file);
        }
    }
    
    repaint();
}
