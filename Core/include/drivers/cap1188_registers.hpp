#pragma once

#include <cstdint>

// ============================================================
//  CAP1188 - 8-Channel Capacitive Touch Sensor
//  Raspberry Pi Linux I2C driver (i2c-dev / SMBus)
//
//  Register set sourced from Microchip datasheet DS00001620B
// ============================================================

// ------------------------------------------------------------
//  I2C address (set by AD pin wiring)
//  AD floating / 3V3    -> 0x29 (default)
//  AD -> 390K to GND    -> 0x28
//  AD -> 100K to GND    -> 0x2A
//  AD -> 680K to GND    -> 0x2B
//  AD -> GND            -> SPI mode only, not used here
// ------------------------------------------------------------
static constexpr uint8_t CAP1188_DEFAULT_ADDR   = 0x29;
static constexpr uint8_t CAP1188_390K_ADDR      = 0x28;         // Used for Tromboneless

// ------------------------------------------------------------
//  General Registers
// ------------------------------------------------------------
static constexpr uint8_t CAP1188_REG_MAIN_CTRL          = 0x00; // R/W  Main Control             - Controls general power states and power dissipation         Default: 0x00
static constexpr uint8_t CAP1188_REG_GENERAL_STATUS     = 0x02; // R    General Status            - Stores general status bits                                  Default: 0x00
static constexpr uint8_t CAP1188_REG_SENSOR_STATUS      = 0x03; // R    Sensor Input Status       - State of sampled capacitive touch sensor inputs             Default: 0x00
static constexpr uint8_t CAP1188_REG_LED_STATUS         = 0x04; // R    LED Status                - Stores status bits for LEDs                                 Default: 0x00
static constexpr uint8_t CAP1188_REG_NOISE_FLAG_STATUS  = 0x0A; // R    Noise Flag Status         - Stores the noise flags for sensor inputs                    Default: 0x00

// ------------------------------------------------------------
//  Delta Count Registers (read-only, one per channel)
//  Stores the difference between current count and base count.
//  A rising value indicates a touch. Compared against threshold
//  to determine if a touch event is flagged.
// ------------------------------------------------------------
static constexpr uint8_t CAP1188_REG_DELTA_COUNT_CS1    = 0x10; // R    Sensor Input 1 Delta Count - Delta count for CS1                                       Default: 0x00
static constexpr uint8_t CAP1188_REG_DELTA_COUNT_CS2    = 0x11; // R    Sensor Input 2 Delta Count - Delta count for CS2                                       Default: 0x00
static constexpr uint8_t CAP1188_REG_DELTA_COUNT_CS3    = 0x12; // R    Sensor Input 3 Delta Count - Delta count for CS3                                       Default: 0x00
static constexpr uint8_t CAP1188_REG_DELTA_COUNT_CS4    = 0x13; // R    Sensor Input 4 Delta Count - Delta count for CS4                                       Default: 0x00
static constexpr uint8_t CAP1188_REG_DELTA_COUNT_CS5    = 0x14; // R    Sensor Input 5 Delta Count - Delta count for CS5                                       Default: 0x00
static constexpr uint8_t CAP1188_REG_DELTA_COUNT_CS6    = 0x15; // R    Sensor Input 6 Delta Count - Delta count for CS6                                       Default: 0x00
static constexpr uint8_t CAP1188_REG_DELTA_COUNT_CS7    = 0x16; // R    Sensor Input 7 Delta Count - Delta count for CS7                                       Default: 0x00
static constexpr uint8_t CAP1188_REG_DELTA_COUNT_CS8    = 0x17; // R    Sensor Input 8 Delta Count - Delta count for CS8                                       Default: 0x00

