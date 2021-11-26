#include "PluginProcessor.h"
#include "PluginEditor.h"

Sampler_Curso_FinalAudioProcessorEditor::Sampler_Curso_FinalAudioProcessorEditor (Sampler_Curso_FinalAudioProcessor& p)
    : AudioProcessorEditor (&p), myWave(p), audioProcessor (p)
{
    setSize (700, 500);
    
    backgroundImage = juce::ImageCache::getFromMemory(BinaryData::back_jpg,
                                                      BinaryData::back_jpgSize);
    
    on_Distor = juce::ImageCache::getFromMemory(BinaryData::on_png,
                                                BinaryData::on_pngSize);
    
    off_Distor = juce::ImageCache::getFromMemory(BinaryData::off_png,
                                                 BinaryData::off_pngSize);

    backgroundComponent.setImage(backgroundImage, juce::RectanglePlacement::stretchToFit);
    addAndMakeVisible(backgroundComponent);
    
    addAndMakeVisible(myWave);
    
    createADSRComponents();
    createGeneralComponents();
    
    getLookAndFeel().setColour (juce::Slider::thumbColourId, juce::Colours::yellow);
    getLookAndFeel().setColour (juce::Slider::rotarySliderFillColourId, juce::Colour(226, 223, 0));
    
    startTimer(60);
}

Sampler_Curso_FinalAudioProcessorEditor::~Sampler_Curso_FinalAudioProcessorEditor()
{
    stopTimer();
}

void Sampler_Curso_FinalAudioProcessorEditor::resized()
{
    backgroundComponent.setBounds(0, 0, getWidth(), getHeight());
    
    myWave.setBounds(0, 0, myWave.getWidth(), myWave.getHeight());
}

void Sampler_Curso_FinalAudioProcessorEditor::timerCallback()
{
    repaint();
}

void Sampler_Curso_FinalAudioProcessorEditor::createGeneralComponents()
{
    rateSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    rateSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    rateSlider.setBounds(250, 370, 100, 100);
    addAndMakeVisible(rateSlider);
    
    rateSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.parameters,
                                                                                                   "rate",
                                                                                                   rateSlider);
    
    rateLabel.setText("Rate", juce::dontSendNotification);
    rateLabel.attachToComponent(&rateSlider, true);
    rateLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    rateLabel.setCentrePosition(rateSlider.getX() + 50, rateSlider.getY() - 5);
    addAndMakeVisible(rateLabel);
    
    volumenSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    volumenSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    volumenSlider.setBounds(350, 370, 100, 100);
    addAndMakeVisible(volumenSlider);
    
    volumenLabel.setText("Volumen", juce::dontSendNotification);
    volumenLabel.attachToComponent(&volumenSlider, true);
    volumenLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    volumenLabel.setCentrePosition(volumenSlider.getX() + 50, volumenSlider.getY() - 5);
    addAndMakeVisible(volumenLabel);
    
    volumenSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.parameters,
                                                                                                      "volume",
                                                                                                      volumenSlider);
    
    distorButton.setClickingTogglesState(true);
    distorButton.setBounds(getWidth() - 90, getHeight() - 90, 80, 80);
    distorButton.setImages(false, true, false, off_Distor, 1.0f, {}, off_Distor, 1.0f, {}, on_Distor, 1.0f, {});
    addAndMakeVisible(distorButton);
    distorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (audioProcessor.parameters,
                                                                                               "distorsion",
                                                                                               distorButton);
    distorLabel.setText("Distortion", juce::dontSendNotification);
    distorLabel.attachToComponent(&distorButton, true);
    distorLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    distorLabel.setCentrePosition(distorButton.getX() + 36, distorButton.getY() - 5);
    addAndMakeVisible(distorLabel);
}

void Sampler_Curso_FinalAudioProcessorEditor::createADSRComponents()
{
    attackSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    attackSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    attackSlider.setBounds(150, 230, 100, 100);
    addAndMakeVisible(attackSlider);
    
    attackLabel.setText("Attack", juce::dontSendNotification);
    attackLabel.attachToComponent(&attackSlider, true);
    attackLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    attackLabel.setCentrePosition(attackSlider.getX() + 50, attackSlider.getY() - 5);
    addAndMakeVisible(attackLabel);
    
    attackSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.parameters,
                                                                                                     "attack",
                                                                                                     attackSlider);
    
    decaySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    decaySlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    decaySlider.setBounds(250, 230, 100, 100);
    addAndMakeVisible(decaySlider);
    
    decayLabel.setText("Decay", juce::dontSendNotification);
    decayLabel.attachToComponent(&decaySlider, true);
    decayLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    decayLabel.setCentrePosition(decaySlider.getX() + 50, decaySlider.getY() - 5);
    addAndMakeVisible(decayLabel);
    
    decaySliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.parameters,
                                                                                                     "decay",
                                                                                                     decaySlider);
    
    sustainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    sustainSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    sustainSlider.setBounds(350, 230, 100, 100);
    addAndMakeVisible(sustainSlider);
    
    sustainLabel.setText("Sustain", juce::dontSendNotification);
    sustainLabel.attachToComponent(&sustainSlider, true);
    sustainLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    sustainLabel.setCentrePosition(sustainSlider.getX() + 50, sustainSlider.getY() - 5);
    addAndMakeVisible(sustainLabel);
    
    sustainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.parameters,
                                                                                                     "sustain",
                                                                                                     sustainSlider);
    
    releaseSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    releaseSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    releaseSlider.setBounds(450, 230, 100, 100);
    addAndMakeVisible(releaseSlider);
    
    releaseLabel.setText("Release", juce::dontSendNotification);
    releaseLabel.attachToComponent(&releaseSlider, true);
    releaseLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    releaseLabel.setCentrePosition(releaseSlider.getX() + 50, releaseSlider.getY() - 5);
    addAndMakeVisible(releaseLabel);
    
    releaseSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.parameters,
                                                                                                     "release",
                                                                                                     releaseSlider);
}

void Sampler_Curso_FinalAudioProcessorEditor::paint (juce::Graphics& g){}

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
