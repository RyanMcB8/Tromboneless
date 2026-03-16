#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include <stdexcept>
#include <string>
#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
// #include shared header

/** @brief A class that adds more functionality to the slider class from Juce. */
class MinDifferenceSlider : public juce::Slider
{
public:
    double minDistance = 1;

    /** @brief A function which sets the minimum difference between the upper and lower fingers of the slider.
     *  @param difference The value of the difference between the fingers.
     */
    void setMinDifference(double difference){
        minDistance = difference;
    }

    void valueChanged() override
    {
        auto min = getMinValue();
        auto max = getMaxValue();

        if (max - min < minDistance)
        {
            if (getThumbBeingDragged() == 1)
                setMaxValue(min + minDistance);
            else
                setMinValue(max - minDistance);
        }
    }
};