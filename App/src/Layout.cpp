/** @file       Layout.cpp
 *  @author     Ryan McBride
 *  @brief      This file holds the defintions of all functions and variables
 *              needed by the Tromboneless project to define the layout of the
 *              window which is being displayed to the end user.
 */

 /* Adding the necessary header files. */
#include "Layout.hpp"
#include "Widgets.hpp"
#include "Panels.hpp"

/* Adding the necessary libraries. */
#include <errno.h>
#include <unistd.h>
#include <stdexcept>
#include <string>
#include <system_error>

Layout::Layout()
{
    /* Sets the initial size of the window to be displayed to the user. */
    setSize (1200, 700);

    /* ============================== Creation of a dropdown menu that provides options for shift keying ============================== */
    addAndMakeVisible (dropDownMenus);

    /* ============================== Creation of slider for the maximum and minimum ranges. ============================== */
    addAndMakeVisible (sliders);

    /* ============================== Creation of slider for adjusting maximum and minimum pressure amplitude. ============================== */
    addAndMakeVisible (pressureSlider);

    /* ============================== Creation of slider for adjusting gains for each frequency band. ============================== */
    addAndMakeVisible (equalizer);
}


void Layout::paint (juce::Graphics& g)
{
    /* Sets the background to be the same as the users settings for other windows. */
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    
    /* Setting the size of the font that will be written. */
    g.setFont (juce::FontOptions (18.0f));
    
    /* Setting the colour of the text that will be written. */
    g.setColour (juce::Colours::white);
    
    /* Adds a banner at the top of the window centred with the name of the device. */
    g.drawText ("Tromboneless Parameters", getLocalBounds(), juce::Justification::centredTop, true);
    
}

//  Window resizing configuration

void Layout::resized()    
{
    auto area = getLocalBounds();
    /* Adding a margin around the edge of the window.*/
    area = area.withSizeKeepingCentre(area.getWidth()*0.94, area.getHeight()*0.94);
    
    auto SliderBounds = area.removeFromTop(300);
    
    /* Adding the sliders to the window. */
    auto distanceSliderBounds = SliderBounds.removeFromLeft(SliderBounds.getWidth()*0.65);
    sliders.setBounds(distanceSliderBounds.removeFromBottom(120));
    
    /* Adding a margin between the drop down menus and the slider. */
    distanceSliderBounds.removeFromBottom(20);

    /* Pressure slider bounds */
    auto pressureSliderBounds = SliderBounds.removeFromRight(area.getWidth()*0.25);
    pressureSlider.setBounds (pressureSliderBounds);
    
    /* Setting the bounds of the drop down menus.*/
    auto comboBounds = distanceSliderBounds.removeFromTop (150);
    dropDownMenus.setBounds (comboBounds);

    /* Adding a margin between the equalizer and the sliders. */
    area.removeFromTop(20);

    /* Equalizer */
    /* Only allowing the synthethiser to be viewed if it is enabled. */
    if(true == synthesiserParameters.synthEnable){
        // auto equalizerBounds = area.removeFromTop(350);
        equalizer.setBounds(area);
    }
    else{
        juce::Rectangle <int> equalizerBounds = area.removeFromTop(50);
        equalizer.setBounds(equalizerBounds);
    }


}

