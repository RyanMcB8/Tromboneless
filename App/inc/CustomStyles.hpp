/** @file       CustomStyles.hpp
 *  @author     Ryan McBride
 *  @brief      This file holds the declarations of all functions and variables
 *              needed by the Tromboneless project to change the looks and feel
 *              of various different widgets which are being displayed to the
 *              end user.
 */

#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include <stdexcept>
#include <string>
#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

/** @brief      Creation of a class which can be used to change the look of the sliders
 *              used within the equalizer for the Tromboneless project. 
 */
class VerticalSliderLookAndFeel : public juce::LookAndFeel_V4
{
    public:
        /** @brief          The constructor function for the `VerticalSliderLookAndFeel`
         *                  class that creates an instance of the look and feel style.
         *  @note           Currently there is not extra functionality attached to the 
         *                  contructor as the style of the vertical slider is defined
         *                  in the `drawLinearSlider()` function and there are no other
         *                  components being modified by this style currently.
         */
        VerticalSliderLookAndFeel();

        /** @brief          The destructor function for the `VerticalSliderLookAndFeel`
         *                  class which is currently set to default as there is no
         *                  need for a deconstructor for this class.
         */
        virtual ~VerticalSliderLookAndFeel() = default;

        /** @brief                  When this look and feel is attached to an object which has
         *                          a vertical slider in it, this function sets the look and
         *                          feel of the slider being used. This makes it look like
         *                          an average equalizer slider.
         *  @param  g               A pointer to the `juce::Graphics` instance which is being
         *                          drawn to. This is likely the main window or else the parent
         *                          component.
         *  @param  x               The integer representation of the slider's left corner in
         *                          the global window.
         *  @param  y               The integer representation of the sliders top corner in
         *                          the global window.
         *  @param  width           The integer representation of the width of the slider.
         *                          i.e. the difference between the left most and right most
         *                          part of the slider.
         *  @param  Height          The integer representation of the width of the slider.
         *                          i.e. the difference between the top most and bottom
         *                          most parts of the slider's bounds.
         *  @param  sliderPos       The position of the sliders finger. The position represents
         *                          x if the slider is horizontal and y if the slider is vertical.
         *  @param  minSliderPos    The minimum slider position.
         *  @param  maxSliderPos    The maximum slider position.
         *  @param  style           The style of the slider whose look and feel is getting 
         *                          modified. For example, LinearVertical or Rotary.
         *  @param  slider          A pointer to the slider which is being modified. This
         *                          can allow for extraction of more data from the slider.
         */
        virtual void drawLinearSlider(juce::Graphics& g, int x, int y, int width,
            int height, float sliderPos, float minSliderPos, float maxSliderPos,
            const juce::Slider::SliderStyle style, juce::Slider& slider) override;

private:
    /*  Setting the main colour of the knob (dial) of the slider. */
    const juce::Colour dialColour           = juce::Colours::white;

    /*  Setting the secondary colour of the knob (dial) of the slider.
        currently, this is for the lines within the knob. */
    const juce::Colour dialEdgeColour       = juce::Colours::red;

    /*  Setting the colour of the ticks along the slider. */
    const juce::Colour tickColour           = juce::Colours::darkgrey; 

    /*  Setting the colour of the track which the knob will be moving
        across. */
    const juce::Colour trackColour          = juce::Colours::darkgrey;

    /*  Setting the colour which will fill the bounds of the slider. */
    const juce::Colour backgroundColour     = juce::Colours::black; 

    /*  Setting the colour of the text which will be beside the slider. */
    const juce::Colour textColour           = juce::Colours::white;

    /*          Sizes.       */
    /*  This sets the track width of the slider. */
    float trackWidth          = 5;
    
    /*  This sets the height of the ticks along the slider. */
    float tickThickness       = 3.0;        

    /*  This sets the width of the ticks along the slider. */
    float tickWidth           = 10;         

/*  Checking the he slider should use a circular finger or rectangular. */
#ifdef USE_CIRCLE_FINGER        
    /*  This sets the radius of the finger of the dial position. */
    float fingerRadius        = 12;         
#else       
    /*  This sets the width of the finger of the slider. */
    float fingerWidth         = 25;     
    