// ------------------------------------------------------------
//  Sensitivity & Configuration Registers
// ------------------------------------------------------------
static constexpr uint8_t CAP1188_REG_SENSITIVITY        = 0x1F; // R/W  Sensitivity Control          - Controls sensitivity of threshold/delta counts and base count scaling  Default: 0x2F
static constexpr uint8_t CAP1188_REG_CONFIG             = 0x20; // R/W  Configuration                - Controls general functionality                          Default: 0x20
static constexpr uint8_t CAP1188_REG_SENSOR_ENABLE      = 0x21; // R/W  Sensor Input Enable          - Controls which capacitive touch sensor inputs are sampled              Default: 0xFF
static constexpr uint8_t CAP1188_REG_SENSOR_CFG1        = 0x22; // R/W  Sensor Input Configuration   - Controls max duration and auto-repeat delay (full power state)         Default: 0xA4
static constexpr uint8_t CAP1188_REG_SENSOR_CFG2        = 0x23; // R/W  Sensor Input Configuration 2 - Controls MPRESS controls for all sensor inputs          Default: 0x07
static constexpr uint8_t CAP1188_REG_AVG_SAMP_RATE      = 0x24; // R/W  Averaging and Sampling Config - Controls averaging and sampling window                 Default: 0x39
static constexpr uint8_t CAP1188_REG_CALIB_ACTIVATE     = 0x26; // R/W  Calibration Activate         - Forces re-calibration for capacitive touch sensor inputs               Default: 0x00
static constexpr uint8_t CAP1188_REG_INTERRUPT_ENABLE   = 0x27; // R/W  Interrupt Enable             - Enables interrupts associated with capacitive touch sensor inputs      Default: 0xFF
static constexpr uint8_t CAP1188_REG_REPEAT_RATE_ENABLE = 0x28; // R/W  Repeat Rate Enable           - Enables repeat rate for all sensor inputs               Default: 0xFF
static constexpr uint8_t CAP1188_REG_MULTI_TOUCH_CFG    = 0x2A; // R/W  Multiple Touch Configuration - Determines number of simultaneous touches to flag       Default: 0x80
static constexpr uint8_t CAP1188_REG_MULTI_TOUCH_PAT_CFG= 0x2B; // R/W  Multiple Touch Pattern Config- Determines MTP configuration                           Default: 0x00
static constexpr uint8_t CAP1188_REG_MULTI_TOUCH_PAT    = 0x2D; // R/W  Multiple Touch Pattern       - Determines pattern/number of sensor inputs used by MTP  Default: 0xFF
static constexpr uint8_t CAP1188_REG_RECALIB_CFG        = 0x2F; // R/W  Recalibration Configuration  - Determines re-calibration timing and sampling window    Default: 0x8A

// ------------------------------------------------------------
//  Threshold Registers (one per channel)
//  Delta count must exceed this value to register a touch.
//  Default value 0x40 = 64 counts.
// ------------------------------------------------------------
static constexpr uint8_t CAP1188_REG_THRESH_CS1         = 0x30; // R/W  Sensor Input 1 Threshold - Delta count threshold for touch on CS1                     Default: 0x40
static constexpr uint8_t CAP1188_REG_THRESH_CS2         = 0x31; // R/W  Sensor Input 2 Threshold - Delta count threshold for touch on CS2                     Default: 0x40
static constexpr uint8_t CAP1188_REG_THRESH_CS3         = 0x32; // R/W  Sensor Input 3 Threshold - Delta count threshold for touch on CS3                     Default: 0x40
static constexpr uint8_t CAP1188_REG_THRESH_CS4         = 0x33; // R/W  Sensor Input 4 Threshold - Delta count threshold for touch on CS4                     Default: 0x40
static constexpr uint8_t CAP1188_REG_THRESH_CS5         = 0x34; // R/W  Sensor Input 5 Threshold - Delta count threshold for touch on CS5                     Default: 0x40
static constexpr uint8_t CAP1188_REG_THRESH_CS6         = 0x35; // R/W  Sensor Input 6 Threshold - Delta count threshold for touch on CS6                     Default: 0x40
static constexpr uint8_t CAP1188_REG_THRESH_CS7         = 0x36; // R/W  Sensor Input 7 Threshold - Delta count threshold for touch on CS7                     Default: 0x40
static constexpr uint8_t CAP1188_REG_THRESH_CS8         = 0x37; // R/W  Sensor Input 8 Threshold - Delta count threshold for touch on CS8                     Default: 0x40
static constexpr uint8_t CAP1188_REG_NOISE_THRESHOLD    = 0x38; // R/W  Sensor Input Noise Threshold - Controls noise threshold for all sensor inputs          Default: 0x01

