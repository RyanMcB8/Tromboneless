/** @file       Panels.cpp
 *  @author     Ryan McBride
 *  
 *  @brief      This file contains the function defintions for to create
 *              the necessary panels that may be used within the Tromboneless
 *              user interface.
 */

 /* Adding the necessary headers. */
 #include   "Panels.hpp"


 /* Initialisation of class members.*/

 DropDownMenus::DropDownMenus(){

    /* ============================== Creation of a dropdown menu that provides options for shift keying ============================== */
    
    addAndMakeVisible (shiftKeyChoice);
    shiftKeyChoice.addItem ("Middle F4", SKOpt_MiddleF4);
    shiftKeyChoice.addItem ("B sharp 4", SKOpt_BSharp4);
    shiftKeyChoice.addItem ("D5", SKOpt_D5);
    shiftKeyChoice.addItem ("F5", SKOpt_F5);
    shiftKeyChoice.addItem ("A sharp 4", SKOpt_ASharp4);
    
    /* This line is the one responsible for calling the shiftKeyingUpdate function when the choice changes. */
    shiftKeyChoice.onChange = [this] {trombonelessParameters.shiftKeyingOption = (ShiftKeyingOptions_t) shiftKeyChoice.getSelectedId(); };
    shiftKeyChoice.setSelectedId (0);

    /* Adding a label to be beside the shift key dropdown menu. */
    addAndMakeVisible (shiftKeySelectLabel);
    shiftKeySelectLabel.setText ("Transposition selector:", juce::dontSendNotification);
    shiftKeySelectLabel.setJustificationType (juce::Justification::centredRight);
    shiftKeySelectLabel.attachToComponent(&shiftKeyChoice, true);

 }

DropDownMenus::~DropDownMenus(){

}

void DropDownMenus::resized(){
    auto area = getLocalBounds();
    auto comboBounds = area.removeFromTop (10);

    /* Scaling the label and combo box. */
    comboBounds = comboBounds.withSizeKeepingCentre (200, 40);
    shiftKeyChoice.setBounds (comboBounds);
}