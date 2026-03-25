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
/*                                CalibrationRotarySlider                                     */
/*                                                                                            */
/* ========================================================================================== */

 CalibrationRotarySlider::CalibrationRotarySlider() : SliderWithLabel(juce::Slider::Rotary)
 {
    this->slider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    this->slider.addListener(this);

    /* Removing an option to enter the data using a text box. */
    maxSlider.slider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    
    /* Adding a listener to the function to perform the correct operations. */
    maxSlider.slider.addListener(this);

    /* Allowing the sliders to be viewed. */
    addAndMakeVisible(maxSlider);
    

 }

void CalibrationRotarySlider::setMinDifference(double difference){
    minDistance = difference;
}

void CalibrationRotarySlider::sliderValueChanged(juce::Slider* sliderChanged){
    auto min = this->slider.getValue();
    auto max = maxSlider.slider.getValue();

    /* Checking if the values are out of range of each other. */
    if (max - min < minDistance)
    {   
        /* Checking if the maximum value is already at the maximum. If it is, only the minimum finger is moved. */
        if (max >= maxSlider.slider.getValue()){
            max = maxSlider.slider.getValue();
            maxSlider.slider.setValue(max);
            this->slider.setValue(max - minDistance);
            return;
        }

        /* Checking if the minimum value is already at the minimum. If it is, only the maximum finger is moved. */
        else if (min <= this->slider.getValue()){
            min = this->slider.getValue();
            this->slider.setValue(min);
            maxSlider.slider.setValue(min + minDistance);
            return;
        }
        
        /* If neither finger is on the edge, onlt the finger not being dragged is adjusted to ensure the minimum range is maintained. */
        else if (sliderChanged == &this->slider){
            maxSlider.slider.setValue(min + minDistance);
            return;
        }

        else{
            this->slider.setValue(max - minDistance);
            return;
        }
    }
    return;
}

void CalibrationRotarySlider::resized()
{
    auto area = getLocalBounds();

    maxSlider.setBounds(area); // max slider
    // maxSlider.setInterceptsMouseClicks(false, false);
    slider.setBounds(area.withSizeKeepingCentre(area.getWidth()*0.95, area.getHeight()*0.95));   // main slider
}

// void CalibrationRotarySlider::mouseDown(const juce::MouseEvent& e)
// {
//     auto centre = getLocalBounds().getCentre().toFloat();
//     auto pos = e.position;

//     float distance = pos.getDistanceFrom(centre);

//     float radius = getWidth() * 0.5f;

//     if (distance > radius * 0.7f)
//         maxSlider.mouseDown(e.getEventRelativeTo(&maxSlider));
//     else
//         slider.mouseDown(e.getEventRelativeTo(&slider));
// }

void CalibrationRotarySlider::mouseDrag(const juce::MouseEvent& e)
{
    if (activeSlider == nullptr)
        return;

    float delta = -e.getDistanceFromDragStartY() * 0.01f;

    activeSlider->setValue(
        activeSlider->getValue() + delta,
        juce::sendNotificationSync
    );
}

void CalibrationRotarySlider::mouseUp(const juce::MouseEvent& e)
{
    if (activeSlider != nullptr)
        activeSlider->mouseUp(e.getEventRelativeTo(activeSlider));

    activeSlider = nullptr;
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
    auto labelBounds = area.removeFromLeft (std::min(area.getWidth()*0.6, double(80)));
    dropDownLabel.setBounds (labelBounds);
    dropDownChoice.setBounds (area.withSizeKeepingCentre(area.getWidth()*0.95, area.getHeight()));
}

 void DropDownMenu::AddItem(juce::String itemName, int itemValue){
    dropDownChoice.addItem(itemName, itemValue);
 }

 void DropDownMenu::ChangeLabelText(juce::String text, juce::NotificationType notification){
    dropDownLabel.setText(text, notification);
 }



/* ========================================================================================== */
/*                                                                                            */
/*                                   CalibrationOnClick                                       */
/*                                                                                            */
/* ========================================================================================== */

 CalibrationOnClick::CalibrationOnClick(){
    /* Setting the button to only be momentary and not a switch. */
    button.setToggleable(false);
    /* Setting the button to only be triggered on the mouse down motion. */
    button.setTriggeredOnMouseDown(true);
    /* Attaching a listener so that this object can perform functions 
    when the button is pressed. */
    button.addListener(this);

 }

 void CalibrationOnClick::resized(){
    juce::Rectangle<int> area = getLocalBounds();
    juce::Rectangle<int> buttonBounds = area.withSizeKeepingCentre(area.getWidth()*0.9, area.getHeight()*0.9);
    button.setBounds(buttonBounds);

 }


/* ========================================================================================== */
/*                                                                                            */
/*                                   CalibrateEmbachure                                       */
/*                                                                                            */
/* ========================================================================================== */

CalibrateEmbachure::CalibrateEmbachure(){
    /* Adding the dropdown menu to select the start */
    addAndMakeVisible (embachureChoice);
    embachureChoice.ChangeLabelText("Embachure\nselection: ");
    embachureChoice.AddItem ("Middle F4", SKOpt_MiddleF4);
    embachureChoice.AddItem ("B sharp 4", SKOpt_BSharp4);
    embachureChoice.AddItem ("D5", SKOpt_D5);
    embachureChoice.AddItem ("F5", SKOpt_F5);
    embachureChoice.AddItem ("A sharp 4", SKOpt_ASharp4);
    
    /* This line is the one responsible for calling the shiftKeyingUpdate function when the choice changes. */
    embachureChoice.OnChange (&trombonelessParameters.embachureOption);

    /* Adding the button. */
    addAndMakeVisible (this->button);
    this->button.setButtonText("Calibrate");

}

void CalibrateEmbachure::resized(){
    juce::Rectangle<int> area = getLocalBounds();
    /* Adding a margin around the edge. */
    area = area.withSizeKeepingCentre(area.getWidth()*0.9, area.getHeight()*0.8);

    /* Making the button take up 10% of the space or 60 pixels.*/
    juce::Rectangle<int> buttonBounds = area.removeFromRight(std::min((float) (area.getWidth()*0.4), (float) 100));
    buttonBounds = buttonBounds.withSizeKeepingCentre((float)(buttonBounds.getWidth()), (float) (buttonBounds.getHeight()*0.5));
    this->button.setBounds(buttonBounds);

    /* Allowing the rest of the area to go to the dropdown and label. */
    embachureChoice.setBounds(area);

}

void CalibrateEmbachure::buttonClicked(juce::Button* button)
{
    if (button == &this->button){
        trombonelessParameters.triggerEmbachureCalibrate = true;
    }
}
