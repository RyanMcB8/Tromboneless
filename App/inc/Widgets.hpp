/** @file           Widgets.hpp
 *  @author         Ryan McBride
 *  @brief          A file which sets the declarations of any custom widgets
 *                  which have been built off of the juce framework. A large
 *                  portion of these widgets have been created specifically
 *                  for the Tromboneless project.
 */


#pragma once

// #include <juce_gui_extra/juce_gui_extra.h>
#include <stdexcept>
#include <string>
#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "CustomStyles.hpp"
#include <tromboneless_data.hpp>

/* A header file containing custom widgets to appear on the screen to reduce and repetitive definitions within the main Layout.cpp file. */

/** @brief A class which inherits attributes from the juce::Component class
 *  to create a new component containing a 8 possible label locations around the slider.
 */
class SliderWithLabel : public juce::Component{
    public:
    
        /** @brief  The slider which is having the labels added to it. */
        juce::Slider slider;

        /** @brief Constructor which sets the style of the slider being used and removes the text entry option. */
        SliderWithLabel(juce::Slider::SliderStyle style){
            slider.setSliderStyle(style);
            slider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
            addAndMakeVisible(slider);
        }

        /** @brief Enumerated list of possible positions for the label to be attached relative to the slider. */
        typedef enum{
            UpperLeft, UpperCentre, UpperRight,
            MiddleLeft,              MiddleRight,
            LowerLeft, LowerCentre, LowerRight,
        } LabelPositions_t;

        /** @brief Enumerated list of possible errors when initialising the labels within this component. */
        typedef enum{
            LabelOK,
            PositonNotAccepted,
        } InitialiseLabelErrors_t;
        
        /** @brief                      A function which may be called to create or edit a
         *                              specified label around the slider.
         *  @param position             The position index of the label which should be modified.
         *                              These are saved as an enum in `Widgets.h`.
         *  @param phrase               An array of type juce::String which contains the phrase
         *                              which should be displayed where the label is.
         *  @param NotificationEnable   The type of notification which should be attached to the
         *                              label. This is set to `dontSendNotification` by default
         *                              but may be modified.
         *  @note                       This function may only address one label at a time, but
         *                              may be called upon for each label needed. There may only
         *                              be one label in each position.
         */
        InitialiseLabelErrors_t CreateLabel(LabelPositions_t position,
            juce::String phrase,
            juce::NotificationType NotificationEnable = juce::dontSendNotification);

        /** @brief                      A function which is automatically called whenever the
         *                              window size is modified. This automatically adjusts
         *                              the bounds of each of the labels as well as the
         *                              slider to ensure the size is correct with respect to
         *                              the screen size for the component.
         *  @note                       The bounds are preset to be 20 for the top and bottom
         *                              labels and 40 for the left and right labels. These
         *                              values may be overwritten by writting to the
         *                              `___LabelBounds` where ___ may be `top`, `right`,
         *                              `left` or `bottom`.
         */
        void resized() override;

        /** @brief                      A function to set the bounds for which the top labels
         *                              may be situated in.
         *  @param  bound               The integer representation of the bounds which are to
         *                              be set.
         */
        void setTopLabelBounds(int bound);

        /** @brief                      A function to get the bounds for which the top labels
         *                              are situated in.
         *  @retval                     The integer representation of the top label bounds.
         */
        int getTopLabelBounds(void);

        /** @brief                      A function to set the bounds for which the labels on
         *                              the left may be situated in.
         *  @param  bound               The integer representation of the bounds which are to
         *                              be set.
         */
        void setLeftLabelBounds(int bound);

        /** @brief                      A function to get the bounds for which the labels on
         *                              the left are situated in.
         *  @retval                     The integer representation of the left label bounds.
         */
        int getLeftLabelBounds(void);

        /** @brief                      A function to set the bounds for which the labels on
         *                              the right may be situated in.
         *  @param  bound               The integer representation of the bounds which are to
         *                              be set.
         */
        void setRightLabelBounds(int bound);

