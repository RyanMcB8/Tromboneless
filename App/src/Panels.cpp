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

 /* Initialisation of class members.*/

void Panels::paint(juce::Graphics& g){
    juce::Rectangle <int> area = getLocalBounds();
    juce::Rectangle <float> outlineRectangle = area.toFloat();
    g.setColour(edgeColour);
    g.fillRoundedRectangle(outlineRectangle , (float) 20);
}




/* ========================================================================================== */
/*                                                                                            */
/*                                   Drop Down Menus                                          */
/*                                                                                            */
/* ========================================================================================== */

 DropDownMenus::DropDownMenus(){

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
    shiftKeyChoice.OnChange (&trombonelessParameters.shiftKeyingOption);

    addAndMakeVisible(calibrateEmbouchure);

}

void DropDownMenus::resized(){
    auto area = getLocalBounds();
    juce::Rectangle <int> workingArea = area.withSizeKeepingCentre(area.getWidth()*0.95, area.getHeight()*0.8);

    juce::Rectangle <int> topDropDownBound = workingArea.withHeight(workingArea.getHeight()*0.4);
     
    shiftKeyChoice.setBounds (topDropDownBound);

    calibrateEmbouchure.setBounds (workingArea.removeFromBottom(workingArea.getHeight()*0.4));
}



Sliders::Sliders(){
    addAndMakeVisible (distanceSlider);
    using juce::Slider;
    distanceSlider.slider.setRange(minimumDistance, maximumDistance, stepDistance);                 /* Setting the range to be between 5 and 60cm. */
    distanceSlider.setMinDifference(distanceRange);
    distanceSlider.slider.setTextValueSuffix (" cm");      /* Adds a unit at the end of the slider so the user knows what the value means. */
    distanceSlider.slider.setMinAndMaxValues (15.0, 45.0, juce::dontSendNotification);
    distanceSlider.slider. addListener (this);  
    distanceSlider.slider.setPopupDisplayEnabled(true, true, this, 1000);
    distanceSlider.slider.setNumDecimalPlacesToDisplay(1);
    distanceSlider.slider.setLookAndFeel(&LandF);

    /* Adding labels to the slider. */    
    distanceSlider.CreateLabel(SliderWithLabel::LabelPositions_t::UpperCentre, "Slider distance");
    distanceSlider.CreateLabel(SliderWithLabel::LabelPositions_t::LowerLeft, (juce::String)(((juce::String) minimumDistance) + (juce::String)" cm"));
    distanceSlider.CreateLabel(SliderWithLabel::LabelPositions_t::LowerRight, (juce::String)(((juce::String) maximumDistance) + (juce::String)" cm"));

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

}


/* ========================================================================================== */
/*                                                                                            */
/*                                   EqualizerPanel                                           */
/*                                                                                            */
/* ========================================================================================== */

EqualizerPanel::EqualizerPanel(){
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
        synthesiserParameters.synthEnable = !synthesiserParameters.synthEnable;
        equalizer.setVisible(synthesiserParameters.synthEnable);

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