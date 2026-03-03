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
    moduleComboBox.onChange = [this] { select_module(); };

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

    imageButton.onClick = [this] { imageButtonClicked(); };

    serial_init();
    select_module();

}

Profiler::~Profiler()
{
    // Close serial port if it was opened
    if (this->serial_port != -1) {
        close(this->serial_port);
        this->serial_port = -1;
    }
}

void Profiler::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    //g.setFont (juce::FontOptions (18.0f));
    //g.setColour (juce::Colours::white);
    //g.drawText ("amhraic", getLocalBounds(), juce::Justification::centredTop, true);
}

//  Window resizing configuration
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

bool Profiler::keyPressed(const juce::KeyPress& key)
{
    if (key == juce::KeyPress::spaceKey)
    {
        imageButtonClicked();  // Trigger the same action as clicking the button
        return true;  // Key was handled
    }
    return false;  // Key was not handled
}

void Profiler::imageButtonClicked()
{
   switch(current_module) 
   {
        case module_id::two_f:
            two_f.profile(*this);  // Pass reference to this
            break;
        case module_id::eas:
             eas.profile(*this);    // Pass reference to this
             break;
        // case module_id::ceis:
        //     ceis.profile();   // Pass reference to this
        //     break;
        /*
        case module_id::yourmodule
            yourmodule.profile();
            break                  // Pass reference to this
        */  
   }
}
   

           








void Profiler::serial_init()
{
    this->serial_port = open("/dev/cu.usbmodem1728391", O_RDWR);    // Open amhraic (usbmodem1728391) in read/write mode

    if (this->serial_port < 0)
    {
        throw std::runtime_error("Failed to open serial port /dev/cu.usbmodem1728391 " + std::string(strerror(errno)));
    }

    if(tcgetattr(this->serial_port, &this->tty) != 0)
    {
        close(this->serial_port);
        throw std::runtime_error("Failed to get terminal attributes: " + std::string(strerror(errno)));
    }

    this->tty.c_cflag &= static_cast<tcflag_t>(~PARENB); // Clear parity bit, disabling parity (most common)
    this->tty.c_cflag &= static_cast<tcflag_t>(~CSTOPB); // Clear stop field, only one stop bit used in communication (most common)
    this->tty.c_cflag &= static_cast<tcflag_t>(~CSIZE); // Clear all bits that set the data size
    this->tty.c_cflag |= CS8; // 8 bits per byte (most common)
    this->tty.c_cflag &= static_cast<tcflag_t>(~CRTSCTS); // Disable RTS/CTS hardware flow control (most common)
    this->tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    this->tty.c_lflag &= static_cast<tcflag_t>(~ICANON);
    this->tty.c_lflag &= static_cast<tcflag_t>(~ECHO); // Disable echo
    this->tty.c_lflag &= static_cast<tcflag_t>(~ECHOE); // Disable erasure
    this->tty.c_lflag &= static_cast<tcflag_t>(~ECHONL); // Disable new-line echo
    this->tty.c_lflag &= static_cast<tcflag_t>(~ISIG); // Disable interpretation of INTR, QUIT and SUSP
    this->tty.c_iflag &= static_cast<tcflag_t>(~(IXON | IXOFF | IXANY)); // Turn off s/w flow ctrl
    this->tty.c_iflag &= static_cast<tcflag_t>(~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL)); // Disable any special handling of received bytes

    this->tty.c_oflag &= static_cast<tcflag_t>(~OPOST); // Prevent special interpretation of output bytes (e.g. newline chars)
    this->tty.c_oflag &= static_cast<tcflag_t>(~ONLCR); // Prevent conversion of newline to carriage return/line feed
    // this->tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
    // this->tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

    this->tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    this->tty.c_cc[VMIN] = 0;

    cfsetispeed(&this->tty, B9600);
    cfsetospeed(&this->tty, B9600);


    if (tcsetattr(this->serial_port, TCSANOW, &this->tty) != 0)
    {
        close(this->serial_port); // Clean up before throwing
        throw std::runtime_error("Failed to set terminal attributes: " + std::string(strerror(errno)));
    }

    this->logMessage("Serial port initialised successfully");
}

void Profiler::select_module()
{
    switch(moduleComboBox.getSelectedId())
    {
        case 1:
            logMessage ("Selected module: 2[f]");
            current_module = module_id::two_f;
            break;
        case 2:
            logMessage ("Selected module: eas");
            current_module = module_id::eas;
            break;
        case 3:
            logMessage ("Selected module: ceis");
            current_module = module_id::ceis;
            break;
        /* case n:
            logMessage ("Selected module: Your Module");
            current_module = module_id::your_module;
            */
        default:
            logMessage ("Unknown module selected.");
            break;
    }
}

void Profiler::serial_write(amhraic::Command cmd)
{
    if (this->serial_port == -1)
    {
        throw std::runtime_error("Serial port not initialized");
    }

    // Flush input buffer before writing to clear any stale data

    ssize_t bytes_written = write(this->serial_port, &cmd, sizeof(cmd));
    if (bytes_written < 0)
    {
        throw std::runtime_error("Failed to write to serial port: " + std::string(strerror(errno)));
    }
    
    // Small delay to ensure command is processed
    usleep(5000); // 5ms delay
}

void Profiler::serial_drain()
{
    if (this->serial_port == -1)
    {
        throw std::runtime_error("Serial port not initialized");
    }

    std::array<uint8_t, 256> buffer;
    while (1)
    {
        const auto n = read(this->serial_port, buffer.data(), buffer.size());
        if (n <= 0)
            break;
    }
}

void Profiler::serial_read()
{
    if (this->serial_port == -1)
    {
        throw std::runtime_error("Serial port not initialized");

    }
    
    // Small delay to ensure firmware has time to respond
    usleep(10000); // 50ms delay

    amhraic::Results result;

    while(1)
    {
        ssize_t bytes_read = read(this->serial_port, &result, sizeof(result));
        if (bytes_read < 0)
        {
            throw std::runtime_error("Failed to read from serial port: " + std::string(strerror(errno)));
        }
        else if (bytes_read == 0)
        {
            throw std::runtime_error("No data available to read from serial port");
        }
        else if (static_cast<size_t>(bytes_read) < sizeof(result))
        {
            throw std::runtime_error("Incomplete data read from serial port");
        }
        else
        {
            break; // Successfully read full result
        }
    }

    this->tester_result = result;
}