#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include <stdexcept>
#include <string>
#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
// #include shared header

/* A header file containing custom widgets to appear on the screen to reduce and repetitive definitions within the main Layout.cpp file. */

class SliderWithLabel : public juce::Slider{

    public:

        /* Enumerated list of possible positions for the label to be attached relative to the slider. */
        typedef enum{
            UpperLeft, UpperCentre, UpperRight,
            MiddleLeft, MiddleCentre, MiddleRight,
            LowerLeft, LowerCentre, LowerRight,
        } LabelPositions_t;

        /* Enumerated list of possibel errors when initialising the label. */
        typedef enum{
            LabelOK,
            PositonNotAccepted,
        } InitialiseLabelErrors_t;

        InitialiseLabelErrors_t CreateLabel(LabelPositions_t position,
                         juce::String phrase,
                         juce::NotificationType NotificationEnable = juce::dontSendNotification){

            switch(position){
                case(UpperLeft):
                    labelSetup(&UpperLeftLabel, phrase, NotificationEnable);
                    UpperLeftLabel.setJustificationType(juce::Justification::Flags::topLeft);
                    UpperLeftLabel.setBounds(40, 40, 40, 40);
                    addAndMakeVisible (UpperLeftLabel);
                    break;
                
                case(UpperCentre):
                    addAndMakeVisible (UpperCentreLabel);
                    labelSetup(&UpperCentreLabel, phrase, NotificationEnable);
                    UpperCentreLabel.setJustificationType(juce::Justification::Flags::centredTop);
                    UpperCentreLabel.setBounds(100, 400, 100, 100);
                    
                    break;

                case(UpperRight):
                    labelSetup(&UpperRightLabel, phrase, NotificationEnable);
                    UpperRightLabel.setJustificationType(juce::Justification::Flags::topRight);
                    UpperRightLabel.setBounds(40, 40, 40, 40);
                    addAndMakeVisible (UpperRightLabel);
                    break;

                case(MiddleLeft):
                    labelSetup(&MiddleLeftLabel, phrase, NotificationEnable);
                    MiddleLeftLabel.setJustificationType(juce::Justification::Flags::centredLeft);
                    MiddleLeftLabel.setBounds(40, 40, 40, 40);
                    addAndMakeVisible (MiddleLeftLabel);
                    break;
                
                case(MiddleCentre):
                    labelSetup(&MiddleCentreLabel, phrase, NotificationEnable);
                    MiddleCentreLabel.setJustificationType(juce::Justification::Flags::centred);
                    MiddleCentreLabel.setBounds(40, 40, 40, 40);
                    addAndMakeVisible (MiddleCentreLabel);
                    break;

                case(MiddleRight):
                    labelSetup(&MiddleRightLabel, phrase, NotificationEnable);
                    MiddleRightLabel.setJustificationType(juce::Justification::Flags::centredRight);
                    MiddleRightLabel.setBounds(40, 40, 40, 40);
                    addAndMakeVisible (MiddleRightLabel);
                    break;

                case(LowerLeft):
                    labelSetup(&LowerLeftLabel, phrase, NotificationEnable);
                    LowerLeftLabel.setJustificationType(juce::Justification::Flags::bottomLeft);
                    LowerLeftLabel.setBounds(40, 40, 40, 40);
                    addAndMakeVisible (LowerLeftLabel);
                    break;
                
                case(LowerCentre):
                    labelSetup(&LowerCentreLabel, phrase, NotificationEnable);
                    LowerCentreLabel.setJustificationType(juce::Justification::Flags::centredBottom);
                    LowerCentreLabel.setBounds(40, 40, 40, 40);
                    addAndMakeVisible (LowerCentreLabel);
                    break;

                case(LowerRight):
                    labelSetup(&LowerRightLabel, phrase, NotificationEnable);
                    LowerRightLabel.setJustificationType(juce::Justification::Flags::bottomRight);
                    LowerRightLabel.setBounds(40, 40, 40, 40);
                    addAndMakeVisible (LowerRightLabel);
                    break;
                default:
                    return PositonNotAccepted;                
            }



            return LabelOK;
        }

    private:
        
        void labelSetup(juce::Label* Label,
                                juce::String phrase,
                                juce::NotificationType NotificationEnable){
                    Label->setText(phrase, NotificationEnable);
                    Label->setEditable(false);
                                }

        /* Creation of labels that may be used with the slider. */
        juce::Label UpperLeftLabel;
        juce::Label UpperCentreLabel;
        juce::Label UpperRightLabel;
        juce::Label MiddleLeftLabel;
        juce::Label MiddleCentreLabel;
        juce::Label MiddleRightLabel;
        juce::Label LowerLeftLabel;
        juce::Label LowerCentreLabel;
        juce::Label LowerRightLabel;
};


/** @brief A class that adds more functionality to the slider class from Juce. */
class CalibrationSlider : public SliderWithLabel
{
public:
    double minDistance = 1;

    CalibrationSlider(){
        this->setSliderStyle(Slider::TwoValueHorizontal);
        this->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
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
    void valueChanged() override
    {
        auto min = getMinValue();
        auto max = getMaxValue();

        /* Checking if the values are out of range of each other. */
        if (max - min < minDistance)
        {   
            /* Checking if the maximum value is already at the maximum. If it is, only the minimum finger is moved. */
            if (max >= getMaximum()){
                max = getMaximum();
                setMaxValue(max);
                setMinValue(max - minDistance);
                return;
            }

            /* Checking if the minimum value is already at the minimum. If it is, only the maximum finger is moved. */
            else if (min <= getMinimum()){
                min = getMinimum();
                setMinValue(min);
                setMaxValue(min + minDistance);
                return;
            }
            
            /* If neither finger is on the edge, onlt the finger not being dragged is adjusted to ensure the minimum range is maintained. */
            else if (getThumbBeingDragged() == 1){
                setMaxValue(min + minDistance);
                return;
            }

            else{
                setMinValue(max - minDistance);
                return;
            }
        }
        return;
    }
};


/** @brief A class that adds more functionality to the slider class from Juce. */
class verticalMixSlider : public SliderWithLabel
{
public:
    verticalMixSlider(){
        this->setSliderStyle(LinearVertical);           /* Setting the type of slider being used. */
        this->setRange(0, 2, 0.01);                     /* Setting the maximum and minimum gain values for each band. */
        this->setValue(1, juce::dontSendNotification);  /* Setting the initial value to be 1 so that there is no change in gain. */
        this->setPopupMenuEnabled(1);
    };

    ~verticalMixSlider() {};
    
};