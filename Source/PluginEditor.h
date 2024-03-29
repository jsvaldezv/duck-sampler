#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "sampler_Waveform.h"

class Sampler_Curso_FinalAudioProcessorEditor : public juce::AudioProcessorEditor,
                                                public juce::FileDragAndDropTarget,
                                                public juce::Timer
{
public:
    
    Sampler_Curso_FinalAudioProcessorEditor (Sampler_Curso_FinalAudioProcessor&);
    ~Sampler_Curso_FinalAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    bool isInterestedInFileDrag (const juce::StringArray& files) override;
    void filesDropped (const juce::StringArray& files, int x, int y) override;
    
    void createADSRComponents();
    void createGeneralComponents();
    
    void timerCallback() override;

private:
    
    std::vector<float> audioPoints;
    juce::String fileName {""};
    
    //****************************************** ADSR COMPONENTS *****************************************//
    //ATTACK SLIDER
    juce::Slider attackSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackSliderAttachment;
    juce::Label attackLabel;
    //DECAY SLIDER
    juce::Slider decaySlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> decaySliderAttachment;
    juce::Label decayLabel;
    //SUSTAIN SLIDER
    juce::Slider sustainSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sustainSliderAttachment;
    juce::Label sustainLabel;
    //RELEASE SLIDER
    juce::Slider releaseSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseSliderAttachment;
    juce::Label releaseLabel;
    
    //****************************************** GENERAL COMPONENTS*****************************************//
    //RATE SLIDER
    juce::Slider rateSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rateSliderAttachment;
    juce::Label rateLabel;
    //VOLUMEN SLIDER
    juce::Slider volumenSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> volumenSliderAttachment;
    juce::Label volumenLabel;
    
    //****************************************** BACKGROUND *****************************************//
    juce::Image backgroundImage;
    juce::ImageComponent backgroundComponent;
    
    //****************************************** WAVE *****************************************//
    sampler_Waveform myWave;
    
    //****************************************** DISTORTION *****************************************//
    juce::ImageButton distorButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> distorAttachment;
    juce::Image on_Distor;
    juce::Image off_Distor;
    juce::Label distorLabel;
    
    Sampler_Curso_FinalAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Sampler_Curso_FinalAudioProcessorEditor)
};
