#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include <stdexcept>
#include <string>
#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "CustomStyles.hpp"
#include "tromboneless_data.hpp"

/* A header file containing custom widgets to appear on the screen to reduce and repetitive definitions within the main Layout.cpp file. */

/** @brief A class which inherits attributes from the juce::Component class
 *  to create a new component containing a 8 possible label locations around the slider.
 */
class SliderWithLabel : public juce::Component{
    public:
    
    
        juce::Slider slider;

        /* Bound parameters. */
        int topLabelBounds = 50;
        int leftLabelBounds = 200;
        int rightLabelBounds = 200;
        int bottomLabelBounds = 50; 

        /* Constructor which sets the style of the slider being used and removes the text entry option. */
        SliderWithLabel(juce::Slider::SliderStyle style){
            slider.setSliderStyle(style);
            slider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
            addAndMakeVisible(slider);
        }


        /* Enumerated list of possible positions for the label to be attached relative to the slider. */
        typedef enum{
            UpperLeft, UpperCentre, UpperRight,
            MiddleLeft,              MiddleRight,
            LowerLeft, LowerCentre, LowerRight,
        } LabelPositions_t;

        /* Enumerated list of possible errors when initialising the labels within this component. */
        typedef enum{
            LabelOK,
            PositonNotAccepted,
        } InitialiseLabelErrors_t;
        
        /** @brief A function which may be called to create or edit a specified label around the slider.
         *  @param position The position index of the label which should be modified.
         *  These are saved as an enum in `Widgets.h`.
         *  @param phrase An array of type juce::String which contains the phrase which should be
         *  displayed where the label is.
         *  @param NotificationEnable The type of notification which should be attached to the label.
         *  This is set to `dontSendNotification` by default but may be modified.
         *  @note This function may only address one label at a time, but may be called upon
         *  for each label needed.
         *  There may only be one label in each position.
         */
        InitialiseLabelErrors_t CreateLabel(LabelPositions_t position,
            juce::String phrase,
            juce::NotificationType NotificationEnable = juce::dontSendNotification);

        /** @brief A function which is automatically called whenever the window size is modified.
         *  This automatically adjusts the bounds of each of the labels as well as the slider
         *  to ensure the size is correct with respect to the screen size for the component.
         *  @note The bounds are preset to be 20 for the top and bottom labels and 40 for the
         *  left and right labels. These values may be overwritten by writting to the
         *  `___LabelBounds` where ___ may be `top`, `right`, `left` or `bottom`.
         */
        void resized() override;

    private:
        
        /** @brief A simple function which writes to the label referenced and sets its
         *  notificiation type.
         *  @param Label A pointer to the label which should be modified.
         *  @param phrase An array of data of type juce::String which contains what the label should say.
         *  @param NotificationEnable The notification type which is being attached to the label.
         *  @note This function was made to prevent large amounts of repetitive code within the 
         *  `CreateLabel` function and does not contain any extra code. 
         */
        void labelSetup(juce::Label* Label, juce::String phrase,
                        juce::NotificationType NotificationEnable);
    private:
        /* Creation of labels that may be used with the slider. */
        juce::Label UpperLeftLabel;
        juce::Label UpperCentreLabel;
        juce::Label UpperRightLabel;
        juce::Label MiddleLeftLabel;
        juce::Label MiddleRightLabel;
        juce::Label LowerLeftLabel;
        juce::Label LowerCentreLabel;
        juce::Label LowerRightLabel;
};

class RotarySliderWithLabel :   public SliderWithLabel,
                                public juce::Slider::Listener
{
public:
    RotarySliderWithLabel();

    ~RotarySliderWithLabel() = default;
    
};


/** @brief A class that adds more functionality to the slider class from Juce. */
class CalibrationSlider :   public SliderWithLabel,
                            public juce::Slider::Listener
{
public:
    /** @brief Contructor for the `CalibrationSlider` class which requires the
     *  type of slider being used to be initiated which is set to `TwoValueHorizontal`
     *  by default.
     */ 
    CalibrationSlider();

    ~CalibrationSlider() = default;

    /** @brief A function which sets the minimum difference between the upper and lower fingers of the slider.
     *  @param difference The value of the difference between the fingers.
     */
    void setMinDifference(double difference);


    /** @brief A function that is called upon when either of the fingers are moved.
     *  @note This function automatically moves the fingers to ensure that they stay at least the minimum range apart from each other.
     */
    void sliderValueChanged(juce::Slider* sliderChanged) override;

    private:
        double minDistance = 1;
};

/** @brief A class that adds more functionality to the slider class from Juce. */
class CalibrationRotarySlider : public juce::Component
{
public:
    CalibrationRotarySlider();

    // Normalised values (0.0 → 1.0)
    float minValue = 0.25f;
    float maxValue = 0.75f;

    /* Setting the limits of the slider. */
    float sliderMinLimit = 0.1;
    float sliderMaxLimit = 2;

