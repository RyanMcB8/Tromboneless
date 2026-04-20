/**
 * @file cap_sensor.cpp
 * @author Ben Allen
 * @brief                   Implementation of CAP11888 driver for Tromboneless
 * @version 0.1
 * @date 2026-04-20
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "drivers/cap_sensor.hpp"
#include <chrono>
using namespace std::chrono_literals;

// Helper Class for 1 sensor channel
CAP1188Channel::CAP1188Channel() : cap1188_(nullptr),pin_(-1){};
CAP1188Channel::~CAP1188Channel(){};

void CAP1188Channel::initialise(CAP1188* cap1188, int pin){
    cap1188_ = cap1188;
    // REMEMBER!! Pin is actually 0-7 here, even though channels
    // are numbered C1 to C8 on physical board
    pin_ = pin;
}

bool CAP1188Channel::isPinTouched(){
    uint8_t touchedPins = cap1188_->touched();
    return touchedPins & (1 << pin_);
}

int8_t CAP1188Channel::pinDelta(){
    return cap1188_->deltaCount(pin_);
}

int CAP1188Channel::getThreshold(){
    std::array<uint8_t, 8> allThresholds = cap1188_->getThresholds();
    return allThresholds[pin_];
}

void CAP1188Channel::setThreshold(int threshold){
    // Get all the current thresholds
    std::array<uint8_t, 8> allThresholds = cap1188_->getThresholds();
    // Edit only our pin's threshold
    allThresholds[pin_] = threshold;
    // Set all thresholds.
    // Only our pin is changed, but the function requires values for all.
    cap1188_->setThresholds(allThresholds);
}

void CAP1188Channel::pinRecalibrate(){
    // Make a pin mask (b7b6...b0) with just our pin
    uint8_t pinMask = 1 << pin_;
    // This will only recalibrate pin on pinMask
    cap1188_->recalibratePins(pinMask);
}

// Driver Base Functions
// Constructor
CAP1188::CAP1188(I2CBus& bus,
        uint8_t address,
        const std::string& gpioChipPath,
        unsigned int gpioLine,
        uint8_t enabledChannels,
        uint8_t interruptEnabledChannels)
    :   bus_(bus),
        address_(address),
        gpioChipPath_(gpioChipPath),
        gpioLine_(gpioLine),
        enabledChannels_(enabledChannels),
        interruptEnabledChannels_(interruptEnabledChannels){}

// Destructor
CAP1188::~CAP1188()
{
    stop();
}

bool CAP1188::initialise()
{
    if (!bus_.openBus())
    {
        printf("Failed to open bus\n");
        return false;
    }

    // Check we have the right device
    uint8_t mid = 0;
    bus_.readBlock8(address_, CAP1188_REG_MANUFACTURER_ID, &mid, 1);
    if (mid != CAP1188_MANUFACTURER_ID_VALUE)
    {
        printf("Wrong manufacturer ID: got 0x%02X, expected 0x%02X\n", mid, CAP1188_MANUFACTURER_ID_VALUE);
        return false;
    }

    uint8_t pid = 0;
    bus_.readBlock8(address_, CAP1188_REG_PRODUCT_ID, &pid, 1);
    if (pid != CAP1188_PRODUCT_ID_VALUE)
    {
        printf("Product ID not found/matching\n");
        return false;
    }
    // Create Instances of Channel CLasses
    for (int i=0; i<8; i++)
        channels[i].initialise(this,i);
    
    // Enable/Disable Channels
    enableChannels();
    enableInterruptChannels();
    linkLEDs(enabledChannels_);
    // Set averaging window, sample time, cycle time
    setAveraging(8);
    setSampleTime("1.28ms");
    setCycleTime("35ms");
    // Configure data ready condition
    // The threshold is 0x30 = 48 decimal
    channels[0].setThreshold(0x30);
    // 0x00 = 35ms
    setInterruptRepeatRate(0x00);
    // 0x05 = 101b = 4x 
    uint8_t sensitivity = 0x05;
    setSensitivity(sensitivity);

    disableStandby();
    clearInterrupt();

    printf("Initialisation Complete\n");
    return true;
}

void CAP1188::start(){

    // Configure GPIO line for input + rising edge detection
    gpiod::line_config lineCfg;
    lineCfg.add_line_settings(
        gpioLine_,
        gpiod::line_settings()
            .set_direction(gpiod::line::direction::INPUT)
            .set_edge_detection(gpiod::line::edge::FALLING)
    );
    chip_ = std::make_shared<gpiod::chip>(gpioChipPath_);

    auto builder = chip_->prepare_request();
    builder.set_consumer("cap_sensor");
    builder.set_line_config(lineCfg);

    request_ = std::make_shared<gpiod::line_request>(builder.do_request());

    // Launch worker thread (blocking)
    running_ = true;
    thread_ = std::thread(&CAP1188::worker, this);
}

void CAP1188::stop(){
    if (!running_)
        return;

    running_ = false;

    // Wait for thread to finish
    if (thread_.joinable())
        thread_.join();

    // Release GPIO resources
    if (request_)
        request_->release();

    if (chip_)
        chip_->close();

}

void CAP1188::worker(){
    while (running_)
    {
        // BLOCKING call — sleeps until GPIO edge event occurs
        bool ready = request_->wait_edge_events(std::chrono::milliseconds(1000));

        if (!running_)
            break;

        if (ready)
        {
            // Consume event (required by libgpiod)
            gpiod::edge_event_buffer buffer;
            request_->read_edge_events(buffer, 1);

            // Handle new measurement
            handleDataReady();
        }
    }
}
void CAP1188::handleDataReady()
{
    // Read delta on Channel 0 ie CS1
    uint8_t delta = channels[0].pinDelta();
    clearInterrupt();

    // Publish to subscriber if registered
    if (cap1188CallbackInterface_)
        cap1188CallbackInterface_(delta);
}
// Callback
void CAP1188::registerCallback(CAP1188CallbackInterface cb)
{
    cap1188CallbackInterface_ = cb;
}

int CAP1188::touched(){
    //Clear the INT bit and any previously touched pins
    uint8_t current = 0;
    bus_.readBlock8(address_, CAP1188_REG_MAIN_CTRL, &current, 1);
    current = current & ~0x01;
    bus_.writeBlock8(address_, CAP1188_REG_MAIN_CTRL, &current, 1);
    //Now read the sensor status
    uint8_t touchVals = 0;
    bus_.readBlock8(address_, CAP1188_REG_SENSOR_STATUS, &touchVals, 1);
    return touchVals;
}
void CAP1188::linkLEDs(uint8_t pinmask)
    {
        bus_.writeBlock8(address_, CAP1188_REG_LED_LINKING,
        &pinmask, 1);
    }

// Sensitivity getter and setter
uint8_t CAP1188::getSensitivity()
{
    uint8_t sensitivity = 0;
    bus_.readBlock8(address_, CAP1188_REG_SENSITIVITY, &sensitivity, 1);
    return sensitivity;
}
void CAP1188::setSensitivity(uint8_t sensitivityVal)
{
    // Bits 6-4 control the sensitivity so a shift is needed
    sensitivityVal <<= 4;
    // Get what's in the register
    uint8_t current = CAP1188::getSensitivity();
    // Clear bits 6-4 and preserve the others
    current &= 0x8F;
    uint8_t newVal = current | sensitivityVal;
    bus_.writeBlock8(address_, CAP1188_REG_SENSITIVITY, &newVal, 1);
}

uint8_t CAP1188::getAveraging()
{
    uint8_t samplesPerAverageIx = 0;
    bus_.readBlock8(address_, CAP1188_REG_AVG_SAMP_RATE, &samplesPerAverageIx, 1);
    //Shift from B6-B4 to B2-B0
    samplesPerAverageIx >>= 4;
    //Preserve only B2-B0
    samplesPerAverageIx &= 0x07;

    return CAP1188_AVERAGING_OPTIONS[samplesPerAverageIx];
}

void CAP1188::setAveraging(uint8_t averagingVal)
{
    auto it = std::find(CAP1188_AVERAGING_OPTIONS.begin(), CAP1188_AVERAGING_OPTIONS.end(), averagingVal);
    if (it == CAP1188_AVERAGING_OPTIONS.end())
        throw std::out_of_range("Samples per average must be 1,2,4,8,16,32,64 or 128");

    uint8_t reg = 0;
    bus_.readBlock8(address_, CAP1188_REG_AVG_SAMP_RATE, &reg, 1);

    // Drop only B6-B4 (set to zero)
    reg &= 0x8F;

    // Get index and shift from B2-B0 to B6-B4
    uint8_t avg = std::distance(CAP1188_AVERAGING_OPTIONS.begin(), it) << 4;

    // Add the new average to the register word
    avg |= reg;

    // Write to register
    bus_.writeBlock8(address_, CAP1188_REG_AVG_SAMP_RATE, &avg, 1);
}

std::string CAP1188::getSampleTime()
{
    uint8_t reg = 0;
    bus_.readBlock8(address_, CAP1188_REG_AVG_SAMP_RATE, &reg, 1);
    reg >>= 2;
    reg &= 0x03;
    return CAP1188_SAMP_TIME_OPTIONS[reg];
}

void CAP1188::setSampleTime(std::string newSampleTime)
{
    auto it = std::find(CAP1188_SAMP_TIME_OPTIONS.begin(), CAP1188_SAMP_TIME_OPTIONS.end(), newSampleTime);
    if (it == CAP1188_SAMP_TIME_OPTIONS.end())
        throw std::out_of_range("Time per sample must be 320us ,640us, 1.28ms, or 2.56ms");

    uint8_t reg = 0;
    bus_.readBlock8(address_, CAP1188_REG_AVG_SAMP_RATE, &reg, 1);
    // Drop only B3-B2
    reg &= 0xF3;
    // Get index and shift from B1-B0 to B3-B2
    uint8_t timeIx = std::distance(CAP1188_SAMP_TIME_OPTIONS.begin(), it) << 2;
    uint8_t newReg = reg | timeIx;
    bus_.writeBlock8(address_, CAP1188_REG_AVG_SAMP_RATE, &newReg, 1);
}

std::string CAP1188::getCycleTime()
{
    uint8_t reg = 0;
    bus_.readBlock8(address_, CAP1188_REG_AVG_SAMP_RATE, &reg, 1);
    // Preserve only B1-B0
    uint8_t cycleTimeIx = reg & 0x03;
    return CAP1188_CYCLE_TIME_OPTIONS[cycleTimeIx];
}
void CAP1188::setCycleTime(std::string newCycleTime)
{
    auto it = std::find(CAP1188_CYCLE_TIME_OPTIONS.begin(), CAP1188_CYCLE_TIME_OPTIONS.end(), newCycleTime);
    if (it == CAP1188_CYCLE_TIME_OPTIONS.end())
        throw std::out_of_range("Time per sample must be 35ms ,70ms, 105ms, or 140ms");

    uint8_t reg = 0;
    bus_.readBlock8(address_, CAP1188_REG_AVG_SAMP_RATE, &reg, 1);
    // Drop B1-B0
    reg &= 0xFC;
    // Get index
    uint8_t timeIx = std::distance(CAP1188_CYCLE_TIME_OPTIONS.begin(), it);
    uint8_t newReg = reg | timeIx;
    bus_.writeBlock8(address_, CAP1188_REG_AVG_SAMP_RATE, &newReg, 1);
}

std::array<uint8_t, 8> CAP1188::getThresholds() 
{
    std::array<uint8_t, 8> block;
    bus_.readBlock8(address_, CAP1188_REG_THRESH_CS1, block.data(), 8);
    return block;
}

void CAP1188::setThresholds(std::array<uint8_t, 8> newThresholds)
{
    for (int i=0; i<8; i++)
        if (newThresholds[i] < 0 || newThresholds[i] > 127)
            throw std::out_of_range("Threshold must lie between 0 and 127");
    bus_.writeBlock8(address_, CAP1188_REG_THRESH_CS1, newThresholds.data(), 8);
}

// Take a mask of pins ie 00100001 for Pins 1 & 6
// Forces reset of Calibration value
void CAP1188::recalibratePins(uint8_t pins)
{
    bus_.writeBlock8(address_, CAP1188_REG_CALIB_ACTIVATE, 
    &pins, 1);
}

int8_t CAP1188::deltaCount(int pin){
        // Pin Check
        if (pin < 0 || pin > 7)
            throw std::out_of_range("Pin must be integer 0-7");

        // Grab from pin's delta count register
        uint8_t u_delta;
        switch (pin){
            case 0: bus_.readBlock8(address_, CAP1188_REG_DELTA_COUNT_CS1, &u_delta, 1); break;
            case 1: bus_.readBlock8(address_, CAP1188_REG_DELTA_COUNT_CS2, &u_delta, 1); break;
            case 2: bus_.readBlock8(address_, CAP1188_REG_DELTA_COUNT_CS3, &u_delta, 1); break;
            case 3: bus_.readBlock8(address_, CAP1188_REG_DELTA_COUNT_CS4, &u_delta, 1); break;
            case 4: bus_.readBlock8(address_, CAP1188_REG_DELTA_COUNT_CS5, &u_delta, 1); break;
            case 5: bus_.readBlock8(address_, CAP1188_REG_DELTA_COUNT_CS6, &u_delta, 1); break;
            case 6: bus_.readBlock8(address_, CAP1188_REG_DELTA_COUNT_CS7, &u_delta, 1); break;
            case 7: bus_.readBlock8(address_, CAP1188_REG_DELTA_COUNT_CS8, &u_delta, 1); break;
            default: u_delta = 0; break; // safety fallback
            }
        // delta count is actually a signed word, convert to signed
        int8_t delta = u_delta;

        return delta;
    }
void CAP1188::disableStandby()
{
    uint8_t status = 0;
    bus_.readBlock8(address_, CAP1188_REG_MAIN_CTRL, &status, 1);
    status &= ~CAP1188_MAIN_STBY_BIT;
    bus_.writeBlock8(address_, CAP1188_REG_MAIN_CTRL, &status, 1);
}

void CAP1188::clearInterrupt()
{
    uint8_t clear = 0x00;
    bus_.writeBlock8(address_, CAP1188_REG_MAIN_CTRL, &clear, 1);
}

void CAP1188::setInterruptRepeatRate(uint8_t rate)
{
    uint8_t reg_data;
    bus_.readBlock8(address_, CAP1188_REG_SENSOR_CFG1, &reg_data, 1);
    reg_data &= 0xF0;
    reg_data |= rate;
    bus_.writeBlock8(address_, CAP1188_REG_SENSOR_CFG1, &reg_data, 1);
}

void CAP1188::enableChannels()
{
    bus_.writeBlock8(address_, CAP1188_REG_SENSOR_ENABLE, &enabledChannels_, 1);
}   

void CAP1188::enableInterruptChannels()
{
    bus_.writeBlock8(address_, CAP1188_REG_INTERRUPT_ENABLE,
                     &interruptEnabledChannels_, 1);
}