// ------------------------------------------------------------
//  Standby Configuration Registers
// ------------------------------------------------------------
static constexpr uint8_t CAP1188_REG_STANDBY_CHANNEL    = 0x40; // R/W  Standby Channel        - Controls which sensor inputs are enabled in standby          Default: 0x00
static constexpr uint8_t CAP1188_REG_STANDBY_CFG        = 0x41; // R/W  Standby Configuration  - Controls averaging and cycle time in standby                 Default: 0x39
static constexpr uint8_t CAP1188_REG_STANDBY_SENS       = 0x42; // R/W  Standby Sensitivity    - Controls sensitivity settings used in standby                Default: 0x02
static constexpr uint8_t CAP1188_REG_STANDBY_THRESH     = 0x43; // R/W  Standby Threshold      - Touch detection threshold for active inputs in standby       Default: 0x40
static constexpr uint8_t CAP1188_REG_CONFIG2            = 0x44; // R/W  Configuration 2        - Additional configuration controls for the device             Default: 0x40

// ------------------------------------------------------------
//  Base Count Registers (read-only, one per channel)
//  Stores the reference (baseline) capacitance for each pad,
//  established during calibration. Delta count = current - base.
// ------------------------------------------------------------
static constexpr uint8_t CAP1188_REG_BASE_COUNT_CS1     = 0x50; // R    Sensor Input 1 Base Count - Reference count value for sensor input 1                  Default: 0xC8
static constexpr uint8_t CAP1188_REG_BASE_COUNT_CS2     = 0x51; // R    Sensor Input 2 Base Count - Reference count value for sensor input 2                  Default: 0xC8
static constexpr uint8_t CAP1188_REG_BASE_COUNT_CS3     = 0x52; // R    Sensor Input 3 Base Count - Reference count value for sensor input 3                  Default: 0xC8
static constexpr uint8_t CAP1188_REG_BASE_COUNT_CS4     = 0x53; // R    Sensor Input 4 Base Count - Reference count value for sensor input 4                  Default: 0xC8
static constexpr uint8_t CAP1188_REG_BASE_COUNT_CS5     = 0x54; // R    Sensor Input 5 Base Count - Reference count value for sensor input 5                  Default: 0xC8
static constexpr uint8_t CAP1188_REG_BASE_COUNT_CS6     = 0x55; // R    Sensor Input 6 Base Count - Reference count value for sensor input 6                  Default: 0xC8
static constexpr uint8_t CAP1188_REG_BASE_COUNT_CS7     = 0x56; // R    Sensor Input 7 Base Count - Reference count value for sensor input 7                  Default: 0xC8
static constexpr uint8_t CAP1188_REG_BASE_COUNT_CS8     = 0x57; // R    Sensor Input 8 Base Count - Reference count value for sensor input 8                  Default: 0xC8