        /** @brief                      A function to get the bounds for which the labels on
         *                              the right are situated in.
         *  @retval                     The integer representation of the right label bounds.
         */
        int getRightLabelBounds(void);

        /** @brief                      A function to set the bounds for which the bottom labels
         *                              may be situated in.
         *  @param  bound               The integer representation of the bounds which are to
         *                              be set.
         */
        void setBottomLabelBounds(int bound);

        /** @brief                      A function to get the bounds for which the bottom labels
         *                              are situated in.
         *  @retval                     The integer representation of the bottom label bounds.
         */
        int getBottomLabelBounds(void);

    private:
        
        /** @brief                      A simple function which writes to the label referenced
         *                              and sets its notificiation type.
         *  @param Label                A pointer to the label which should be modified.
         *  @param phrase               An array of data of type juce::String which contains
         *                              what the label should say.
         *  @param NotificationEnable   The notification type which is being attached to the
         *                              label.
         *  @note                       This function was made to prevent large amounts of
         *                              repetitive code within the `CreateLabel` function and
         *                              does not contain any extra code. 
         */
        void labelSetup(juce::Label* Label, juce::String phrase,
                        juce::NotificationType NotificationEnable);
    
        /* Top label bound parameter (maximum height of the above the slider). */
        int topLabelBounds = 50;
        /* Left label bound parameter (maximum width of the label to the left of the slider). */
        int leftLabelBounds = 200;
        /* Right label bound parameter (maximum width of the label to the right of the slider). */
        int rightLabelBounds = 200;
        /* Bottom label bound parameter (maximum height of the label under the slider). */
        int bottomLabelBounds = 50; 

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


/** @brief A class that adds more functionality to the slider class from Juce. */
class CalibrationSlider :   public SliderWithLabel,
                            public juce::Slider::Listener
{
public:
    /** @brief                      The contructor function for the `CalibrationSlider` class
     *                              which requires the type of slider being used to be
     *                              initiated. This is set to `TwoValueHorizontal` by default.
     */ 
    CalibrationSlider();

    /** @brief                      The destructor function for the `CalibrationSlider` class
     *                              which removes the listener from the slider.
     */
    ~CalibrationSlider();

    /** @brief                      A function which sets the minimum difference between the upper
     *                              and lower fingers of the slider.
     *  @param difference           The value of the minimum distance between the slider knobs.
     */
    void setMinDifference(float difference);

    /** @brief                      A function which returns the current minimum distance that the
     *                              slider fingers may be apart.
     *  @retval                     The double precision representation of the minimum distance
     *                              between the slider fingers.
     */
    float getMinDifference(void);


    /** @brief                      A function that is called upon when either of the fingers are
     *                              moved.
     *  @note                       This function automatically moves the fingers to ensure that
     *                              they stay at least the minimum range apart from each other.
     */
    void sliderValueChanged(juce::Slider* sliderChanged) override;

    private:
        /** A variable which sets the minimum distance between the maxmimum and minimum slider knobs. */
        float minDistance = 1.0;
};

/** @brief A class that adds more functionality to the slider class from Juce. */
class DualRotarySlider : public juce::Component,
                         public juce::Slider::Listener
{
public:

    /** The slider which will represent the high end of the calibration. */
    juce::Slider    maxSlider;
    /** The slider which will represent the low end of the calibration. */
    juce::Slider    minSlider;
    /** The label of the component. */
    juce::Label     rotaryLabel;
    
    /** @brief                      The constructor function for the `DualRotarySlider` class
     *                              which sets the range of the sliders, minimum distance and
     *                              adds listeners to them both so that when the sliders are
     *                              moved, the corresponding variables may be changed.
     */
    DualRotarySlider();

    /** @brief                      The destructor function for the `DualRotarySlider` class
     *                              which removes the listeners from the sliders.
     */
    ~DualRotarySlider();

    /** @brief                      A function that is called upon when either of the fingers
     *                              are moved.
     *  @note                       This function automatically moves the fingers to ensure that
     *                              they stay at least the minimum range apart from each other.
     */
    void sliderValueChanged(juce::Slider* sliderChanged) override;

    /** @brief                      A callback function for when the window size has changed.
     *                              This allows for the labels and sliders to be resized to
     *                              be the best relative size. This can either be set to a 
     *                              predetermined size like 100px x 200px, but it is currrently
     *                              set to be relative to the size of the screen.
     */
    void resized() override;

    /** @brief                      A function to set the minimum difference between the sliders 2
     *                              fingers.
     *  @param  difference          The difference in terms of the chosen maximum and minimum
     *                              values as a floating point value.
     */
    void setMinDifference(float difference);

    /** @brief                      A function to get the current minimum difference between the
     *                              2 slider fingers.
     *  @retval                     Returns the floating point representation of the minimum
     *                              difference between the 2 sliders in terms of the chosen maximum
     *                              and minimum values.
     */
    float getMinDifference(void);

    /** @brief                      A function to set the relative radius of the minimum
     *                              finger slider. It should always be less than or equal to
     *                              the maximum slider radius and be between 0 and 1.
     *  @param  radius              The relative radius that the minimum slider should have.
     */
    void setMinSliderRadius(float radius);

    /** @brief                      A function to get the relative radius of the minimum
     *                              finger slider for this object.
     *  @retval                     Returns the floating point representation of the
     *                              relative radius.
     */
    float getMinSliderRadius(void);

    /** @brief                      A function to set the relative radius of the maximum
     *                              finger slider. It should always be more than or equal to
     *                              the minimum slider radius and be between 0 and 1.
     *  @param  radius              The relative radius that the maximum slider should have.
     */
    void setMaxSliderRadius(float radius);

    /** @brief                      A function to get the relative radius of the maximum
     *                              finger slider for this object.
     *  @retval                     Returns the floating point representation of the
     *                              relative radius.
     */
    float getMaxSliderRadius(void);

    /** @brief                      A function to set the maximum height of the label attached
     *                              to the rotary slider.
     *  @param  height              The height of the label in terms of the resolution as a
     *                              floating point value.
     */
    void setLabelHeight(float height);

    /** @brief                      A function to get the current height of the label which is
     *                              attached to the rotary slider.
     *  @retval                     The height of the label as a floating point value.
     */
    float getLabelHeight(void);

    /** @brief                      A function to set the minimum limit of the rotary slider.
     *                              This may be any floating point value that is less than the
     *                              difference between the maximum value and the minimum 
     *                              difference between the slider fingers.
     *  @param  limit               The floating point representation of the limit. 
     */
    void setMinLimit(float limit);

    /** @brief                      A function to get the current minium limit of the rotary
     *                              slider.
     *  @retval                     The floating point representation of the minimum limit.
     */
    float getMinLimit(void);

    /** @brief                      A function to set the maximum limit of the rotary slider.
     *                              This may be any floating point value that is more than the
     *                              sum of the minium value and the minimum difference between
     *                              the slider fingers.
     *  @param  limit               The floating point representation of the limit. 
     */
    void setMaxLimit(float limit);

    /** @brief                      A function to get the current maximum limit of the rotary
     *                              slider.
     *  @retval                     The floating point representation of the minimum limit.
     */
    float getMaxLimit(void);

    /** @brief                      A function to set the minimum step the slider may be moved
     *                              by the user. This value is in the same scale as the maxLimit
     *                              and minLimit.
     *  @param  step                The minimum step as a floating point value.
     */
    void setInterval(float step);

    /** @brief                      A function to get the current minimum step which the
     *                              slider may be adjusted by the user. This value is in
     *                              the same scale as the maxLimit and minLimit.
     *  @retval                     The floating point representation of the current
     *                              minimum step.
     */
    float getInterval(void);

    /** @brief                      A function to allow for the minimum angle (furtherest
     *                              anticlockwise) that the slider may be rotated to be set.
     *                              This is measured in radians.
     *  @param  angle               The angle in radians as a floating point value.
     */
    void setMinAngle(float angle);

    /** @brief                      A function to return the current minimum angle (furtherest
     *                              anticlockwise) that the slider may be rotated. This
     *                              is measured in radians.
     *  @retval                     The angle in radians as a floating point value.
     */
    float getMinAngle(void);

    /** @brief                      A function to allow for the maximum angle (furtherest
     *                              clockwise) that the slider may be rotated to be set.
     *                              This is measured in radians.
     *  @param  angle               The angle in radians as a floating point value.
     */
    void setMaxAngle(float angle);

    /** @brief                      A function to return the current maximum angle (furtherest
     *                              clockwise) that the slider may be rotated. This
     *                              is measured in radians.
     *  @retval                     The angle in radians as a floating point value.
     */
    float getMaxAngle(void);
    
private:

    /* Setting the relative radius of the minimum slider. */
    float minSliderRadius =   0.6;

    /* Setting the relative radius of the maximum slider. */
    float maxSliderRadius =   0.9;

    /*  Setting the minimum limit of the slider. */
    float minLimit = 0.1;
    /*  Setting the maximum limit of the slider. */
    float maxLimit = 2.0;

    /*  Setting the minimum step to which the slider may be moved. */
    float interval = 0.1;

    /* This is the minimum difference between the min and max sliders with respect ot the non normalised values. */
    float minDifference = 0.2;

    /* Setting the relative height of the label. */
    float labelHeight;

    /*  These values should be moved to be private and set/get functions added. */
    /*  Setting the minimum angle which the slider can reach. */
    float minAngle      =   M_PI + 0.5;
    /*  Setting the maximum angle which the slider can reach. */
    float maxAngle      =   3*M_PI - 0.5;
    
    /* Finding the minimum difference in angle for the finger differences by
       normalising the minDifference with respect to the max and min limits
       and then mulitplying it by the difference betweent the max and min
       angles. */
    float minAngleDifference = (maxAngle - minAngle)*(minDifference/(maxLimit - minLimit));

    /** @brief                      A function to update the minimum angle difference between
     *                              the 2 sliders when one of the parameters has changed
     *                              within its calculation.
     */
    void updateMinAngleDifference(void);
};

/** @brief  A class which can allow for the look and feel of a set of rotary sliders be modified
 *          to look like a barometer (pressure gauge).
 *  @note   This is used in the Tromboneless project to calibrate the maximum and minimum gains on
 *          the mouthpiece.
 */
class Barometer :   public DualRotarySlider
{
    public:
        /** @brief                  Constructor for the Barometer class which
         *                          sets the look and feel of the sliders,
         *                          attaches a listener to them both and sets
         *                          the labels text to be the corresponding
         *                          text.
         */
        Barometer();

