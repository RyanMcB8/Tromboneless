/** @file       Panels.cpp
 *  @author     Ryan McBride
 *  
 *  @brief      This file contains the function defintions for to create
 *              the necessary panels that may be used within the Tromboneless
 *              user interface.
 */

 /* Adding the necessary headers. */
 #include   "Panels.hpp"
 #include   "Widgets.hpp"
 #include   <tromboneless_data.hpp>

 /* Initialisation of class members.*/

void Panels::paint(juce::Graphics& g){
    juce::Rectangle <int> area = getLocalBounds();
    juce::Rectangle <float> outlineRectangle = area.toFloat();
    g.setColour(backgroundColour);
    g.fillRoundedRectangle(outlineRectangle , (float) 20);
}

void Panels::setBackgroundColour(juce::Colour colour){
    backgroundColour = colour;
    return;
}

juce::Colour Panels::getBackgroundColour(void){
    return backgroundColour;
}


/* ========================================================================================== */
/*                                                                                            */
/*                                   Drop Down Menus                                          */
/*                                                                                            */
/* ========================================================================================== */

 DropDownMenus::DropDownMenus(CoreWrapper& coreWrapper): coreWrapper_ref(coreWrapper){

    /* ============================== Creation of a dropdown menu that provides options for shift keying ============================== */
    
    juce::Component::addAndMakeVisible (shiftKeyChoice);
    shiftKeyChoice.ChangeLabelText("Transposition\nchoice: ");
    shiftKeyChoice.AddItem ("Bass", SKOpt_BASS);
    shiftKeyChoice.AddItem ("Piccolo", SKOpt_PICCOLO);
    shiftKeyChoice.AddItem ("Alto", SKOpt_ALTO);
    shiftKeyChoice.AddItem ("Tenor", SKOpt_TENOR);
    shiftKeyChoice.AddItem ("Contrabass", SKOpt_CONTRABASS);
    shiftKeyChoice.AddItem ("Soprano", SKOpt_SOPRANO);
    
    /* This line is the one responsible for calling the shiftKeyingUpdate function when the choice changes. */
    shiftKeyChoice.SetOnChange([this] { shiftKeyChoiceChanged(); });

    addAndMakeVisible(calibrateEmbouchure);
    return;
}

void DropDownMenus::shiftKeyChoiceChanged(){
    auto selectedOption =
        static_cast<ShiftKeyingOptions_t>(shiftKeyChoice.getSelectedId());

    trombonelessParameters.shiftKeyingOption = selectedOption;

    PitchMapper* mapper_ptr = coreWrapper_ref.getPitchMapper();
    mapper_ptr->SetTromboneType(selectedOption);

    return;
}

void DropDownMenus::resized(){
    auto area = getLocalBounds();
    juce::Rectangle <int> workingArea = area.withSizeKeepingCentre(area.getWidth()*0.95, area.getHeight()*0.8);

    juce::Rectangle <int> topDropDownBound = workingArea.withHeight(workingArea.getHeight()*0.4);
     
    shiftKeyChoice.setBounds (topDropDownBound);

    calibrateEmbouchure.setBounds (workingArea.removeFromBottom(workingArea.getHeight()*0.4));
    return;
}



Sliders::Sliders(CoreWrapper& coreWrapper): coreWrapper_ref(coreWrapper){
    addAndMakeVisible (distanceSlider);
    using juce::Slider;
    distanceSlider.slider.setRange(minimumDistance, maximumDistance, stepDistance);                 /* Setting the range to be between 5 and 60cm. */
    distanceSlider.setMinDifference(distanceRange);
    distanceSlider.slider.setTextValueSuffix (" cm");      /* Adds a unit at the end of the slider so the user knows what the value means. */
    distanceSlider.slider.setMinAndMaxValues (15.0, 45.0, juce::dontSendNotification);
    distanceSlider.slider.addListener (this);  
    distanceSlider.slider.setPopupDisplayEnabled(true, true, this, 1000);
    distanceSlider.slider.setNumDecimalPlacesToDisplay(1);
    distanceSlider.slider.setLookAndFeel(&LandF);

    /* Adding labels to the slider. */    
    distanceSlider.CreateLabel(SliderWithLabel::LabelPositions_t::UpperCentre, "Slider distance");
    distanceSlider.CreateLabel(SliderWithLabel::LabelPositions_t::LowerLeft, (juce::String)(((juce::String) minimumDistance) + (juce::String)" cm"));
    distanceSlider.CreateLabel(SliderWithLabel::LabelPositions_t::LowerRight, (juce::String)(((juce::String) maximumDistance) + (juce::String)" cm"));
    return;
}

