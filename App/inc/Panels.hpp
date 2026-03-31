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

 class Panels   :   public juce::Component
 {
    public:
        Panels() = default;
        ~Panels() = default;

        void paint(juce::Graphics& g) override;
    private:
        juce::Colour edgeColour         = juce::Colour((unsigned int) (0x43080810));
 };

 class DropDownMenus :   public Panels
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
        CalibrateEmbouchure calibrateEmbouchure;
 };

 class Sliders : public Panels,
                public juce::Slider::Listener
 {
    public:
        CalibrationSlider distanceSlider;
        CalibrationSliderLookAndFeel LandF;

        Sliders();
        ~Sliders();

        void resized() override;

        void sliderValueChanged(juce::Slider* sliderChanged) override;

    private:
        float minimumDistance = 5;
        float maximumDistance = 60;
        float stepDistance = 0.1;   
        float distanceRange = 5;
 };

 class EqualizerPanel   :   public Panels,
                            public juce::Button::Listener
 {
    public:
        EqualizerPanel();

        ~EqualizerPanel();

        void resized() override;

        // void DisplayPanel(bool option);

        void buttonClicked(juce::Button* buttonClicked) override;

    private:
        juce::ToggleButton  button;
        Equalizer equalizer;
        juce::Label buttonLabel;

 };