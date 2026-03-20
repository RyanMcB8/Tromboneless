#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include <stdexcept>
#include <string>
#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

class VerticalSliderLookAndFeel : public juce::LookAndFeel_V4
{
public:
    void drawLinearSlider(juce::Graphics& g,
                          int x, int y, int width, int height,
                          float sliderPos,
                          float minSliderPos,
                          float maxSliderPos,
                          const juce::Slider::SliderStyle style,
                          juce::Slider& slider) override
    {
        /* Ensureing that only the vertical sliders are being changed */
        if (style == juce::Slider::LinearVertical)
        {
            auto bounds = juce::Rectangle<float>((float)x, (float)y, (float)width, (float)height);

            /* Adding a background colour to the slider. */
            g.setColour(backgroundColour);
            auto background = (bounds.withHeight(bounds.getHeight()));
            background = background.withSizeKeepingCentre(width, height*1.5);
            g.fillRect(background);

            /* Adding a track decal running the length of the slider. */
            g.setColour(trackColour);
            g.fillRect(bounds.withWidth(trackWidth).withCentre(bounds.getCentre()));

            /* Adding ticks to the slider */
            int numSteps = 10;
            g.setColour(tickColour);

            for (int i = 1; i <= numSteps-1; ++i){
                float proportion = (float)i / (float)numSteps;

                float yPos = bounds.getBottom() - proportion * bounds.getHeight();

                // Draw horizontal tick line (perpendicular to vertical slider)
                g.drawLine(bounds.getCentreX() - tickWidth, yPos,
                           bounds.getCentreX() + tickWidth, yPos, tickThickness);
            }
            
            /* Drawing the maxmimum and minimum ticks as wider than the rest of the ticks.*/
            g.drawLine(bounds.getCentreX() - tickWidth*1.5, bounds.getBottom(),
                           bounds.getCentreX() + tickWidth*1.5, bounds.getBottom(), tickThickness);

            g.drawLine(bounds.getCentreX() - tickWidth*1.5, bounds.getBottom() - bounds.getHeight(),
                           bounds.getCentreX() + tickWidth*1.5, bounds.getBottom() - bounds.getHeight(), tickThickness);

            /* Drawing the custom finger. */
            g.setColour(dialColour);
            g.fillEllipse(bounds.getCentreX() - fingerRadius, sliderPos - fingerRadius,
                          2*fingerRadius, 2*fingerRadius);

            g.setColour(dialEdgeColour);
            g.drawEllipse(bounds.getCentreX() - (0.9)*fingerRadius, sliderPos - (0.9 *fingerRadius),
                          (1.8*fingerRadius), (1.8*fingerRadius), 2);
        }
        else
        {
            // fallback to default for other styles
            juce::LookAndFeel_V4::drawLinearSlider(g, x, y, width, height,
                                                   sliderPos, minSliderPos, maxSliderPos,
                                                   style, slider);
        }
    }

    private:
        const juce::Colour dialColour           = juce::Colours::white;
        const juce::Colour dialEdgeColour       = juce::Colours::grey;
        const juce::Colour tickColour           = juce::Colours::darkgrey;
        const juce::Colour trackColour          = juce::Colours::darkgrey;
        const juce::Colour backgroundColour     = juce::Colours::black;

        /* Sizes. */
        float   trackWidth        = 5;          /* This sets the track width of the slider. */
        float tickThickness       = 3.0;        /* This sets the height of the ticks along the slider. */
        float tickWidth           = 10;         /* This sets the width of the ticks along the slider. */
        float fingerRadius        = 12;         /* This sets the radius of the finger of the dial position. */
};