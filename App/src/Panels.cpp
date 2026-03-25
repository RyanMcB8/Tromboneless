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

/* ========================================================================================== */
/*                                                                                            */
/*                                   Drop Down Menus                                          */
/*                                                                                            */
/* ========================================================================================== */

 DropDownMenus::DropDownMenus(){

    /* ============================== Creation of a dropdown menu that provides options for shift keying ============================== */
    
    juce::Component::addAndMakeVisible (shiftKeyChoice);
    shiftKeyChoice.ChangeLabelText("Transposition\nchoice: ");
    shiftKeyChoice.AddItem ("Middle F4", SKOpt_MiddleF4);
    shiftKeyChoice.AddItem ("B sharp 4", SKOpt_BSharp4);
    shiftKeyChoice.AddItem ("D5", SKOpt_D5);
    shiftKeyChoice.AddItem ("F5", SKOpt_F5);
    shiftKeyChoice.AddItem ("A sharp 4", SKOpt_ASharp4);
    
    /* This line is the one responsible for calling the shiftKeyingUpdate function when the choice changes. */
    shiftKeyChoice.OnChange (&trombonelessParameters.shiftKeyingOption);

}

void DropDownMenus::resized(){
    auto area = getLocalBounds();
     
    shiftKeyChoice.setBounds (area);
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