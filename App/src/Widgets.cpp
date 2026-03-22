/** @file       Widgets.cpp
 *  @author     Ryan McBride
 *  @brief      A file which defines the functions which have been declared
 *              within the `Widgets.hpp` file.
*/

/* Adding the necessary header files. */
#include    "Widgets.hpp"

/* ========================================================================================== */
/*                                                                                            */
/*                                   SliderWithLabel                                          */
/*                                                                                            */
/* ========================================================================================== */

SliderWithLabel::InitialiseLabelErrors_t SliderWithLabel::CreateLabel(LabelPositions_t position,
            juce::String phrase,
            juce::NotificationType NotificationEnable){
                
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

void SliderWithLabel::resized()
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
    MiddleRightLabel.setBounds(CentreLabelBounds.removeFromRight(rightLabelBounds));

    /* Top labels. */
    BottomLabelBounds = area.removeFromBottom(bottomLabelBounds);
    LowerLeftLabel.setBounds(BottomLabelBounds.removeFromLeft(leftLabelBounds));
    LowerCentreLabel.setBounds(BottomLabelBounds);
    LowerRightLabel.setBounds(BottomLabelBounds.removeFromRight(rightLabelBounds));

    /* Adding slider */
    slider.setBounds(area);
}


/* ========================================================================================== */
/*                                                                                            */
/*                                   SliderWithLabel                                          */
/*                                                                                            */
/* ========================================================================================== */