#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include <stdexcept>
#include <string>
#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

/** @brief      Creation of a class which can be used to change the look of the sliders
 *              used within the equalizer for the Tromboneless projet. 
 */
class VerticalSliderLookAndFeel : public juce::LookAndFeel_V4
{
    public:
        VerticalSliderLookAndFeel();
        virtual ~VerticalSliderLookAndFeel() = default;
        virtual void drawLinearSlider(juce::Graphics& g, int x, int y, int width,
            int height, float sliderPos, float minSliderPos, float maxSliderPos,
            const juce::Slider::SliderStyle style, juce::Slider& slider) override;

private:
    const juce::Colour dialColour           = juce::Colours::white;
    const juce::Colour dialEdgeColour       = juce::Colours::grey;
    const juce::Colour tickColour           = juce::Colours::darkgrey;
    const juce::Colour trackColour          = juce::Colours::darkgrey;
    const juce::Colour backgroundColour     = juce::Colours::black; //juce::Colours::black;

    /* Sizes. */
    float trackWidth          = 5;          /* This sets the track width of the slider. */
    float tickThickness       = 3.0;        /* This sets the height of the ticks along the slider. */
    float tickWidth           = 10;         /* This sets the width of the ticks along the slider. */

#ifdef USE_CIRCLE_FINGER        
    float fingerRadius        = 12;         /* This sets the radius of the finger of the dial position. */
#else        
    float fingerWidth         = 25;         /* This sets the width of the finger of the slider. */
    float fingerHeight        = 10;          /* This sets the height of the finger of the slider. */
#endif        
};

class CalibrationSliderLookAndFeel  :  public   juce::LookAndFeel_V4{
    public:
        CalibrationSliderLookAndFeel();

        virtual ~CalibrationSliderLookAndFeel() = default;

        virtual void drawLinearSlider(juce::Graphics& g, int x, int y,
            int width, int height, float sliderPos, float minSliderPos, 
            float maxSliderPos, const juce::Slider::SliderStyle style, 
            juce::Slider& slider);

        // virtual void drawSlider(juce::Graphics& g, juce::Slider& slider) override;
    private:
        const juce::Colour  emptyTrackColour= juce::Colours::darkgrey;
        const juce::Colour  fullTrackColour = juce::Colours::gold;
        const juce::Colour  thumbOneColour  = juce::Colours::yellow;
        const juce::Colour  thumbTwoColour  = juce::Colours::pink;

        float trackWidth        =   5;

};

juce::Path PaintTrombone(float x=300, float y=300, float width=50);
