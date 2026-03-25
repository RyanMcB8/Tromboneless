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
    const juce::Colour dialColour           = juce::Colours::white;             /* Setting the colour of the finger. */
    const juce::Colour dialEdgeColour       = juce::Colours::grey;              /* Setting the colour of the inner part of the finger.*/
    const juce::Colour tickColour           = juce::Colours::darkgrey;          /* */
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

    private:
        const juce::Colour  emptyTrackColour= juce::Colours::darkgrey;
        const juce::Colour  fullTrackColour = juce::Colours::gold;
        const juce::Colour  thumbOneColour  = juce::Colours::yellow;
        const juce::Colour  thumbTwoColour  = juce::Colours::pink;

        float trackWidth        =   5;

};

/** @brief              A function which returns a path matching the perimeter of a trombone.
    @param  x           The x coordinate from which the paths minimum x starts. The default
                        value for this is 300.
    @param  y           The y coordinate from which the paths minimum y starts. The default 
                        value for this is 300.
    @param  width       The maximum width which the path is allowed to enclose. The default
                        value for this is 50.
    @param  height      The maximum height which the path is allowed to enclose. The default
                        value for this is 20.
    @param  incPipes    A boolean value determining if the trombones pipes should be included
                        within the path or whether only the end of the pipe should be
                        returned. The pipes are included by default.
    @retval             Returns a juce::Path which traces the perimeter of a trombone.*/
juce::Path PaintTrombone(float x=300, float y=300, float width=50, float height = 20, bool incPipes = true);


/** @brief              A function that traces out the shape of an arc based upon the thickness
 *                      of the arc and the gap between its inner sides. i.e the inner circle
 *                      diameter that would remove an area from the larger outer circle.
 *  @param  x           The x positon of the path, where this is the left most pixel of the
 *                      area allowed for the path.
 *  @param  y           The y position of the path, where thes is the top most pixel of the
 *                      area allowed by the path.
 *  @param  thickness   The thickness of the arc itself.
 *  @param  gap         The inner diameter of the arc itself. The arcs center diameter would
 *                      be the equivalent of gap + (thickness/2).
 *  @param  direction   A boolean option where 0 represents the arc having its outer face
 *                      facing right, and 1 represents the arc having its outer face
 *                      facing left.
 *  @retval             Returns a juce::Path which traces the perimeter of an arc.
 */
juce::Path PaintArc(float x, float y, float thickness, float gap, bool direction);

class BarometerLookAndFeel  :  public   juce::LookAndFeel_V4{
    public:
        BarometerLookAndFeel();

        virtual ~BarometerLookAndFeel() = default;

        virtual void drawRotarySlider(juce::Graphics& g, int x, int y, int width, 
            int height, float sliderPos, const float rotaryStartAngle,
             const float rotaryEndAngle, juce::Slider& slider) override;

    private:
        const juce::Colour  emptyTrackColour= juce::Colours::darkgrey;
        const juce::Colour  fullTrackColour = juce::Colours::gold;
        const juce::Colour  thumbOneColour  = juce::Colours::yellow;
        const juce::Colour  thumbTwoColour  = juce::Colours::pink;
        const juce::Colour  backgroundColour= juce::Colours::white;

        float trackWidth        =   5;

};

class BarometerOuterLookAndFeel  :  public   juce::LookAndFeel_V4{
    public:
        BarometerOuterLookAndFeel();

        virtual ~BarometerOuterLookAndFeel() = default;

        virtual void drawRotarySlider(juce::Graphics& g, int x, int y, int width, 
            int height, float sliderPos, const float rotaryStartAngle,
             const float rotaryEndAngle, juce::Slider& slider) override;

    private:
        const juce::Colour  thumbColour  = juce::Colours::yellow;

};

juce::Path RotaryArc(float x = 100, float y = 100, float thickness = 5, float width = 50, 
    float height = 50, float angularRange = 5, float rotation = 0);


juce::Path drawCircle(float x, float y, float xRadius, float yRadius);