// ------------------------------------------------------------
//  LED Control Registers
// ------------------------------------------------------------
static constexpr uint8_t CAP1188_REG_LED_OUTPUT_TYPE    = 0x71; // R/W  LED Output Type              - Controls output type for LED outputs                   Default: 0x00
static constexpr uint8_t CAP1188_REG_LED_LINKING        = 0x72; // R/W  Sensor Input LED Linking     - Controls linking of sensor inputs to LED channels      Default: 0x00
static constexpr uint8_t CAP1188_REG_LED_POLARITY       = 0x73; // R/W  LED Polarity                 - Controls output polarity of LEDs                       Default: 0x00
static constexpr uint8_t CAP1188_REG_LED_OUTPUT_CTRL    = 0x74; // R/W  LED Output Control           - Controls the output state of the LEDs                  Default: 0x00
static constexpr uint8_t CAP1188_REG_LED_LINKED_TRANS   = 0x77; // R/W  LED Linked Transition Control - Controls transition when LEDs are linked to CS channels Default: 0x00
static constexpr uint8_t CAP1188_REG_LED_MIRROR_CTRL    = 0x79; // R/W  LED Mirror Control           - Controls mirroring of duty cycles for the LEDs         Default: 0x00
static constexpr uint8_t CAP1188_REG_LED_BEHAVIOUR1     = 0x81; // R/W  LED Behavior 1               - Controls behaviour and response of LEDs 1-4            Default: 0x00
static constexpr uint8_t CAP1188_REG_LED_BEHAVIOUR2     = 0x82; // R/W  LED Behavior 2               - Controls behaviour and response of LEDs 5-8            Default: 0x00
static constexpr uint8_t CAP1188_REG_LED_PULSE1_PERIOD  = 0x84; // R/W  LED Pulse 1 Period           - Controls the period of each breathe during a pulse     Default: 0x20
static constexpr uint8_t CAP1188_REG_LED_PULSE2_PERIOD  = 0x85; // R/W  LED Pulse 2 Period           - Controls the period of breathing during breathe/pulse  Default: 0x14
static constexpr uint8_t CAP1188_REG_LED_BREATHE_PERIOD = 0x86; // R/W  LED Breathe Period           - Controls the period of an LED breathe operation        Default: 0x5D
static constexpr uint8_t CAP1188_REG_LED_CONFIG         = 0x88; // R/W  LED Config                   - Controls LED configuration                             Default: 0x04
static constexpr uint8_t CAP1188_REG_LED_PULSE1_DUTY    = 0x90; // R/W  LED Pulse 1 Duty Cycle       - Min and max duty cycle for pulse operation             Default: 0xF0
static constexpr uint8_t CAP1188_REG_LED_PULSE2_DUTY    = 0x91; // R/W  LED Pulse 2 Duty Cycle       - Min and max duty cycle for breathe and pulse operation Default: 0xF0
static constexpr uint8_t CAP1188_REG_LED_BREATHE_DUTY   = 0x92; // R/W  LED Breathe Duty Cycle       - Min and max duty cycle for breathe operation           Default: 0xF0
static constexpr uint8_t CAP1188_REG_LED_DIRECT_DUTY    = 0x93; // R/W  LED Direct Duty Cycle        - Min and max duty cycle for Direct mode LED operation   Default: 0xF0
static constexpr uint8_t CAP1188_REG_LED_DIRECT_RAMP    = 0x94; // R/W  LED Direct Ramp Rates        - Rising and falling edge ramp rates of the LEDs         Default: 0x00
static constexpr uint8_t CAP1188_REG_LED_OFF_DELAY      = 0x95; // R/W  LED Off Delay                - Determines the off delay for all LED behaviours        Default: 0x00

// ------------------------------------------------------------
//  Calibration Registers (read-only, one per channel)
// ------------------------------------------------------------
static constexpr uint8_t CAP1188_REG_CALIB_CS1          = 0xB1; // R    Sensor Input 1 Calibration     - Upper 8-bit calibration value for sensor input 1    Default: 0x00
static constexpr uint8_t CAP1188_REG_CALIB_CS2          = 0xB2; // R    Sensor Input 2 Calibration     - Upper 8-bit calibration value for sensor input 2    Default: 0x00
static constexpr uint8_t CAP1188_REG_CALIB_CS3          = 0xB3; // R    Sensor Input 3 Calibration     - Upper 8-bit calibration value for sensor input 3    Default: 0x00
static constexpr uint8_t CAP1188_REG_CALIB_CS4          = 0xB4; // R    Sensor Input 4 Calibration     - Upper 8-bit calibration value for sensor input 4    Default: 0x00
static constexpr uint8_t CAP1188_REG_CALIB_CS5          = 0xB5; // R    Sensor Input 5 Calibration     - Upper 8-bit calibration value for sensor input 5    Default: 0x00
static constexpr uint8_t CAP1188_REG_CALIB_CS6          = 0xB6; // R    Sensor Input 6 Calibration     - Upper 8-bit calibration value for sensor input 6    Default: 0x00
static constexpr uint8_t CAP1188_REG_CALIB_CS7          = 0xB7; // R    Sensor Input 7 Calibration     - Upper 8-bit calibration value for sensor input 7    Default: 0x00
static constexpr uint8_t CAP1188_REG_CALIB_CS8          = 0xB8; // R    Sensor Input 8 Calibration     - Upper 8-bit calibration value for sensor input 8    Default: 0x00
static constexpr uint8_t CAP1188_REG_CALIB_LSB1         = 0xB9; // R    Sensor Input Calibration LSB 1 - 2 LSBs of calibration value for sensor inputs 1-4  Default: 0x00
static constexpr uint8_t CAP1188_REG_CALIB_LSB2         = 0xBA; // R    Sensor Input Calibration LSB 2 - 2 LSBs of calibration value for sensor inputs 5-8  Default: 0x00

