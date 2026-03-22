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
            UpperCentreLabel.setJustificationType(juce::Justification::Flags::centredBottom);
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
    TopLabelBounds = area.removeFromTop(std::min(area.getHeight()*0.3, (double) topLabelBounds));
    UpperLeftLabel.setBounds(TopLabelBounds.removeFromLeft(std::min(area.getWidth()*0.2, (double) leftLabelBounds)));
    UpperCentreLabel.setBounds(TopLabelBounds);
    UpperRightLabel.setBounds(TopLabelBounds.removeFromRight(std::min(area.getWidth()*0.2, (double) rightLabelBounds)));

    /* Middle labels. */
    CentreLabelBounds = area;
    MiddleLeftLabel.setBounds(CentreLabelBounds.removeFromLeft(std::min(area.getWidth()*0.2, (double) leftLabelBounds)));
    MiddleRightLabel.setBounds(CentreLabelBounds.removeFromRight(std::min(area.getWidth()*0.2, (double) rightLabelBounds)));

    /* Top labels. */
    BottomLabelBounds = area.removeFromBottom(std::min(area.getHeight()*0.5, (double) bottomLabelBounds));
    LowerLeftLabel.setBounds(BottomLabelBounds.removeFromLeft(std::min(area.getWidth()*0.2, (double) leftLabelBounds)));
    LowerCentreLabel.setBounds(BottomLabelBounds);
    LowerRightLabel.setBounds(BottomLabelBounds.removeFromRight(std::min(area.getWidth()*0.2, (double) rightLabelBounds)));

    /* Adding slider */
    slider.setBounds(area);
}


/* ========================================================================================== */
/*                                                                                            */
/*                                   SliderWithLabel                                          */
/*                                                                                            */
/* ========================================================================================== */




/* ========================================================================================== */
/*                                                                                            */
/*                                   Drop Down Menu                                           */
/*                                                                                            */
/* ========================================================================================== */
 DropDownMenu::DropDownMenu(juce::String labelPhrase){
    addAndMakeVisible (dropDownChoice);
    dropDownChoice.setSelectedId (0);

    /* Adding a label to be beside the shift key dropdown menu. */
    addAndMakeVisible (dropDownLabel);
    dropDownLabel.setText (labelPhrase, juce::dontSendNotification);
    dropDownLabel.setJustificationType (juce::Justification::centred);
    dropDownLabel.attachToComponent(&dropDownChoice, true);
 }

 void DropDownMenu::resized(){
    auto area = getLocalBounds();
    auto labelBounds = area.removeFromLeft (std::min(area.getWidth()*0.2, double(150)));
    dropDownLabel.setBounds (labelBounds);
    dropDownChoice.setBounds (area.withSizeKeepingCentre(area.getWidth()*0.95, area.getHeight()));
}

 void DropDownMenu::AddItem(juce::String itemName, int itemValue){
    dropDownChoice.addItem(itemName, itemValue);
 }

 void DropDownMenu::ChangeLabelText(juce::String text, juce::NotificationType notification){
    dropDownLabel.setText(text, notification);
 }
