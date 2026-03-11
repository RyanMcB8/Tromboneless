# Notes on App

## This will contain a large amount of tutorial code for the use of certain classes within JUCE and will be removed for the alpha release once the initial template is complete. 

## Map of the code
Within the `main.cpp` file is the code responsible for creating main window.

The code within `Layout.cpp` creates the custom layout for the window such as setting the inital size and adding all widgets and text to be displayed to the user.

The `Widgets.cpp` file hosts the classes for creating more custom widgets that base library of JUCE does not already provide. These include modifications to shapes of the widgets.

The `tromboneless_data.cpp` file has the functions responsible for calling upon the objects within the core functionality and modifying their values to match that of what the UX is displaying.

Each of these files has an associated header file, excluding `main.cpp`, which hosts the declarations and custom types which may be used externally.


## Code tutorials

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