#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include <stdexcept>
#include <string>
#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "Widgets.h"
#include "tromboneless_data.h"


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
            
        void sliderValueChanged (juce::Slider* slider) override
        {
            if (slider == &nearDistanceSlider)
                farDistanceSlider.setRange (nearDistanceSlider.getValue() + distanceRange, Layout::maximumDistance, juce::dontSendNotification);
            else if (slider == &farDistanceSlider)
                nearDistanceSlider.setRange (Layout::minimumDistance, farDistanceSlider.getValue() - distanceRange, juce::dontSendNotification);
        }

        

    private:
        /* ========== This is where the buttons and sliders should be initialised and attached to the profiler. ========== */     

        /* Shift keying drop down menu and label */  
        juce::ComboBox shiftKeyChoice;
        juce::Label shiftKeySelectLabel;

        /* Sliders and labels for calibrating the range. */
        juce::Slider nearDistanceSlider;
        juce::Label nearDistanceLabel;
        juce::Slider farDistanceSlider;
        juce::Label farDistanceLabel;

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


