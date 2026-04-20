# Tromboneless

![Tromboneless Repo](https://img.shields.io/badge/github-repo-blue?logo=github)

![release](https://img.shields.io/badge/Tromboneless-Release_1.1-blue)

<a href="https://github.com/RyanMcB8/Tromboneless/releases" alt="Latest release">
        <![release](https://img.shields.io/badge/Tromboneless-Release_1.1-blue) /></a>

![version](https://img.shields.io/badge/version-1.2.3-blue)

[![GitHub issues by-label](https://img.shields.io/github/issues/badges/shields/good%20first%20issue)](https://github.com/ryanmcb8/Tromboneless/issues?q=is%3Aissue+is%3Aopen+label%3A%22good+first+issue%22)

</p>


<img src="Documentation/Hardware/trombonelessflow.png" width="620">


This GitHub repository contains the hardware and firmware for Tromboneless - an open source device capable of measuring oral input to synthesise the output of a brass trombone using a Raspberry Pi 5, implementing MIDI protocol.

Please refer to our Wiki [wiki](https://github.com/RyanMcB8/Tromboneless/wiki) to see component, sequence and state diagrams for the MIDI implementation.

## Table of Contents
- [Getting Started](#getting-started)
- [Dependencies](#dependencies)
- [Bill of Materials](#bill-of-materials)
- [Documentation](#documentation)
- [Social Media](#social-media)
- [Authors & Contributions](#authors-and-contributions)
- [License](#license)

## Getting Started:

### Setting up the Raspberry Pi 5

1. Install Raspberry Pi OS onto your Raspberry Pi.<br>
   For set-up guidance for PiOS, please follow [this guide].(https://www.raspberrypi.com/documentation/computers/getting-started.html#installing-the-operating-system) <br>
   <br>

### Cloning

2. To clone with all submodules, run:
 ```
   git clone --recursive https://github.com/RyanMcB8/Tromboneless.git
```
   
   If the submodules do not download, run:

```
   git submodule init
   git submodule update --recursive
```
### Libraries
3. Tromboneless uses some libraries which are essential in the running of the Core and App.
   To download these on your Linux OS, run:
```
apt-get install cmake
sudo apt install libgpiod-dev
...    ADD LIBS
...    ADD LIBS
```

### Building the Core  
4. The following commands can be executed to build the exectuables. This requires CMake      version 3.07 or greater. (edit)
```
cmake .
make - j1
```

Due to the computational limitations of the Pi, we recommend only using one thread to make the executable.


## Dependencies
The Tromboneless App utilises the JUCE framework to create all the widgets necessary for operation.

 - Juce <br>
 - RTMidi <br>
 - libgpiod       (specifically for interacting with the GPIO pins on the Pi) <br>
 - PkgConfig      (for finding the necessary libraries within the users' system before compilation.)
 - gtk+-3.0       (for the backend of the app.)
 - webkit2gtk-4.1 (for cross-platform functionality for the app.)
 - Curl
 - alsa           (for audio output from the internal synth.)
 - freetype2


## Bill of Materials

Below contains the materials required to construct the Tromboneless hardware, with a circuit diagram showing how to configure it.

### Controller

| Microcontroller  | Quantity | Cost (£) |
|------------------|----------|----------|
| Raspberry Pi 5   | 1        |     58.98|
| Active Cooler for Raspberry Pi 5 (recommended)  | 1        |     4.80|


### Sensors

| Sensors                                                        | Quantity | Cost (£) |
|----------------------------------------------------------------|----------|----------|
| VL53L1X Time of Flight (ToF) Sensor Breakout                   | 1        |  20.40   |
| CAP1188 8-Key Capacitive Touch Sensor                          | 1        |  7.70    |
| ADS1115 16-bit ADC                                             | 1        |  6.49    |

Total Cost: £34.59

### Additional Components

| Additional Components                                          | Quantity | 
|----------------------------------------------------------------|----------|
| 1.0 MΩ Resistor                                               | 1        |    
| 300.0 Ω Resistor                                               | 1        |   
| USB MIDI Cable                                                 | 1        |    
| Infrared LED                                                   | 1        |    
| Photodiode                                                     | 1        |


### Circuit Diagram

Circuit diagram showing how to assemble the Tromboneless :

<img width="600" alt="image" src="Documentation/Hardware/updated_circuit_image.svg" />

## Documentation

<!-- If you wish to create your very own Tromboneless, the hardware used to create the protoype may be seen in the [Hardware](https://github.com/RyanMcB8/Tromboneless/tree/main/Documentation/Hardware) directory under Documentation. This includes a full circuit diagram.<br> -->

For documentation of the code used, please refer to [Documentation](https://github.com/RyanMcB8/Tromboneless/tree/main/Documentation), where there are both LaTex and HTML versions of documentation for this project.<br>

## Social Media
 
 - **#1 Post** on [r/Trombone](https://www.reddit.com/r/Trombone/) (17-02-26) <br>


 - **28.6k+ Views** across [r/Embedded](https://www.reddit.com/r/embedded/comments/1sgra2m/tromboneless_update/), [r/Trombone](https://www.reddit.com/r/Trombone/comments/1r6bswo/tromboneless/) and [r/linuxaudio](https://www.reddit.com/r/linuxaudio/comments/1skgn6u/the_tromboneless/).
 
Follow us for additional updates, including showcase videos and demonstrations:<br>

[Reddit](https://www.reddit.com/user/Forward_Vehicle4096/)<br>
[Instagram](https://www.instagram.com/tromboneless.tech/)<br>


## Authors and Contributions

- Ben Allen - 

- Aidan MacIntosh - 

- Ryan McBride - Internal synthesiser and envelope design, creation of the App side for calibration and changing of parameters in real-time and all associated test scripts. Developed the CMakeLists to create libraries to reduce repetitive compilations.

- Kerr McLaren - 

- Ciaran Rogers -  

## License

### ADS1115 
- The ADS1115 library was adopted from [Bernd Porr](https://github.com/berndporr), which can be sourced [here](https://github.com/berndporr/rpi_ads1115).

### ADD HERE

<!-- ## SSH Steps
The following steps are to allow for a remote access connection into your Raspberry Pi if you wish to use another computer as your input terminal.<br>

* SSH pair created on Raspberry with Located in /home/tromboneless/.ssh

1) When generating own pair, make ssh of ed25519 format: ```ssh-keygen -t ed25519```
2) Add your public key to /home/yourusername/.ssh/authorized_keys. If connected to the same network as the pi, you can do this automatically with ```ssh-copy-id yourusername@IP_Address```

### If on the same network as pi:
ssh in with eg ```ssh name@IP_Address```

### If on a different network:

1.) You need to set up your own personal tailscale account.
   Invitations to share access to the pi are made on the owner account, then you need to be logged in to your personal account when you follow the invitation link.
   After this is set up, you work from your personal account.
   
2.) Make sure your public ssh key is on the pi

3.) ssh in with ip address associated with the pi in tailscale (not the same as the default device one seen above): ```ssh name@IP_Address```.
We recommend working within vscode, as you can open a remote window from there which will have a proper GUI hosted by your local machine, but controlling the hardware on the pi. -->