    std::function<void(float, float)> onValueChange;

    void paint (juce::Graphics& g) override;

    void mouseDown (const juce::MouseEvent& e) override;

    void mouseDrag (const juce::MouseEvent& e) override;

private:
    typedef enum
    {
        min,
        max,
    } handle_t;

    float startAngle = -M_PI*0.75f;
    float endAngle = -startAngle;

    handle_t activeHandle = handle_t::min;

    float positionToValue (float x, float y);

    handle_t getClosestHandle (float x, float y);

    void drawHandle (juce::Graphics& g, float centreX, float centreY,
                     float radius, float angle, juce::Colour colour);
};

/** @brief A class that adds more functionality to the slider class from Juce. */
class verticalMixSlider : public SliderWithLabel,
                          public juce::Slider::Listener
{
public:
    verticalMixSlider();

    ~verticalMixSlider() = default;

    void sliderValueChanged(juce::Slider* sliderChanged) override;
    
};


/** @brief A class which inherits its sliders from `verticalMixSlider` made to act as an equalizer.
 *  @param nSliders The number of sliders which should be added to the object. 10 by default.
 */
class Equalizer : public verticalMixSlider
{
    public:
        char nSliders;
        juce::OwnedArray <verticalMixSlider> eqSliders;
        
        /* Constructor. */
        Equalizer(char numberOfSliders = 10);

        /*Destructor. */
        ~Equalizer();
        
        /** @brief A function to set the size of the sliders with respect to the equalizer.
         * 
         */
        void resized() override;

        void sliderValueChanged(juce::Slider* sliderChanged) override;
        
    private:
        juce::Array <juce::String> frequencyLabels; 
        float prevLabel;
        float newLabel;
        VerticalSliderLookAndFeel customLook;
        
};


 /** @brief     A class that creates a drop down menu or 'comboBox' with a label
  *             attached to it. This takes up 95% of the available local area.
  *             The label is set to have a maxmimum width of 200, but scales as
  *             the window size is reduced.
  */
 class DropDownMenu :   public juce::Component
 {
     public:
        juce::ComboBox dropDownChoice;
     /** @brief                  Constructor for the DropDownMenu class that creates an
         *                          instance of the class. This makes a juce component that
         *                          has a combobox or drop-down menu within it with a label
         *                          attached to its left by default.
         *  @param  labelPhrase     This parameter should be of type juce::String and contain
         *                          the phrase which should be displayed by the label. By
         *                          default this is set to "ComboBox" so it may be viewed on
         *                          creation.
         */
        DropDownMenu(juce::String labelPhrase = "ComboBox");

        /* Destructor. */
        ~DropDownMenu() = default;

        /** @brief      Adding a functiont that will allow for the `DropDownMenu`
                        to be resized when the window size is adjusted to fit best.
        */
        void resized() override;

        /** @brief                  A function which allows for the addition of more items
         *                          within the drop down menu.
         *  @param  itemName        This parameter of type `juce::String` should contain the
         *                          phrase which will be displayed to the end user on the
         *                          display to select this option.
         *  @param  itemValue       This is the unique number assigned to each of the items.
         *                          It must be more than 0 for it to be displayed. Each item
         *                          must have a different value for them to be distinguishable
         *                          from each other.
         */
        void AddItem(juce::String itemName, int itemValue);

        /** @brief                  This function sets what will happen when the option chosen
         *                          has been changed by the user.
         *  @param  updateVaraible  This should be a pointer to the variable which is being 
         *                          set to the item index which has been chosen.
         */
        template <typename T>
        void OnChange(T* updateVariable){
            dropDownChoice.onChange = [updateVariable, this] {*updateVariable = static_cast<T> (dropDownChoice.getSelectedId());};
        }

        /** @brief                  This function allows for the editting of the main label
         *                          displayed beside the drop down menu.
         *  @param  text            This parameter should contain the text to which the label
         *                          should be updated to display.
         *  @param  notification    This is the type of notification which should be attached
         *                          to the drop down menu. By default this value is set to
         *                          `juce::dontSendNotification`.
         */
        void ChangeLabelText(juce::String text, juce::NotificationType notification = juce::dontSendNotification);

    private:
        /* Shift keying drop down menu and label */  
        juce::Label dropDownLabel;
 };


 class CalibrationOnClick   :   public juce::Component,
                                public juce::Button::Listener
 {
    public:
        
        CalibrationOnClick();

        ~CalibrationOnClick() = default;

        void resized() override;
        juce::TextButton button;
        
    private:
 };

class CalibrateEmbachure    :   public CalibrationOnClick
{
    public:
        CalibrateEmbachure();

        ~CalibrateEmbachure() = default;

        void resized() override;

        void buttonClicked(juce::Button* thisButtonClicked) override;

    private:
        DropDownMenu embachureChoice;
        
        juce::TooltipWindow tooltipWindow;

};

