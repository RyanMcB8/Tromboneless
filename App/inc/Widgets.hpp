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
        
        /* Creation of labels that may be used with the slider. */
        juce::Label UpperLeftLabel;
        juce::Label UpperCentreLabel;
        juce::Label UpperRightLabel;
        juce::Label MiddleLeftLabel;
        juce::Label MiddleRightLabel;
        juce::Label LowerLeftLabel;
        juce::Label LowerCentreLabel;
        juce::Label LowerRightLabel;
        juce::Slider slider;

        /* Bound parameters. */
        int topLabelBounds = 20;
        int leftLabelBounds = 50;
        int rightLabelBounds = 50;
        int bottomLabelBounds = 20; 

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
        void labelSetup(juce::Label* Label,
                                juce::String phrase,
                                juce::NotificationType NotificationEnable){
                    Label->setText(phrase, NotificationEnable);
                    Label->setEditable(false);
                    addAndMakeVisible (Label);
                                }

};


/** @brief A class that adds more functionality to the slider class from Juce. */
class CalibrationSlider :   public SliderWithLabel,
                            public juce::Slider::Listener
{
public:
    double minDistance = 1;

    /** @brief Contructor for the `CalibrationSlider` class which requires the
     *  type of slider being used to be initiate which is set to `TwoValueHorizontal`
     *  by default.
     */ 
    CalibrationSlider() :SliderWithLabel(juce::Slider::TwoValueHorizontal){
        this->slider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
        this->slider.addListener(this);
    }

    /** @brief A function which sets the minimum difference between the upper and lower fingers of the slider.
     *  @param difference The value of the difference between the fingers.
     */
    void setMinDifference(double difference){
        minDistance = difference;
    }


    /** @brief A function that is called upon when either of the fingers are moved.
     *  @note This function automatically moves the fingers to ensure that they stay at least the minimum range apart from each other.
     */
    void sliderValueChanged(juce::Slider* sliderChanged) override
    {
        auto min = sliderChanged->getMinValue();
        auto max = sliderChanged->getMaxValue();

        /* Checking if the values are out of range of each other. */
        if (max - min < minDistance)
        {   
            /* Checking if the maximum value is already at the maximum. If it is, only the minimum finger is moved. */
            if (max >= sliderChanged->getMaximum()){
                max = sliderChanged->getMaximum();
                sliderChanged->setMaxValue(max);
                sliderChanged->setMinValue(max - minDistance);
                return;
            }

            /* Checking if the minimum value is already at the minimum. If it is, only the maximum finger is moved. */
            else if (min <= sliderChanged->getMinimum()){
                min = sliderChanged->getMinimum();
                sliderChanged->setMinValue(min);
                sliderChanged->setMaxValue(min + minDistance);
                return;
            }
            
            /* If neither finger is on the edge, onlt the finger not being dragged is adjusted to ensure the minimum range is maintained. */
            else if (sliderChanged->getThumbBeingDragged() == 1){
                sliderChanged->setMaxValue(min + minDistance);
                return;
            }

            else{
                sliderChanged->setMinValue(max - minDistance);
                return;
            }
        }
        return;
    }
};


/** @brief A class that adds more functionality to the slider class from Juce. */
class verticalMixSlider : public SliderWithLabel,
                          public juce::Slider::Listener
{
public:
    verticalMixSlider() : SliderWithLabel(juce::Slider::LinearVertical){
        this->slider.setRange(0, 2, 0.01);                     /* Setting the maximum and minimum gain values for each band. */
        this->slider.setValue(1, juce::dontSendNotification);  /* Setting the initial value to be 1 so that there is no change in gain. */
        this->slider.setPopupMenuEnabled(1);
        this->topLabelBounds = 30;
        this->leftLabelBounds = 1;
        this->rightLabelBounds = 1;
        this->bottomLabelBounds = 30; 
        this->slider.addListener(this);
    };

    ~verticalMixSlider() {
        
    };

    void sliderValueChanged(juce::Slider* sliderChanged) override
    {
        /* Add any code for responding to slider values being changed here.*/
        if (sliderChanged){}
    }
    
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
        Equalizer(char numberOfSliders = 10): verticalMixSlider(){
            
            /* If there is no change in the number of sliders, the system will use the standard range. */
            if(numberOfSliders == 10){
                frequencyLabels.add ((juce::String) "0", (juce::String) "31.25");
                prevLabel = 31.25;
            }

            nSliders = numberOfSliders;
            /* Looping through every slider, creating it, setting its attributes adn adding it to an array. */
            for (char i=0; i < nSliders ; i++){
                
                /* Adding all n sliders which will be used. */
                eqSliders.add(new verticalMixSlider());
                eqSliders[i]->slider.setPopupDisplayEnabled(true, true, this, 1000);
                eqSliders[i]->slider.setLookAndFeel(&customLook);
                eqSliders[i]->slider.addListener(this);
                
                if(numberOfSliders == 10){
                    newLabel = (prevLabel+prevLabel);
                    if (newLabel >= 1000){
                        // juce::String newString = (newLabel/1000) + "k"
                        frequencyLabels.add (((juce::String) (newLabel/1000))+ "k");
                    }
                    else{
                        frequencyLabels.add ((juce::String) (newLabel));
                    }
                    prevLabel = newLabel; 
                }
                
                /* Adding the appropriate labels to the slider. */
                eqSliders[i]->CreateLabel(SliderWithLabel::LabelPositions_t::UpperCentre, frequencyLabels[i+1]);
                eqSliders[i]->CreateLabel(SliderWithLabel::LabelPositions_t::LowerCentre, frequencyLabels[i]);
            }
            
        }

        /*Destructor. */
        ~Equalizer(){
            for (char i=0; i < nSliders; i++){
                eqSliders[i]->slider.setLookAndFeel(nullptr);
            }
        }
        
        /** @brief A function to set the size of the sliders with respect to the equalizer.
         * 
         */
        void resized() override
        {
            /* Finding the local bounds for the entire object. */
            juce::Rectangle<int> totalArea = getLocalBounds();
            juce::Rectangle<int> sliderBound;
            
            /* Looping through and having the sliders share the same amount of space each. */
            for (char i=0; i < nSliders ; i++){
                sliderBound = totalArea.removeFromLeft((totalArea.getWidth()) / (nSliders - i));
                eqSliders[i]->setBounds(sliderBound);
                addAndMakeVisible (eqSliders[i]);
            }
            
        }

        void sliderValueChanged(juce::Slider* sliderChanged) override{
            for (int i=0; i < nSliders; i++){
                if (sliderChanged == &(eqSliders[i]->slider)){
#ifdef DBG_MSG
                    std::cout << "Slider:"<< i << "\n";
#endif              
                    /* Updating the slider value in the synthesiserParameters struct to match the new updated value. */
                    synthesiserParameters.gains[i] = eqSliders[i]->slider.getValue();
                }
            }
            return;            
        }
        
    private:
        juce::Array <juce::String> frequencyLabels; 
        float prevLabel;
        float newLabel;
        VerticalSliderLookAndFeel customLook;
        
};