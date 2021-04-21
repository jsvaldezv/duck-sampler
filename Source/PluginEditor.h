#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class Sampler_Curso_FinalAudioProcessorEditor : public juce::AudioProcessorEditor,
                                                public juce::FileDragAndDropTarget
{
public:
    
    Sampler_Curso_FinalAudioProcessorEditor (Sampler_Curso_FinalAudioProcessor&);
    ~Sampler_Curso_FinalAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    bool isInterestedInFileDrag (const juce::StringArray& files) override;
    void filesDropped (const juce::StringArray& files, int x, int y) override;
    
    void createADSRComponents();

private:
    
    juce::TextButton loadButton {"Load"};
    
    std::vector<float> audioPoints;
    bool shouldBePaint {false};
    
    //ATTACK SLIDER
    juce::Slider attackSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackSliderAttachment;
    //DECAY SLIDER
    juce::Slider decaySlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> decaySliderAttachment;
    //SUSTAIN SLIDER
    juce::Slider sustainSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sustainSliderAttachment;
    //RELEASE SLIDER
    juce::Slider releaseSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseSliderAttachment;
    //RATE SLIDER
    juce::Slider rateSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rateSliderAttachment;
    //VOLUMEN SLIDER
    juce::Slider volumenSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> volumenSliderAttachment;
    
    Sampler_Curso_FinalAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Sampler_Curso_FinalAudioProcessorEditor)
};