        /** @brief                  Decontructor for the Barometer class which
         *                          removes the listeners from the sliders as
         *                          well as resetting their look and feel.
         */
        ~Barometer();

        /** @brief                  A callback function which is automatically
         *                          called upon when either of the sliders
         *                          knobs are adjusted.
         *  @param  sliderChanged   A pointer to the slider which has been
         *                          changed by the user.
         */
        void sliderValueChanged(juce::Slider* sliderChanged) override;

        /** @brief                  A function which is used to change the
         *                          background of the component to look
         *                          like it is a barometer/ pressure gauge.
         *  @param  g               A pointer to the juce::Graphics instance
         *                          which is being drawn to. This is likely
         *                          the parent component or the main window
         *                          of the system.
         *  @note                   This function may be altered to change
         *                          the look of the device but it currently
         *                          adds a white circle as a background,
         *                          golden outer circle for the edge of it,
         *                          2 arcs around it corresponding to bounds,
         *                          with lines connecting them representing
         *                          the possible settings.
         */
        void paint (juce::Graphics& g) override;

        /** @brief                  A function to set the colour of the background of the slider.
         *  @param  colour          The colour to which the background should be set to.
         */
        void setBackgroundColour(juce::Colour colour);

        /** @brief                  A function to get the current colour of the background of the
         *                          slider.
         *  @retval                 Returns the colour as a juce::Colour type.
         */
        juce::Colour getBackgroundColour(void);

