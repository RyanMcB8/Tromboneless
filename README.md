# Tromboneless

![Tromboneless Repo](https://img.shields.io/badge/github-repo-blue?logo=github)

![release](https://img.shields.io/badge/Tromboneless-Release_1.1-blue)

<a href="https://github.com/RyanMcB8/Tromboneless/releases" alt="Latest release">
        <![release](https://img.shields.io/badge/Tromboneless-Release_1.1-blue) /></a>

![version](https://img.shields.io/badge/version-1.2.3-blue)

[![GitHub issues by-label](https://img.shields.io/github/issues/badges/shields/good%20first%20issue)](https://github.com/ryanmcb8/Tromboneless/issues?q=is%3Aissue+is%3Aopen+label%3A%22good+first+issue%22)

</p>


<img src="Documentation/Hardware/trombonelessflow.png" width="500">


This GitHub repository contains the hardware and firmware for Tromboneless - an open source device capable of measuring oral input to synthesise the output of a brass trombone using a Raspberry Pi 5, implementing MIDI protocol. 

Tromboneless also contains its own internal synth, with custom control via the Tromboneless App!

Please refer to our Wiki [wiki](https://github.com/RyanMcB8/Tromboneless/wiki) to see component, sequence and state diagrams for the MIDI implementation.

## Table of Contents
- [Getting Started](#getting-started)
- [Dependencies](#dependencies)
- [Bill of Materials](#bill-of-materials)
- [Documentation](#documentation)
- [Social Media](#social-media)
- [Authors & Contributions](#authors-and-contributions)
- [License](#license)

## Set-Up Guide:



### Installing PiOS on Raspberry Pi 5

1. For set-up guidance for PiOS, please follow [this guide].(https://www.raspberrypi.com/documentation/computers/getting-started.html#installing-the-operating-system) <br>
   <br>

### Installing the Tromboneless Software

2. After installing PiOS on the Raspberry Pi 5, 

In the terminal or command line, run:

 ```
   git clone --recursive https://github.com/RyanMcB8/Tromboneless.git
```
   
   If this causes any errors, run:

```
   git submodule init
   git submodule update --recursive
```

*Note : Make sure git is installed on the Raspberry Pi 5.*

### Libraries

3. The Tromboneless software makes use of external libraries which are integral to the running of Tromboneless. 

These libraries must be installed on the Raspberry Pi 5 prior to running/building of the software.

   To download the required libraries, in the Pi terminal, run:
```
apt-get install cmake
sudo apt install libgpiod-dev
sudo apt install 

```

### Building Tromboneless Core Tests

4. The Core directory contains all real-time processing used by the tromboneless hardware to convert raw data from hardware to a corresponding range of MIDI functions with accompanying unit test executables.     

To run the software tests, run: 
```
cmake .
make - j1
```

### Building Tromboneless App

5. The App directory hosts a JUCE-based GUI which allows for the user to:
   - Adjust trombone slider length 
   - Adjust pressure sensitivity when interacting with mouthpiece
   - Transpose to alternative trombone ranges. 

To build the app, run:

```
cmake .
make - j4
```

*Note: Due to the computational limitations of the Pi, we recommend only using one thread to make the executable.* 


## Dependencies

6. The Tromboneless App uses the [JUCE](https://juce.com/) framework for all UI widgets.

| Library | Purpose |
|---|---|
| [JUCE](https://juce.com/) | UI framework |
| RTMidi | MIDI I/O |
| libgpiod | GPIO pin interaction (Raspberry Pi) |
| PkgConfig | Library detection at compile time |
| libgtk-3-dev | App backend |
| libwebkit2gtk-4.1-dev | Cross-platform support |
| libcurl4-openssl-dev | HTTP/network support |
| ALSA | Audio output for the internal synth |
| freetype2 | Font rendering |


## Assembling the Hardware:

To build the Tromboneless hardware, the following materials are required:

### Computation

|   | Quantity | Cost (£) |
|------------------|----------|----------|
| Raspberry Pi 5   | 1        |     ~58.98|
| Active Cooler for Raspberry Pi 5 (recommended)  | 1        |     4.80|

### Sensors

| Sensors                                                        | Quantity | Cost (£) |
|----------------------------------------------------------------|----------|----------|
| [VL53L1X](https://shop.pimoroni.com/products/vl53l1x-breakout?variant=12628497236051) Time of Flight (ToF) Sensor Breakout                   | 1        |  16.50   |
| [CAP1188](https://thepihut.com/products/adafruit-cap1188-8-key-capacitive-touch-sensor-breakout-i2c-or-spi?variant=27739497169&country=GB&currency=GBP&utm_medium=product_sync&utm_source=google&utm_content=sag_organic&utm_campaign=sag_organic&gad_source=5&gad_campaignid=22549809780&gclid=EAIaIQobChMI_O-VrPr8kwMVHJtQBh383QmIEAQYASABEgKftPD_BwE) 8-Key Capacitive Touch Sensor Breakout                          | 1        |  7.70    |
| [ADS1115](https://shop.pimoroni.com/products/adafruit-ads1115-16-bit-adc-4-channel-with-programmable-gain-amplifier?variant=370782375) 16-bit ADC Breakout                                            | 1        |  14.70    |

Total Sensor Cost: £38.90

### Additional Components

| Additional Components                                          | Quantity | 
|----------------------------------------------------------------|----------|
| 1.0 MΩ Resistor                                               | 1        |    
| 300.0 Ω Resistor                                               | 1        |   
| USB MIDI Cable                                                 | 1        |    
| Infrared LED                                                   | 1        |    
| Photodiode                                                     | 1        |


### Assembling the circuit

Once all materials have been collected, we can now begin assembly of the Tromboneless hardware.

Click [here](Ben link in brackets here) for step-by-step hardware assembly instructions.


<!-- <img width="600" alt="image" src="Documentation/Hardware/updated_circuit_image.svg" /> -->


## Social Media

 - **#1 Post** on [r/Trombone](https://www.reddit.com/r/Trombone/) (17-02-26) <br>

 - **#3 Post** on [r/Trombone](https://www.reddit.com/r/Trombone/) (??-02-26) <br>

 - **28.6k+ total views** across [r/Embedded](https://www.reddit.com/r/embedded/comments/1sgra2m/tromboneless_update/), [r/Trombone](https://www.reddit.com/r/Trombone/comments/1r6bswo/tromboneless/) and [r/linuxaudio](https://www.reddit.com/r/linuxaudio/comments/1skgn6u/the_tromboneless/).

 Social media strategy was aimed at directly targeting the demographic of people who would be interested in the project. 

 Upon comparison with initial Instagram analytics, we prioritised the forum-based site [Reddit](https://www.reddit.com/) as the main channel of communication between the project and target audience.  
 
Follow our page for demonstrations, updates and new development! :<br>

[Reddit](https://www.reddit.com/user/Forward_Vehicle4096/)<br>
[Instagram](https://www.instagram.com/tromboneless.tech/)<br>

## Documentation
For full documentation of the Tromboneless software, refer to the [Documentation](https://github.com/RyanMcB8/Tromboneless/tree/main/Documentation) folder, which contains both LaTeX and HTML versions.

## Authors and Contributions

- **Ben Allen** - 

- **Aidan McIntosh** - 

- **Ryan McBride** - Internal synthesiser and envelope design, creation of the App side for calibration and changing of parameters in real-time and all associated test scripts. Developed the CMakeLists to create libraries to reduce repetitive compilations.

- **Kerr McLaren** - 

- **Ciaran Rogers** -  


<!-- ### ADS1115 

- ADS1115 library adopted from [Bernd Porr](https://github.com/berndporr), which can be sourced [here](https://github.com/berndporr/rpi_ads1115). -->

###





