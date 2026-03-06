#include "profiler.h"

#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#include <stdexcept>
#include <string>
#include <system_error>

Profiler::Profiler()
{
    setSize (600, 400);

    addAndMakeVisible (terminal);
    terminal.setMultiLine (true);
    terminal.setReturnKeyStartsNewLine (true);
    terminal.setReadOnly (true);
    terminal.setScrollbarsShown (true);
    terminal.setCaretVisible (false);
    terminal.setPopupMenuEnabled (true);
    terminal.setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x32ffffff));
    terminal.setColour (juce::TextEditor::outlineColourId, juce::Colour (0x171715));
    terminal.setColour (juce::TextEditor::shadowColourId, juce::Colour (0x16000000));

    addAndMakeVisible (imageButton);
    
    // Setup module dropdown
    addAndMakeVisible (module_label);
    module_label.setText ("Module:", juce::dontSendNotification);
    module_label.setJustificationType (juce::Justification::centredRight);
    
    addAndMakeVisible (moduleComboBox);
    moduleComboBox.addItem ("2f", 1);
    moduleComboBox.addItem ("eas", 2);
    moduleComboBox.addItem ("ceis", 3);
    moduleComboBox.setSelectedId (1);

    juce::File imageFile = juce::File::getCurrentWorkingDirectory().getChildFile("resources/circle_gold.png");
    if (imageFile.existsAsFile())
    {
        auto buttonImage = juce::ImageCache::getFromFile (imageFile);
        if (buttonImage.isValid())
        {
            imageButton.setImages (false, true, true,
                                   buttonImage, 1.0f, juce::Colours::transparentBlack,  // normal
                                   buttonImage, 0.8f, juce::Colours::transparentBlack,  // over (hover)
                                   buttonImage, 0.6f, juce::Colours::transparentBlack); // down (pressed)
        }
    }

    // imageButton.onClick = ;

}

Profiler::~Profiler()
{
    return;
}



/** @brief functions that sets the background colour of the window
    @param  g The panel which should be divided
    @retval void
    @note Always sets the background to getLookAndFeel().findColour
    */
void Profiler::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setFont (juce::FontOptions (18.0f));
    g.setColour (juce::Colours::white);
    g.drawText ("HELLO WORLD", getLocalBounds(), juce::Justification::centredTop, true);
}

//  Window resizing configuration

/** @brief Function that changes the size of the profiler
    @retval no return value: void
    @note Resizes the window to be 80 by 20
*/
void Profiler::resized()    
{
    auto area = getLocalBounds();

    auto buttonArea = area.removeFromTop (60);
    imageButton.setBounds (buttonArea.reduced (10).removeFromRight (100));
    
    // // Layout serial port selector
    // auto serialArea = buttonArea.reduced (10);
    // module_label.setBounds (serialArea.removeFromLeft (100));
    // moduleComboBox.setBounds (serialArea.removeFromLeft (200));

    auto labelBounds = buttonArea.removeFromLeft (60);
    auto comboBounds = buttonArea.removeFromLeft (80);
    comboBounds = comboBounds.withSizeKeepingCentre (80, 20);  // width, height
    labelBounds = labelBounds.withSizeKeepingCentre (60, 20);
    module_label.setBounds (labelBounds);
    moduleComboBox.setBounds (comboBounds);

    terminal.setBounds (area.reduced (8));
}

/** @brief Function that is triggered when a key is pressed
    @param key An instance of KeyPress that stores the value of the key being pressed
    @retval boolean value of true if a key is pressed, otherwise false
 */
bool Profiler::keyPressed(const juce::KeyPress& key)
{
    if (key == juce::KeyPress::spaceKey)
    {
        // imageButtonClicked();  // Trigger the same action as clicking the button
        return true;  // Key was handled
    }
    return false;  // Key was not handled
}