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

 DropDownMenus::DropDownMenus(){

    /* ============================== Creation of a dropdown menu that provides options for shift keying ============================== */
    
    juce::Component::addAndMakeVisible (shiftKeyChoice);
    shiftKeyChoice.ChangeLabelText("Transposition choice: ");
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

    /* Scaling the label and combo box. */

    // auto comboBounds = 
    shiftKeyChoice.setBounds (area.withSizeKeepingCentre(area.getWidth()*0.95, area.getHeight()));
}