Sliders::~Sliders(){
    distanceSlider.slider.removeListener(this);
    distanceSlider.slider.setLookAndFeel(nullptr);
    return;

}

void Sliders::resized(){
    juce::Rectangle <int> area = getLocalBounds();
    juce::Rectangle <int> workingArea = area.withSizeKeepingCentre(area.getWidth()*0.95, area.getHeight()*0.8);
    distanceSlider.setBounds(workingArea);
    return;

}


void Sliders::sliderValueChanged(juce::Slider* sliderChanged){
    if (sliderChanged == &distanceSlider.slider){
        /*  Update the calibrated distance of the slider in the main window. */
        trombonelessParameters.nearDistance = distanceSlider.slider.getMinValue();
        trombonelessParameters.farDistance = distanceSlider.slider.getMaxValue();

        PitchMapper* mapper_ptr = coreWrapper_ref.getPitchMapper();
        mapper_ptr->SetSlideMinLimit(static_cast<int>(distanceSlider.slider.getMinValue()*10));
        mapper_ptr->SetSlideMaxLimit(static_cast<int>(distanceSlider.slider.getMaxValue()*10));

        return;
    }
    return;
}

void Sliders::setMinimumDistance(float distance){
    minimumDistance = distance;
    return;
}

float Sliders::getMinimumDistance(void){
    return minimumDistance;
}

void Sliders::setMaximumDistance(float distance){
    maximumDistance = distance;
    return;
}

float Sliders::getMaximumDistance(void){
    return maximumDistance;
}

void Sliders::setStepDistance(float distance){
    stepDistance = distance;
    return;
}

float Sliders::getStepDistance(void){
    return stepDistance;
}

void Sliders::setDistanceRange(float range){
    distanceRange = range;
    return;
}

float Sliders::getDistanceRange(void){
    return distanceRange;
}

/* ========================================================================================== */
/*                                                                                            */
/*                                   EqualizerPanel                                           */
/*                                                                                            */
/* ========================================================================================== */

EqualizerPanel::EqualizerPanel(CoreWrapper& coreWrapper): coreWrapper_ref(coreWrapper){
    addAndMakeVisible(button);
    addAndMakeVisible(equalizer);
    addAndMakeVisible(buttonLabel);
    equalizer.setVisible(false);
    button.addListener(this);
    buttonLabel.setText((juce::String) "Enable equalizer", juce::dontSendNotification);

}

EqualizerPanel::~EqualizerPanel(){
    button.removeListener(this);
}

void EqualizerPanel::resized(){
    juce::Rectangle<int> area = getLocalBounds();
    area = area.withSizeKeepingCentre((float) (area.getWidth()*0.95), (float) area.getHeight()*0.95);
    if (true == synthesiserParameters.synthEnable){
        juce::Rectangle <int> buttonBounds = area.removeFromTop(std::min( (float) 20, (float) (area.getHeight()*0.1)));
        button.setBounds(buttonBounds.removeFromLeft(50));

        /* Adding the label next to the button. */
        buttonLabel.setBounds(buttonBounds.removeFromLeft(100));

        /* Allowing the rest of the space to be for the equalizer itself. */
        equalizer.setBounds(area);
    }

    else{
        button.setBounds(area);
    }


}

void EqualizerPanel::buttonClicked(juce::Button* buttonClicked)
{
    if (buttonClicked == &button){
        /*  Alternating between showing the equalizer and hiding it. */
        synthesiserParameters.synthEnable = !synthesiserParameters.synthEnable;
        equalizer.setVisible(synthesiserParameters.synthEnable);

        /*  Changing the displayed lbale depending upon on the visibility of the equalizer. */
        if (false == synthesiserParameters.synthEnable){
            buttonLabel.setText((juce::String) "Enable equalizer", juce::dontSendNotification);
        }
        else{
            buttonLabel.setText((juce::String) "Disable equalizer", juce::dontSendNotification);
        }
        
        /* Updating the parent component to redraw the screen. */
        if (auto* parent = getParentComponent()) parent->resized();
    }
}