        /** @brief                  A function to set the colour of the text around the slider.
         *  @param  colour          The colour to which the text should be set to.
         */
        void setTextColour(juce::Colour colour);

        /** @brief                  A function to get the current colour of the text around the
         *                          slider.
         *  @retval                 Returns the colour as a juce::Colour type.
         */
        juce::Colour getTextColour(void);

        /** @brief                  A function to set the colour of the boarder of the slider.
         *  @param  colour          The colour to which the boarder should be set to.
         */
        void setBoarderColour(juce::Colour colour);

        /** @brief                  A function to get the current colour of the boarder of the
         *                          slider.
         *  @retval                 Returns the colour as a juce::Colour type.
         */
        juce::Colour getBoarderColour(void);

        /** @brief                  A function to set the colour of the edge around the slider.
         *  @param  colour          The colour to which the edge should be set to.
         */
        void setEdgeColour(juce::Colour colour);

        /** @brief                  A function to get the current colour of the edge around the
         *                          slider.
         *  @retval                 Returns the colour as a juce::Colour type.
         */
        juce::Colour getEdgeColour(void);

        /** @brief                      A function to set the relative radius of the outer rotary slider.
         *  @param  radius              The floating point representation of the relative radius with
         *                              respect to the widgets (barometer instance) bounds.
         */
        void setOuterRadius(float radius);

