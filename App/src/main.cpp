/* This file is specifically for the creation, functions and closing
    of the appliation, not any widgets present within the window. */

#include "Layout.h"
#include <juce_gui_extra/juce_gui_extra.h>
#include "tromboneless_data.h"


//==============================================================================

/**  @brief Creation of main application class
     @note  This is called upon when the code is run for the first time.
*/
class GuiAppApplication final : public juce::JUCEApplication

{
public:
    //==============================================================================
    GuiAppApplication() {}

    // We inject these as compile definitions from the CMakeLists.txt
    // If you've enabled the juce header with `juce_generate_juce_header(<thisTarget>)`
    // you could `#include <JuceHeader.h>` and use `ProjectInfo::projectName` etc. instead.
    const juce::String getApplicationName() override       { return JUCE_APPLICATION_NAME_STRING; }
    const juce::String getApplicationVersion() override    { return JUCE_APPLICATION_VERSION_STRING; }
    bool moreThanOneInstanceAllowed() override             { return false; }

    //==============================================================================
    void initialise (const juce::String& commandLine) override
    {
        // This method is where you should put your application's initialisation code..
        juce::ignoreUnused (commandLine);

        mainWindow.reset (new MainWindow (getApplicationName()));
    }

    /** @brief Function used to control what happens when the user wants to close the application
     *  @note Currently nothing extra is happening here other than closing the window
     */
    void shutdown() override
    {
        // Add your application's shutdown code here..

        mainWindow = nullptr; // (deletes our window)
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        // This is called when the app is being asked to quit: you can ignore this
        // request and let the app carry on running, or call quit() to allow the app to close.
        quit();
    }

    /** @brief Handler for when the user tries to open mutliple instances of the application
        @note This function will return the command line parameters passed for the new instance were and may be used within the current window.
    */
    void anotherInstanceStarted (const juce::String& commandLine) override
    {
        juce::ignoreUnused (commandLine);
    }

    //==============================================================================
    /** @brief This class implements the desktop window that contains an instance of the MainComponent class.
        @note Inherits the juce::DocumentWindow class and functions.
    */
    class MainWindow final : public juce::DocumentWindow
    {
    public:

        // Generation fo what the main window will look like
        explicit MainWindow (juce::String name)
            : DocumentWindow (name,
                              juce::Desktop::getInstance().getDefaultLookAndFeel()
                                                          .findColour (backgroundColourId),
                              allButtons)
        {
            setUsingNativeTitleBar (true);
            setContentOwned (new Layout(), true);

           #if JUCE_IOS || JUCE_ANDROID
            setFullScreen (true);
           #else
            setResizable (true, true);
            centreWithSize (getWidth(), getHeight());
           #endif

            setVisible (true);
        }

        /** @brief function handler for when the user tries to close the program
            @note Currently does not do anything other than request the system to close the application.
        */
        void closeButtonPressed() override
        {
            // This is called when the user tries to close this window. Here, we'll just
            // ask the app to quit when this happens, but you can change this to do
            // whatever you need.
#ifdef DBG_ON_CLOSE
            std::cout << "Gain values: ";

            for (int i=0; i< 10; i++){
                std::cout << synthesiserParameters.gains[i] << "   ";
            }
            std::cout << "\n";
#endif
            getInstance()->systemRequestedQuit();
        }

        /* Note: Be careful if you override any DocumentWindow methods - the base
           class uses a lot of them, so by overriding you might break its functionality.
           It's best to do all your work in your content component instead, but if
           you really have to override any DocumentWindow methods, make sure your
           subclass also calls the superclass's method.
        */

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    std::unique_ptr<MainWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (GuiAppApplication)