    /*  This sets the height of the finger of the slider. */    
    float fingerHeight        = 10;         
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

/** @brief      A class which may be attached to a rotary slider from the juce framework
 *              to change its look and feel. This changes the look of the slider to have
 *              an arrow rotate about the centre of the slider pointing to the different
 *              values rather than a knob moving in an arc as the basic rotary slider.
 */
class NeedleLookAndFeel :   public juce::LookAndFeel_V4
{
    public:
        /** @brief                      The constructor function for the `NeedleLookAndFeel`
         *                              class which is set to default.
         */
        NeedleLookAndFeel() = default;

        /** @brief                      The destructor function for the `NeedleLookAndFeel`
         *                              class which is set to default.
         */
        virtual ~NeedleLookAndFeel() = default;

        /** @brief                      A function which is used to redraw the rotary
         *                              slider whose look and feel is being changed.
         *                              This will full describe how it should appear
         *                              to the user.
         *  @param  g                   A pointer to the `juce::Graphics` instance which
         *                              is being updated. This is the parent component or
         *                              the main window.
         *  @param  x                   The x position of the slider.
         *  @param  y                   The y position of the slider.
         *  @param  width               The width of the slider.
         *  @param  height              The maximum height of the slider.
         *  @param  sliderPos           The position of the slider finger.
         *  @param  rotaryStartAngle    The minimum angle which the slider starts at. This
         *                              represents the minimum value, typically bottom left.
         *  @param  rotaryEndAngle      The maximum angle which the slider ends at. This
         *                              represents the maximum value, typically bottom right.
         *  @param  slider              This is a pointer to the slider which is being drawn.
         *                              Extra data may be extracted from it if necessary.
         *  @note                       This function is a call back which is automatically
         *                              called to redraw the slider to look how described. It
         *                              should not need to be called by any high level function. 
         */
        virtual void drawRotarySlider(juce::Graphics& g, int x, int y, int width, 
            int height, float sliderPos, const float rotaryStartAngle,
                const float rotaryEndAngle, juce::Slider& slider) override;

    private:
        /*  Setting the colour of the needle/arrow which is being drawn. */
        const juce::Colour needleColour =   juce::Colours::black;


};

/** @brief                  A function which can draw an arc with a specified thickness
 *                          and nominal radius.
 *  @param  x               The x position of the centre point of the arc. This is set
 *                          to be 100 by default
 *  @param  y               The y position of the centre point of the arc. This is set
 *                          to be 100 by default.
 *  @param  thickness       The thickness of the arc which is being drawn. This is set 
 *                          to be 5 by default.
 *  @param  width           The radius in the x axis of the arc. This is set to be 50 by
 *                          default. 
 *  @param  height          the radius in the y axis of the arc. This is set to be 50 by
 *                          default.
 *  @param  angularRange    This sets the difference in minimum versus maximum angle of
 *                          the arc which is being drawn. This is measured in radians 
 *                          and is set to be 5 by default.
 *  @param  rotation        This parameter rotates the arc to be facing the orientated
 *                          the preferred way by the user. This function splits the
 *                          `angularRange` in half from the up direction and has the
 *                          left and right arcs of equal length. By default, this 
 *                          parameter is set to 0.
 *  @note                   This is built upon using the juce::centredArc function and
 *                          line functions to create a path. If the user chooses to 
 *                          draw the shape outline as opposed to fill, this will be 
 *                          visible.
 */
juce::Path RotaryArc(float x = 100, float y = 100, float thickness = 5, float width = 50, 
    float height = 50, float angularRange = 5, float rotation = 0);


/** @brief                  A simpified function for drawing a circle using the juce
 *                          framework reducing the number of parameters compared to
 *                          the ellipse function in juce.
 *  @param  x               The centre x position of the circle.
 *  @param  y               The centre y position of the circle.
 *  @param  xRadius         The radius of the circle.
 *  @param  yRadius         The radius of the circle.
 *  @note                   The reason there is an x and y radius is to compare which
 *                          is smaller and set the circles radius to that size to stop
 *                          the circle from being out of bounds or being distorted.
 */
juce::Path drawCircle(float x, float y, float xRadius, float yRadius);