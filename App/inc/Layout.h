#pragma once
#include "tromboneless_data.h"
#include <juce_gui_extra/juce_gui_extra.h>
#include <stdexcept>
#include <string>
#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "Widgets.h"

#include <stdio.h>

// #define DBG_MSG

/** @brief Main class that creates the window
 */
class Layout final : public juce::Component,
                     public juce::Slider::Listener
{
    public:

        // Constructor
        Layout();

        // Deconstructor
        ~Layout() override;

        /** @brief functions that sets the background colour of the window
        @param  g The panel which should be modified
        @retval void
        @note Always sets the background to the user preference for the background.
        * This function also adds a title to be within the window.
        */  
        void paint (juce::Graphics&) override;


        /** @brief Function that changes the size of the profiler
        @retval no return value: void
        @note Resizes the contents of the window including dropdown menus, labels and buttons
        */
        void resized() override;
        
        /** @brief Function that is triggered when a key is pressed
        @param key An instance of KeyPress that stores the value of the key being pressed
        @retval boolean value of true if a key is has been handled, otherwise false
        */
        bool keyPressed(const juce::KeyPress& key) override;

        double minimumDistance;    /* Setting the minimum distance that the tromboneless' slider will reach. */ 
        double maximumDistance;    /* Setting the maximum distance that the tromboneless' slider will reach. */
        double distanceRange;      /* The minimum distance between the maximum and minimum distabces allowed. */
        double stepDistance;       /* The minimum change between values when the user adjusts the distance slider. */
        
        double minimumPressure;    /* The gain factor of the minimum pressure that could be read relative to the normal maximum. */
        double maximumPressure;    /* The gain factor of the maxmimum pressure that could be read relative to the normal maximum. */
        double stepPressure;       /* The granularity of how fine the user may adjust the pressure slider relative to the normal maximum. */
        double pressureRange;      /* The minimum pressure pressure difference between the maximum and minimum pressure values relative to the normal maximum. */

            
        /** @brief A function handler for when any of the sliders on screen are moved.
         *  @param slider A pointer to the slider that has been modified
         *  @note This function must check which slider has been modified before making any changes.
         */
        void sliderValueChanged (juce::Slider* slider) override
        {
            /* Calibrated distance has changed. */
            if (slider == &distanceSlider.slider){
                trombonelessParameters.nearDistance = distanceSlider.slider.getMinValue();
                trombonelessParameters.farDistance = distanceSlider.slider.getMaxValue();
#ifdef DBG_MSG
                std::cout << "DistanceSlider min:" <<  trombonelessParameters.nearDistance << "\n";
                std::cout << "DistanceSlider max:" <<  trombonelessParameters.farDistance << "\n";
#endif
            }

            /* Calibrated pressure has changed. */
            else if (slider == &pressureSlider.slider){
                trombonelessParameters.lowPressure = pressureSlider.slider.getMinValue();
                trombonelessParameters.highPressure = pressureSlider.slider.getMaxValue();
#ifdef DBG_MSG
                std::cout << "PressureSlider min:" <<  trombonelessParameters.lowPressure << "\n";
                std::cout << "PressureSlider max:" <<  trombonelessParameters.highPressure << "\n";
#endif
            }

        }

        

    private:
        /* ========== This is where the buttons and sliders should be initialised and attached to the profiler. ========== */     

        /* Shift keying drop down menu and label */  
        juce::ComboBox shiftKeyChoice;
        juce::Label shiftKeySelectLabel;

        /* Sliders and labels for calibrating the range. */
        CalibrationSlider distanceSlider;

        /* Sliders and labels for calibrating the pressure maximum and minimum. */
        CalibrationSlider pressureSlider;

        /* Sliders and labels for setting the gains of each frequency band. */
        verticalMixSlider lowFreqSlider;


        /** @brief Function that can display a message on the window to the user
            @param title Takes in a message of type juce::String to be displayed as the message title
            @param message Takes in a message of type juce::String to be the message displayed
            @retval void
            @note This function uses an alert window.
        */
        void showMessageBox (const juce::String& title, const juce::String& message)
        {
            juce::AlertWindow::showMessageBoxAsync(
                juce::MessageBoxIconType::InfoIcon,
                title,
                message
            );
        }
        
        

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Layout)
};