        /** @brief                      A function to get the relative radius of the outer rotary slider.
         *  @retval                     The floating point representation of the relative radius with
         *                              respect to the widgets (barometer instance) bounds.
         */
        float getOuterRadius(void);

        /** @brief                      A function to set the relative radius of the inner rotary slider.
         *  @param  radius              The floating point representation of the relative radius with
         *                              respect to the widgets (barometer instance) bounds.
         */
        void setInnerRadius(float radius);

        /** @brief                      A function to get the relative radius of the inner rotary slider.
         *  @retval                     The floating point representation of the relative radius with
         *                              respect to the widgets (barometer instance) bounds.
         */
        float getInnerRadius(void);

        /** @brief                      A function to set the relative radius of the label.
         *  @param  radius              The floating point representation of the relative radius with
         *                              respect to the widgets (barometer instance) bounds.
         */
        void setLabelRadius(float radius);

        /** @brief                      A function to get the relative radius of the label.
         *  @retval                     The floating point representation of the relative radius with
         *                              respect to the widgets (barometer instance) bounds.
         */
        float getLabelRadius(void);

    private:
        /*  Creation of an instance of the NeedleLookAndFeel class to be
            added to the slider knobs to have the correct aesthetic. */
        NeedleLookAndFeel arrow;

        /* Defining the colours used within the barometer. */
        juce::Colour backgroundColour   = juce::Colours::white;
        juce::Colour textColour         = juce::Colours::black;
        juce::Colour boarderColour      = juce::Colours::gold;
        juce::Colour edgeColour         = juce::Colour((unsigned int) (0x43080810));
        
        /* Dimensions */
        /* Setting the size of the outer arc on the barometer. */
        float relativeOuterRadius = 0.4;

        /* Setting the size of the inner arc on the barometer.*/
        float relativeInnerRadius = 0.35;

        /* Setting the distance of the gain labels relative to the centre point. */
        float relativeLabelRadius = 0.3;
};

/** @brief A class that adds more functionality to the slider class from Juce. */
class verticalMixSlider : public SliderWithLabel,
                          public juce::Slider::Listener
{
public:
    /** @brief                      Constructor function for the vericalMixSlider
     *                              which creates an instance of the class and
     *                              sets the parameters to match it best such as
     *                              setting the range to be between -9 dB and 9 dB
     *                              with a step size of 3 dB as well as label size.
     */
    verticalMixSlider();

    /** @brief                      Destructor function for the verticalMixSlider
     *                              class which removes the listener from the
     *                              slider before killing the object.
     */
    ~verticalMixSlider();

    /** @brief                      A callback function for when the slider used
     *                              has been adjusted and its value may have been
     *                              changed.
     *  @param  sliderChanged       This is a pointer to the slider which has
     *                              been changed.
     *  @note                       As this class only has one slider that is in
     *                              it, there is no need to use it but the JUCE
     *                              function passes it anyway.
     */
    void sliderValueChanged(juce::Slider* sliderChanged) override;
    
};


/** @brief A class which inherits its sliders from `verticalMixSlider` made to act as an equalizer.
 *  @param nSliders The number of sliders which should be added to the object. 10 by default.
 */
class Equalizer : public verticalMixSlider
{
    public:

