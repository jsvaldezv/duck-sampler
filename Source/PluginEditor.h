#pragma once
#include <JuceHeader.h>
#include "Helpers/Parameters.h"
#include "PluginProcessor.h"
#include "Components/WaveformComponent.h"

class DuckSamplerAudioProcessorEditor : public juce::AudioProcessorEditor, public juce::FileDragAndDropTarget, public juce::Timer
{
public:
    
    DuckSamplerAudioProcessorEditor (DuckSamplerAudioProcessor&);
    ~DuckSamplerAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void prepareSlider (juce::Slider&, juce::Label&, juce::String);
    void createADSRComponents();
    void createGeneralComponents();
    
    bool isInterestedInFileDrag (const juce::StringArray& files) override;
    void filesDropped (const juce::StringArray& files, int x, int y) override;
    
    void timerCallback() override;

private:
    
    DuckSamplerAudioProcessor& audioProcessor;
    
    juce::String fileName { "" };
    
    // ADSR
    juce::Slider attackSlider, decaySlider, sustainSlider, releaseSlider;
    juce::Label attackLabel, decayLabel, sustainLabel, releaseLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackSliderAttachment, decaySliderAttachment, sustainSliderAttachment, releaseSliderAttachment;
    
    // General
    juce::Slider rateSlider, volumenSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rateSliderAttachment, volumenSliderAttachment;
    juce::Label rateLabel, volumenLabel;
    
    // Background
    juce::Image backgroundImage;
    juce::ImageComponent backgroundComponent;
    
    // Wave component
    WaveformComponent myWave;
    std::vector<float> audioPoints;
    
    // Distortion
    juce::ImageButton distorButton;
    juce::Image on_Distor, off_Distor;
    juce::Label distorLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> distorAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DuckSamplerAudioProcessorEditor)
};
