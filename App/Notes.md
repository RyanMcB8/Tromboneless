# Notes on App

## Map of the code
Within the `main.cpp` file is the code responsible for creating main window. This should not need to be modified as the contents of the window are defined elsewhere.

The code within `Layout.cpp` creates the defines the general structure of the window by creating instances of widgets and panels and arranging them to be in the correct position on the screen.

The `Panels.cpp` file is where each panel on the display is created. This sets the backgorund colour of each panel and calls upon the specific widgets which should be in each panel.

The `Widgets.cpp` file hosts the classes for creating more custom widgets that base library of JUCE does not already provide. These include modifications to shapes of the widgets such as the custom calibration sliders and equalizer.

The `tromboneless_data.cpp` file has the variables which interact with the Tromboneless Core functionality and allow for the Core and App to communicate parameters between each other. If there is a need to add more communication channels between the app and core, this is where t should be added. For example, a flag for if a speaker has been connected.

Each of these files has an associated header file, excluding `main.cpp`, which hosts the declarations and custom types which may be used externally. All documentation for the functions are within the header files with more in-depth step by step explanations within the cpp files.

If the files are thought about as a pipe line, the order would be:
JUCE --> Widgets --> Panels --> Layout --> main

## Code tutorials specifically for JUCE:

### For labels:

This code will display only uppercase characters input.
```
  addAndMakeVisible (uppercaseLabel);
  uppercaseLabel.setText ("Uppercase:", juce::dontSendNotification);
  uppercaseLabel.attachToComponent (&uppercaseText, true);
  uppercaseLabel.setColour (juce::Label::textColourId, juce::Colours::orange);
  uppercaseLabel.setJustificationType (juce::Justification::right);
  addAndMakeVisible (uppercaseText);
  uppercaseText.setColour (juce::Label::backgroundColourId, juce::Colours::darkblue);
```

These functions will allow for the text to be editted by the user when the app is open.
``` 
 addAndMakeVisible (inputText);
 inputText.setEditable (true);
 inputText.setColour (juce::Label::backgroundColourId, juce::Colours::darkblue);
 inputText.onTextChange = [this] { uppercaseText.setText (inputText.getText().toUpperCase(), juce::dontSendNotification); };
 ```
When this label is changed by the user, the `onTextChange` sets a lambda function to be excucted whenever there is any change within the label.


## For drop down menus
The drop down menus within the JUCE library are referred to as a `comboBox`.

The lines of code below display an example which first creates an instance of a combo-box called 'styleMenu' and adds optionsusing the `addItem` method. When the choice is changed, the `styleMenuChanged()` function is called upon. The first option within the list is set as the default to be selected.
```
  addAndMakeVisible (styleMenu);
  styleMenu.addItem ("Plain", 1);
  styleMenu.addItem ("Bold", 2);
  styleMenu.addItem ("Italic", 3);
  styleMenu.onChange = [this] { styleMenuChanged(); };
  styleMenu.setSelectedId (1);
  setSize (400, 200);
```
