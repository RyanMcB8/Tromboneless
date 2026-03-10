#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include <stdexcept>
#include <string>
#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
// #include shared header


/* ========================================================================
        Defintion of structs and enums used for the App side
   ======================================================================== */
/** @brief Data type which can change the appearance of the slider and button displayed on the window
    @note This code was created by Charles Schiemeyer in his "SimpleEQ" project on his GitHub: matkatmusic 
*/
struct LookAndFeel : juce::LookAndFeel_V4
{
    void drawRotarySlider (juce::Graphics&,
                           int x, int y, int width, int height,
                           float sliderPosProportional,
                           float rotaryStartAngle,
                           float rotaryEndAngle,
                           juce::Slider&) override;
    
    void drawToggleButton (juce::Graphics &g,
                           juce::ToggleButton & toggleButton,
                           bool shouldDrawButtonAsHighlighted,
                           bool shouldDrawButtonAsDown) override;
};

/** @brief Data type which can store the label of a slider and display it as a rotary slider on the window
    @note This code was created by Charles Schiemeyer in his "SimpleEQ" project on his GitHub: matkatmusic 
*/
struct RotarySliderWithLabels : juce::Slider
{
    RotarySliderWithLabels(juce::RangedAudioParameter& rap, const juce::String& unitSuffix) :
    
    juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                 juce::Slider::TextEntryBoxPosition::NoTextBox),
    param(&rap),
    suffix(unitSuffix)
    {
        setLookAndFeel(&lnf);
    }
    
    ~RotarySliderWithLabels()
    {
        setLookAndFeel(nullptr);
    }
    
    struct LabelPos
    {
        float pos;
        juce::String label;
    };
    
    juce::Array<LabelPos> labels;
    
    void paint(juce::Graphics& g) override;
    juce::Rectangle<int> getSliderBounds() const;
    int getTextHeight() const { return 14; }
    juce::String getDisplayString() const;
private:
    LookAndFeel lnf;
    
    juce::RangedAudioParameter* param;
    juce::String suffix;
};


/** @brief Main class that creates the window
 */
class Profiler final : public juce::Component
{
    public:

        // Constructor
        Profiler();

        // Deconstructor
        ~Profiler() override;

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

        /* Creation of an enum which corresponds to different shift keying
           initial positions which may adjusted by the user through the app. */
        typedef enum{
            SKOpt_NoOption,
            SKOpt_MiddleF4,
            SKOpt_BSharp4,
            SKOpt_D5,
            SKOpt_F5,
            SKOpt_ASharp4,
        } ShiftKeyingOptions_t;

        

    private:
        /* ========== This is where the buttons and sliders should be initialised and attached to the profiler. ========== */        
        juce::ComboBox moduleComboBox;

        juce::Label module_label;

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

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Profiler)
};

