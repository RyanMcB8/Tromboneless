/** @file       Layout.hpp
 *  @author     Ryan McBride
 *  @brief      This file holds the declarations of all functions and variables
 *              needed by the Tromboneless project to define the layout of the
 *              window which is being displayed to the end user.
 */


/*  Preventing recurssive definitons. */
#pragma once

/*  Adding all the necessary header files to be included in the file */
// #include <juce_gui_extra/juce_gui_extra.h>
#include <stdexcept>
#include <string>
#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include "Widgets.hpp"
#include "Panels.hpp"
#include "CoreWrapper.hpp"
#include <stdio.h>

/** @brief Main class that creates the window
 */
class Layout final : public juce::Component
{
    public:

        // Constructor
        Layout(CoreWrapper& coreWrapper);

        // Deconstructor
        ~Layout() = default;

        /** @brief      Function that sets the background colour of the window.
         *  @param  g   The panel which should be modified.
         *  @note       Always sets the background to the user preference for the background.
         *              This function also adds a title to be within the window.
         */  
        void paint (juce::Graphics& g) override;


        /** @brief Function that changes the size of the profiler
         *  @note Resizes the contents of the window including dropdown menus, labels and buttons
         */
        void resized() override;

        

    private:
        /*  Creating the reference to the core*/
        CoreWrapper& coreWrapper_ref;

        /* ========== This is where the buttons and sliders should be initialised and attached to the profiler. ========== */     

        /* Shift keying drop down menu and label */  
        DropDownMenus   dropDownMenus;

        /* Sliders and labels for calibrating the range. */
        Sliders sliders;

        /* Sliders and labels for calibrating the pressure maximum and minimum. */
        Barometer pressureSlider;

        /* Adding an option to calibrate the embouchure. */
        CalibrateEmbouchure calibrateEmbouchure;
        

        /* Creation of the equalizer. */
        EqualizerPanel equalizer;

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