        /** @brief Creating an array of sliders for the equalizer. */
        juce::OwnedArray <verticalMixSlider> eqSliders;
        
        /** @brief                  Contructor for the Equalizer class which creates a set
         *                          of n vertical sliders in the style of an equalizer.
         *  @param  numberOfSliders The number of sliders which are present within the
         *                          equalizer. This is set to 10 by default by may be altered.
         *  @note                   Caution: If the nuimberOfSliders is set to be higher than
         *                          the number of elements in the `gains` variable in
         *                          `SynthesiserParameters_t`, then the extra sliders will not
         *                          affect any variable.
         */
        Equalizer(char numberOfSliders = 10);

        /** @brief                  Destructor for the Equalizer which removes the slider
         *                          listeners from each of the sliders as well as resetting
         *                          the look and feel of each of them.
         */
        ~Equalizer();
        
        /** @brief                  A function to set the size of the sliders with respect
         *                          to the equalizer size. Whenever the parent component
         *                          changes size, this function is called upon.
         * 
         */
        void resized() override;

        /** @brief                  A callback function for when one of the sliders within
         *                          the equalizer has been detected to have changed in value.
         *                          This is where the corresponding variables the sliders
         *                          affect may be changed.
         *  @param  sliderChanged   A pointer to the slider which has been changed.
         *  @note                   As the equalizer has multiple sliders within it, it is
         *                          important to check which of the sliders has been changed
         *                          before altering any values. This function uses predefined
         *                          variables which are affected by each of the sliders. If
         *                          you wish to change this, you must change it within the
         *                          function defintion as there are no functions to call to
         *                          change the variable.    
         */
        void sliderValueChanged(juce::Slider* sliderChanged) override;
        
    private:
        /*  Creation of an array which stores the different labels which will be set
            as the frequency ranges of the sliders. */
        juce::Array <juce::String> frequencyLabels; 
        
        /*  Stores the previous label which is being written to the bottom of the
            slider. i.e. the minimum frequency in the frequency band which is being
            adjusted by the slider. */
        float prevLabel;

        /*  Stores the current label which is being written to the top of the slider.
            i.e the maximum frequency in the frequency band which is being adjusted
            by the slider. */
        float newLabel;

        /*  Creating an instance of the VerticalSliderLookAndFeel class to apply a new
            look and feel to the sliders within this class to match that of a standard
            equalizer. */
        VerticalSliderLookAndFeel customLook;

        /* variable to store the number of sliders which are being used. */
        char nSliders;
        
};


 /** @brief     A class that creates a drop down menu or 'comboBox' with a label
  *             attached to it. This takes up 95% of the available local area.
  *             The label is set to have a maxmimum width of 200, but scales as
  *             the window size is reduced.
  */
 class DropDownMenu :   public juce::Component
 {
     public:

        int getSelectedId() const{
            return dropDownChoice.getSelectedId();
        }

        template <typename Fn>
        void SetOnChange(Fn&& callback){
            dropDownChoice.onChange = std::forward<Fn>(callback);
        }
        
        /** @brief                  Constructor for the DropDownMenu class that creates an
         *                          instance of the class. This makes a juce component that
         *                          has a combobox or drop-down menu within it with a label
         *                          attached to its left by default.
         *  @param  labelPhrase     This parameter should be of type juce::String and contain
         *                          the phrase which should be displayed by the label. By
         *                          default this is set to "ComboBox" so it may be viewed upon
         *                          creation.
         */
        DropDownMenu(juce::String labelPhrase = "ComboBox");

        /** @brief                  Destructor for the DropDownMenu which is set to default as
         *                          there are no components within this class that need to be
         *                          deinitialised before the deconstruction of the object.
         */
        ~DropDownMenu() = default;

