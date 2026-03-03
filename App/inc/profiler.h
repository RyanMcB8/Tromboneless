#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include <stdexcept>
#include <string>
=// #include shared header


class Profiler final : public juce::Component
{
    public:

        Profiler();
        ~Profiler() override;

        void paint (juce::Graphics&) override;

        void resized() override;

        void logMessage(const juce::String& m)
        {
            terminal.moveCaretToEnd();
            terminal.insertTextAtCaret(m + juce::newLine);
        }
        
        bool keyPressed(const juce::KeyPress& key) override;
        

    private:

        juce::TextEditor terminal;

        juce::ImageButton imageButton;
        
        juce::ComboBox moduleComboBox;

        juce::Label module_label;

        void imageButtonClicked();
        void select_module();
        void showMessageBox (const juce::String& title, const juce::String& message)
        {
            juce::AlertWindow::showMessageBoxAsync(
                juce::MessageBoxIconType::InfoIcon,
                title,
                message
            );
        }

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Profiler)
};

