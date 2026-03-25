/** @file       Panels.hpp
 *  @author     Ryan McBride
 *
 *  @brief      A header file to contain the panels used within the Tromboneless
 *              user interface window. This will allow for the encapsulation of
 *              different areas as opposed to having large amounts of repeat calls
 *              in the `Layout.hpp` file.
 */

 /* Preventing recurssion. */
 #pragma once

 /* Adding any necessary include files. */
 #include   "Widgets.hpp"

 /* Class definitions. */

 class DropDownMenus :   public juce::Component
 {
    public:
        /* Constructor. */
        DropDownMenus();

        // /* Destructor. */
        ~DropDownMenus() = default;

        /** @brief      Adding a functiont that will allow for the `DropDownMenus`
                        to be resized when the window size is adjusted to fit best.
        */
        void resized() override;

    private:
        /* Shift keying drop down menu and label */
        DropDownMenu shiftKeyChoice;
 };

 class EqualizerPanel   :   public juce::Component,
                            public juce::Button::Listener
 {
    public:
        EqualizerPanel();

        ~EqualizerPanel() = default;

        void resized() override;

        // void DisplayPanel(bool option);

        void buttonClicked(juce::Button* buttonClicked) override;

    private:
        juce::ToggleButton  button;
        Equalizer equalizer;
        juce::Label buttonLabel;

 };