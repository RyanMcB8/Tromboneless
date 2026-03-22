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
            UpperLeftLabel.setJustificationType(juce::Justification::Flags::bottomLeft);
            addAndMakeVisible (UpperLeftLabel);
            break;
        
        case(UpperCentre):
            labelSetup(&UpperCentreLabel, phrase, NotificationEnable);
            UpperCentreLabel.setJustificationType(juce::Justification::Flags::centredBottom);
            addAndMakeVisible (UpperCentreLabel);
            break;

        case(UpperRight):
            labelSetup(&UpperRightLabel, phrase, NotificationEnable);
            UpperRightLabel.setJustificationType(juce::Justification::Flags::bottomRight);
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
            LowerLeftLabel.setJustificationType(juce::Justification::Flags::topLeft);
            addAndMakeVisible (LowerLeftLabel);
            break;
        
        case(LowerCentre):
            labelSetup(&LowerCentreLabel, phrase, NotificationEnable);
            LowerCentreLabel.setJustificationType(juce::Justification::Flags::centredTop);
            addAndMakeVisible (LowerCentreLabel);
            break;

        case(LowerRight):
            labelSetup(&LowerRightLabel, phrase, NotificationEnable);
            LowerRightLabel.setJustificationType(juce::Justification::Flags::topRight);
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
    juce::Rectangle<int> LeftLabelBounds = TopLabelBounds.removeFromLeft(std::min(area.getWidth()*0.2, (double) leftLabelBounds));
    UpperLeftLabel.setBounds(LeftLabelBounds);

    juce::Rectangle<int> RightLabelBounds = TopLabelBounds.removeFromRight(std::min(area.getWidth()*0.2, (double) rightLabelBounds));
    UpperRightLabel.setBounds(RightLabelBounds);
    
    UpperCentreLabel.setBounds(TopLabelBounds);

    /* Bottom labels. */
    BottomLabelBounds = area.removeFromBottom(std::min(area.getHeight()*0.5, (double) bottomLabelBounds));
    LeftLabelBounds = BottomLabelBounds.removeFromLeft(std::min(area.getWidth()*0.2, (double) leftLabelBounds));
    LowerLeftLabel.setBounds(LeftLabelBounds);

    RightLabelBounds = BottomLabelBounds.removeFromRight(std::min(area.getWidth()*0.2, (double) rightLabelBounds));
    LowerRightLabel.setBounds(RightLabelBounds);
    
    LowerCentreLabel.setBounds(BottomLabelBounds);

    /* Middle labels. */
    CentreLabelBounds = area;
    LeftLabelBounds = CentreLabelBounds.removeFromLeft(std::min(area.getWidth()*0.2, (double) leftLabelBounds));
    MiddleLeftLabel.setBounds(LeftLabelBounds);

    RightLabelBounds = CentreLabelBounds.removeFromRight(std::min(area.getWidth()*0.2, (double) rightLabelBounds));
    MiddleRightLabel.setBounds(RightLabelBounds);


    /* Adding slider */
    slider.setBounds(area);
}

void SliderWithLabel::labelSetup(juce::Label* Label, juce::String phrase,
                juce::NotificationType NotificationEnable){
    Label->setText(phrase, NotificationEnable);
    Label->setEditable(false);
    addAndMakeVisible (Label);
}


/* ========================================================================================== */
/*                                                                                            */
/*                                   CalibrationSlider                                        */
/*                                                                                            */
/* ========================================================================================== */

 CalibrationSlider::CalibrationSlider() :   SliderWithLabel(juce::Slider::TwoValueHorizontal){
        this->slider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
        this->slider.addListener(this);
    }

 void CalibrationSlider::setMinDifference(double difference){
     minDistance = difference;
 }

 void CalibrationSlider::sliderValueChanged(juce::Slider* sliderChanged) 
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

/* ========================================================================================== */
/*                                                                                            */
/*                                   verticalMixSlider                                        */
/*                                                                                            */
/* ========================================================================================== */

 verticalMixSlider::verticalMixSlider() : SliderWithLabel(juce::Slider::LinearVertical){
        this->slider.setRange(0, 2, 0.01);                     /* Setting the maximum and minimum gain values for each band. */
        this->slider.setValue(1, juce::dontSendNotification);  /* Setting the initial value to be 1 so that there is no change in gain. */
        this->slider.setPopupMenuEnabled(1);
        this->topLabelBounds = 30;
        this->leftLabelBounds = 1;
        this->rightLabelBounds = 1;
        this->bottomLabelBounds = 30; 
        this->slider.addListener(this);
    };

 void verticalMixSlider::sliderValueChanged(juce::Slider* sliderChanged) {
        /* Add any code for responding to slider values being changed here.*/
        if (sliderChanged){}
    }

/* ========================================================================================== */
/*                                                                                            */
/*                                       Equalizer                                            */
/*                                                                                            */
/* ========================================================================================== */

 Equalizer::Equalizer(char numberOfSliders): verticalMixSlider(){
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

 Equalizer::~Equalizer(){
    for (char i=0; i < nSliders; i++){
        eqSliders[i]->slider.setLookAndFeel(nullptr);
    }
}

void Equalizer::resized()
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

void Equalizer::sliderValueChanged(juce::Slider* sliderChanged) {
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
