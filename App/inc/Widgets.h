#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include <stdexcept>
#include <string>
#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

/* A header file containing custom widgets to appear on the screen to reduce and repetitive definitions within the main Layout.cpp file. */

class SliderWithLabel : public juce::Component{
    public:
        
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
        juce::Slider slider;
        

        SliderWithLabel(juce::Slider::SliderStyle style){
            slider.setSliderStyle(style);
            slider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
            addAndMakeVisible(slider);
        }


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
                    addAndMakeVisible (UpperLeftLabel);
                    break;
                
                case(UpperCentre):
                    labelSetup(&UpperCentreLabel, phrase, NotificationEnable);
                    UpperCentreLabel.setJustificationType(juce::Justification::Flags::centredTop);
                    addAndMakeVisible (UpperCentreLabel);
                    break;

                case(UpperRight):
                    labelSetup(&UpperRightLabel, phrase, NotificationEnable);
                    UpperRightLabel.setJustificationType(juce::Justification::Flags::topRight);
                    addAndMakeVisible (UpperRightLabel);
                    break;

                case(MiddleLeft):
                    labelSetup(&MiddleLeftLabel, phrase, NotificationEnable);
                    MiddleLeftLabel.setJustificationType(juce::Justification::Flags::centredLeft);
                    addAndMakeVisible (MiddleLeftLabel);
                    break;
                
                case(MiddleCentre):
                    labelSetup(&MiddleCentreLabel, phrase, NotificationEnable);
                    MiddleCentreLabel.setJustificationType(juce::Justification::Flags::centred);
                    addAndMakeVisible (MiddleCentreLabel);
                    break;

                case(MiddleRight):
                    labelSetup(&MiddleRightLabel, phrase, NotificationEnable);
                    MiddleRightLabel.setJustificationType(juce::Justification::Flags::centredRight);
                    addAndMakeVisible (MiddleRightLabel);
                    break;

                case(LowerLeft):
                    labelSetup(&LowerLeftLabel, phrase, NotificationEnable);
                    LowerLeftLabel.setJustificationType(juce::Justification::Flags::bottomLeft);
                    addAndMakeVisible (LowerLeftLabel);
                    break;
                
                case(LowerCentre):
                    labelSetup(&LowerCentreLabel, phrase, NotificationEnable);
                    LowerCentreLabel.setJustificationType(juce::Justification::Flags::centredBottom);
                    addAndMakeVisible (LowerCentreLabel);
                    break;

                case(LowerRight):
                    labelSetup(&LowerRightLabel, phrase, NotificationEnable);
                    LowerRightLabel.setJustificationType(juce::Justification::Flags::bottomRight);
                    addAndMakeVisible (LowerRightLabel);
                    break;
                default:
                    return PositonNotAccepted;                
            }
            
            
            return LabelOK;
        }

        void resized() override
        {
            /* Finding the local bounds for the entire object. */
            juce::Rectangle<int> area = getLocalBounds();
            
            /* Initialisation of the bounds for each section. */
            juce::Rectangle<int> TopLabelBounds;
            juce::Rectangle<int> CentreLabelBounds;
            juce::Rectangle<int> BottomLabelBounds;

            /* Top labels. */
            TopLabelBounds = area.removeFromTop(topLabelBounds);
            UpperLeftLabel.setBounds(TopLabelBounds.removeFromLeft(leftLabelBounds));
            UpperCentreLabel.setBounds(TopLabelBounds);
            UpperRightLabel.setBounds(TopLabelBounds.removeFromRight(rightLabelBounds));

            /* Middle labels. */
            CentreLabelBounds = area;
            MiddleLeftLabel.setBounds(CentreLabelBounds.removeFromLeft(leftLabelBounds));
            MiddleCentreLabel.setBounds(CentreLabelBounds);
            MiddleRightLabel.setBounds(CentreLabelBounds.removeFromRight(rightLabelBounds));

            /* Top labels. */
            BottomLabelBounds = area.removeFromBottom(bottomLabelBounds);
            LowerLeftLabel.setBounds(BottomLabelBounds.removeFromLeft(leftLabelBounds));
            LowerCentreLabel.setBounds(BottomLabelBounds);
            LowerRightLabel.setBounds(BottomLabelBounds.removeFromRight(rightLabelBounds));

            /* Adding slider */
            slider.setBounds(area);
        }

    private:
        
        void labelSetup(juce::Label* Label,
                                juce::String phrase,
                                juce::NotificationType NotificationEnable){
                    Label->setText(phrase, NotificationEnable);
                    Label->setEditable(false);
                    addAndMakeVisible (Label);
                                }

        

        int topLabelBounds = 20;
        int leftLabelBounds = 40;
        int rightLabelBounds = 40;
        int bottomLabelBounds = 20; 
};


/** @brief A class that adds more functionality to the slider class from Juce. */
class CalibrationSlider :   public SliderWithLabel,
                            public juce::Slider::Listener
{
public:
    double minDistance = 1;

    CalibrationSlider() :SliderWithLabel(juce::Slider::TwoValueHorizontal){
        // this->slider.setSliderStyle(Slider::TwoValueHorizontal);
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
class verticalMixSlider : public SliderWithLabel
{
public:
    verticalMixSlider() : SliderWithLabel(juce::Slider::LinearVertical){
        // this->slider.setSliderStyle(LinearVertical);           /* Setting the type of slider being used. */
        this->slider.setRange(0, 2, 0.01);                     /* Setting the maximum and minimum gain values for each band. */
        this->slider.setValue(1, juce::dontSendNotification);  /* Setting the initial value to be 1 so that there is no change in gain. */
        this->slider.setPopupMenuEnabled(1);
    };

    ~verticalMixSlider() {};
    
};