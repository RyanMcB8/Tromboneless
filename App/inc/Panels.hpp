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

 /**    @brief      A class which is inherited from Juce:Component reponsible
  *                 for setting the general style of a panel within the main
  *                 window.
  *     @note       A panel should represent at least one component such as an
  *                 image, label, slider or another widget type. Similar types
  *                 should be grouped together such as all calibration settings.
  */
 class Panels   :   public juce::Component
 {
    public:
        /** @brief                  The constructor function for the `Panels` class
         *                          which currently provides no other function than
         *                          creating an instance of the `Panels` class. I.e.
         *                          no initialisation functions or settings.
         */
        Panels() = default;

        /** @brief                  The destructor function for the `Panels` class
         *                          which currently does nothing extra as there is
         *                          no initialisation within the class.
         */
        ~Panels() = default;

        /** @brief                  This function draws onto the parent function the
         *                          default shape and colour of the panels designed.
         *  @param  g               The point to the `juce::Graphics` instance which
         *                          is being modified.
         *  @note                   The current design uses a rounded rectangle with 
         *                          a dark colour and a slight transparency to have
         *                          the panel have a slight contrast to the 
         *                          background.
         */
        void paint(juce::Graphics& g) override;
    private:
        /*  Definition of the colour which the panel should be. Currently a dark 
            grey colour with a slight transparency.*/
        juce::Colour edgeColour         = juce::Colour((unsigned int) (0x43080810));
 };

 /**    @brief      A class which is created to display the drop down menus required
  *                 by the Tromboneless UI to choose the chosen option for 
  *                 calibration settings.
  */
 class DropDownMenus :   public Panels
 {
    public:
        /** @brief          The constructor function for the `DropDownMenus` class
         *                  which creates an instance of the class and adds the 
         *                  chosen options to the shift keying drop down specifically.
         *                  This also makes the components within it visible to the 
         *                  user.
         */
        DropDownMenus();

        /** @brief          The destructor function for the `DropDownMenus` class
         *                  which is set to default as no child object within the
         *                  instance need to be altered within the class.
         */
        ~DropDownMenus() = default;

        /** @brief      Adding a function that will allow for the `DropDownMenus`
                        to be resized when the window size is adjusted to fit best.
        */
        void resized() override;

    private:
        /*  Shift keying drop down menu and label */
        DropDownMenu shiftKeyChoice;

        /*  Creating an instance of the CalibrateEmbouchure class. */
        CalibrateEmbouchure calibrateEmbouchure;
 };

 /**    @brief      A class which adds all the linear sliders being used
  *                 within the calibration section to be within the same    
  *                 panel.
  */
 class Sliders : public Panels,
                public juce::Slider::Listener
 {
    public:
        /*  Creating an instance of the CalibrationSlider class to be
            used for calibrating the maximum and minimum distance 
            which the trombonist wishes to have their hand for the
            pitch bend. */
        CalibrationSlider distanceSlider;

        /*  Creating an instance of the CalibrationSliderLookAndFeel
            class which will later be applied to the slider to look
            like a trombone being actuated to its full length. */
        CalibrationSliderLookAndFeel LandF;

        /** @brief          The constructor function for the `Sliders` class
         *                  which sets the maximum and minimum distance of 
         *                  the slider as well as its knobs' initial
         *                  positions and other such initialisation steps.
         */
        Sliders();

        /** @brief          The destructor function for the `Sliders` class
         *                  which removes the look and feel of the sliders
         *                  from the sliders as well as their listeners.
         */
        ~Sliders();

        /** @brief      Adding a function that will allow for the `Sliders` class
         *              to be resized when the window size is adjusted to fit best.
         *  @note       For this function, this sets the size of the labels to be 
         *              a maximum size and scales them downwards if the relative
         *              size of the window is too small to fit the phrase. This
         *              scales the length of the trombone to be able to fit in
         *              a window with a width of any size.              
         */
        void resized() override;

        /** @brief                  A callback function which is automatically called upon when
         *                          one of the sliders present has the value of one of it's
         *                          knobs changed.
         *  @param  sliderChange    A pointer to the slider which has been modified. This
         *                          allows for the comparison between the different sliders
         *                          within the class to detect which one has been affected.
         *  @note                   This is where the interaction with the Core side of the
         *                          Tromboneless takes place by referring to the specific
         *                          variable which stores all the calibration data for the 
         *                          Tromboneless.
         */
        void sliderValueChanged(juce::Slider* sliderChanged) override;

    private:
        /*  Sets the minimum distance the users hand may be from the Tromboneless for it to
            be detected and counted as a position on the pitch bend. */
        float minimumDistance = 5;
        /*  Sets the maxmimum distance the users hand may be from the Tromboneless for it to
            be detected and counted as a position on the pitch bend. */
        float maximumDistance = 60;
        /*  Sets the minimum change in distance the user can have when adjusting the slider.
            This is essentially the resolution of the slider. */
        float stepDistance = 0.1;  
        /*  Setting the minimum distance that the maximum and minimum hand positions must be
            from each other. This reduces the chances of having ambiguity in the pitch bend. */ 
        float distanceRange = 5;
 };

 /**    @brief      A class which may be called to add a panel to
  *                 the window contains a series of sliders for the
  *                 equalizer allowing for live editting of the 
  *                 synthesisers sound to ensure it sounds how the 
  *                 Trombonist wishes it to.
  */
 class EqualizerPanel   :   public Panels,
                            public juce::Button::Listener
 {
    public:
        /** @brief                  The constructor function for the `EqualizerPanel` class
         *                          which adds the child components to the parent window and
         *                          attaches a listener to the equalizer to detect when the
         *                          slider values have been adjusted by the user.
         */
        EqualizerPanel();

        /** @brief                  The destructor function which for the `EqualizerPanel` class
         *                          which removes the listener from the equalizer before 
         *                          deinitialising.
         */
        ~EqualizerPanel();

        /** @brief                  A function which changes the size of the child components 
         *                          when the parent window has changed in size.
         *  @note                   This function currently adds a slight margin around the
         *                          edges of the bounds to ensure that the child components
         *                          do not overlap with any external components.
         */
        void resized() override;

        /** @brief                  A callback function which is called upon automatically when
         *                          the user has pressed a button within this class.
         *  @param  buttonClicked   A pointer to the button which has been clicked.
         *  @note                   As this class only has one button, there is no need to check
         *                          which button has been clicked but it is predefined to pass a
         *                          parameter in the juce library. This button is used to show or 
         *                          hide the equalizer from the user in the parent window.
         */
        void buttonClicked(juce::Button* buttonClicked) override;

    private:
        /*  Initialising the button which will be used to show or hide the equalizer. */
        juce::ToggleButton  button;
        /*  Initialising the equalizer which will allow the user to interact with the
            gains of the frequency bands for the synthesiser. */
        Equalizer equalizer;
        /*  Initialising the label which will appear beside the enable/disable button
            which will indicate what pressing the button will do. */
        juce::Label buttonLabel;

 };