        /** @brief                  Adding a function that will allow for the `DropDownMenu`
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

        /** @brief                  This function sets what will happen when the option wihtin
         *                          the drop down menu has been changed by the user.
         *  @param  updateVariable  This should be a pointer to the variable which is being 
         *                          set to the item index which has been chosen.
         *  @note                   This function uses a lambda function to update the specified
         *                          variable to ensure that the specified variable may be changed
         *                          for different instances of the class.
         */
        template <typename T> void OnChange(T* updateVariable){
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

        /** @brief                  A function which returns the current text which the label is displaying.
         *  @retval                 Returns the text being distplayed as a type `juce::String`.
         */
        juce::String getLabelText(void);

    private:
        /* Adding the label which will be next to the drop down menu. */  
        juce::Label dropDownLabel;

        /* Adding the drop down menu to the class. */
        juce::ComboBox dropDownChoice;
 };

/** @brief      A class which allows for the additon of a button to the screen
 *              without inheriting the button directly. This allows for other
 *              widgets to be added to it later without compatibility issues.
 */
 class CalibrationOnClick   :   public juce::Component,
                                public juce::Button::Listener
 {
    public:
        
        /** @brief              A constructor for the CalibrationOnClick class
         *                      which inherits characteristics from juce::
         *                      Component and Button::Listener. It is used to
         *                      receive a calibration message when clicked.
         */
        CalibrationOnClick();

        /** @brief              Destructor for the CalibrationOnClick class
         *                      which removes the listener from the attached
         *                      button.
         */
        ~CalibrationOnClick();

        /** @brief              A function which is called upon whenever the 
         *                      user interface window has changed in size. This
         *                      can change the size of the component so that it
         *                      can either be kept at a constant size of scaled
         *                      to fit a specified area of the screen.
         * @note                Currently, this adds a small margin around the
         *                      local area and fills the rest of the area with 
         *                      the attached button.
         */
        void resized() override;

        /**  Creating the TextButton instance for the CalibrationOnClick class
            to have a button within it. */
        juce::TextButton button;
        
 };

 /** @brief     A class which is inherited from the CalibrationOnClick class
  *             that adds a dropdown menu next to the button for the user to
  *             select which option they wish to calibrate.
  *  @note      This class was specifically designed for the embachure selction
  *             as no other widget needed to have a calibration button, hence
  *             the name of the class and lack of an intermediate step from
  *             CalibrationOnClick.
  */
class CalibrateEmbouchure    :   public CalibrationOnClick
{
    public:
        /** @brief          Constructor function for the CalibrateEmbouchure
         *                  class which sets the specific parameters of the
         *                  label, dropdown menu and button which are being
         *                  used.
         */
        CalibrateEmbouchure();

        /** @brief          Destructor function for the CalibrateEmbouchure
         *                  class which is currently set to default as there
         *                  are mo specific methods or parameters which need
         *                  to be dealt with before destructing the object.
        */
        ~CalibrateEmbouchure() = default;

        /** @brief          A function which is automatically called upon when
         *                  the user window is adjusted in size. This function
         *                  will allow for the change in relative width and
         *                  height of the components dependent upon screen size.
         */
        void resized() override;

        /** @brief          Callback function which is automatically triggered
         *                  when the button within this class is clicked by the
         *                  user. As this class only has one instance of a
         *                  button, there is no need to check which button has 
         *                  been pressed. However, a pointer to the button is
         *                  still passed as default from the JUCE framework.
         *  @note           Currently this function updates the
         *                  trombonelessParameters struct instance to change
         *                  the calibrate embouchure flag to be on. This will
         *                  allow the Core to read the ADC and set the 
         *                  corresponding value. 
        */
        void buttonClicked(juce::Button* thisButtonClicked) override;

    private:
        /*  Adding a dropdown menu next to the calibration button to select 
            the embachure option which is being calibrated. */
        DropDownMenu embouchureChoice;
        
        /*  Adding a tootltipWindow to the class which may be displayed when
            the user runs their cursor over the button for at least 300ms. */
        juce::TooltipWindow tooltipWindow;

};