// ------------------------------------------------------------
//  Identification Registers (read-only)
// ------------------------------------------------------------
static constexpr uint8_t CAP1188_REG_PRODUCT_ID         = 0xFD; // R    Product ID      - Fixed value identifying this product                               Default: 0x50
static constexpr uint8_t CAP1188_REG_MANUFACTURER_ID    = 0xFE; // R    Manufacturer ID - Fixed value identifying Microchip                                  Default: 0x5D
static constexpr uint8_t CAP1188_REG_REVISION           = 0xFF; // R    Revision        - Fixed value representing the chip revision                         Default: 0x83

// ------------------------------------------------------------
//  Known identification values
// ------------------------------------------------------------
static constexpr uint8_t CAP1188_PRODUCT_ID_VALUE       = 0x50;
static constexpr uint8_t CAP1188_MANUFACTURER_ID_VALUE  = 0x5D;
static constexpr uint8_t CAP1188_REVISION_VALUE         = 0x83;

// ------------------------------------------------------------
//  Main Control register bit masks (REG_MAIN_CTRL 0x00)
// ------------------------------------------------------------
static constexpr uint8_t CAP1188_MAIN_INT_BIT           = (1 << 0); // Interrupt flag - clear to acknowledge a touch interrupt
static constexpr uint8_t CAP1188_MAIN_DSLEEP_BIT        = (1 << 4); // Deep sleep enable
static constexpr uint8_t CAP1188_MAIN_STBY_BIT          = (1 << 5); // Standby enable
static constexpr uint8_t CAP1188_MAIN_GAIN1_BIT         = (1 << 6); // Gain bit 1 (GAIN[1:0] at bits 7:6)
static constexpr uint8_t CAP1188_MAIN_GAIN2_BIT         = (1 << 7); // Gain bit 2

// ------------------------------------------------------------
//  Sensitivity control values (bits 6:4 of REG_SENSITIVITY 0x1F)
//  Lower value = more sensitive (smaller delta required)
// ------------------------------------------------------------
static constexpr uint8_t CAP1188_SENSITIVITY_128X       = 0x00;
static constexpr uint8_t CAP1188_SENSITIVITY_64X        = 0x01;
static constexpr uint8_t CAP1188_SENSITIVITY_32X        = 0x02; // default
static constexpr uint8_t CAP1188_SENSITIVITY_16X        = 0x03;
static constexpr uint8_t CAP1188_SENSITIVITY_8X         = 0x04;
static constexpr uint8_t CAP1188_SENSITIVITY_4X         = 0x05;
static constexpr uint8_t CAP1188_SENSITIVITY_2X         = 0x06;
static constexpr uint8_t CAP1188_SENSITIVITY_1X         = 0x07;

static constexpr std::array<uint8_t, 8> CAP1188_SENSITIVITY_OPTIONS = {128, 64, 32, 16, 8, 4, 2, 1};
static constexpr std::array<uint8_t, 8> CAP1188_AVERAGING_OPTIONS = {1, 2, 4, 8, 16, 32, 64, 128};
static std::array<std::string, 4> CAP1188_SAMP_TIME_OPTIONS = {"320us", "640us", "1.28ms", "2.56ms"};
static std::array<std::string, 4> CAP1188_CYCLE_TIME_OPTIONS = {"35ms", "70ms", "105ms", "140ms"};






