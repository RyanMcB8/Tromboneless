# Notes for the Hardware implementation and design #

![the Tromboneless circuit diagram cannot be displayed, try reloading the page.](./Tromboneless_Circuit_Diagram.svg "The Tromboneless Circuit Diagram")

The image above shows a circuit diagram representing the hardware and connections used to create the prototype for the Tromboneless. The chosen peripherals may be changed to suitable alternatives but the code will need to be adapted to have the correct drivers for the new devices.

## Details
### Processing
To process and host the program, the Tromboneless used a Raspberry Pi 5 with a linux build which allowed for easy navigation around the directories and remote access for remote testing purposes. As this system was built using C++, this could should be capable of being adapted to work using another main board which supports the standard language. Unfortunately, this would require changing low level drivers and pin defintions within the code base to allow it to work.

One recommendation from the Tromboneless team is to use a cooler on the device to ensure that it doesn't begin to thermal throttle. The Pi was found to crash when building the JUCE submodule without a cooler. The cooler used in development was the [Active Cooler for Raspberry Pi 5](https://www.raspberrypi.com/products/active-cooler/) from Raspberry Pi purchased though [The Pi Hut](https://thepihut.com/products/active-cooler-for-raspberry-pi-5?variant=42531603906755&country=GB&currency=GBP&utm_medium=product_sync&utm_source=google&utm_content=sag_organic&utm_campaign=sag_organic&srsltid=AfmBOopawetvQ34E0mKudIs347SPJoy7kBSFid2CSjyG0rD71liytS9ox10).

### Power
Each of the peripherals is connected directly the Raspberry Pi for both power and ground. Each of the peripherals receives 5V.

### Reading data
To read the data from the peripherals, the I2C protocol was utilised to allow for a reduction in pins for communication compared to SPI. To read the data at an appropriate time, each of the peripherals need to send a data ready interrupt to the processor for it to initiate a transfer. This allowed for a reduction in computation for the processor by utilising blocking I/O as opposed to polling for data continuously. This shifted a large amount of computation to the peripherals from the main processor freeing to complete other threads.

#### ADC
To find the amount of pressue the user is exerting into the mouthpiece (blowing), a balloon was enclosed in a tube restricting its width and reducing the light within the tube. The system developed used an IR transmitter and receiver, both of which were diodes. When the balloon expands, the amount of infra-red being reflected increases due to the light bouncing off the base of the balloon. The IR transmitter (an IR LED), was kept on persistently with the receiver having its cathode pin connected to the ADC. The cathode pin also connected to a 10K pull down resister in parallel which created a potential divider allowing the ADC to receive varying voltages as the amount of IR light is varied.

The specific ADC used was the [ADS1115](https://www.ti.com/product/ADS1115) on a development board from [the Pi hut](https://thepihut.com/products/adafruit-ads1115-16-bit-adc-4-channel-with-programmable-gain-amplifier?variant=27739204689&country=GB&currency=GBP&utm_medium=product_sync&utm_source=google&utm_content=sag_organic&utm_campaign=sag_organic&gad_source=1&gad_campaignid=22549809780&gbraid=0AAAAADfQ4GE0eUTG7A4_1ePnORtOHgtR3&gclid=EAIaIQobChMI5uO14NbPkwMVjptQBh1eBzxAEAQYASABEgIkwvD_BwE).

An alternative to this method is to use a pressure sensor but from testing, the lower end sensors did not have a large enough range to match that which a trombonist would reach during a performance.

#### ToF sensor
To detect the distance the user's hand is from the device, a Time of Flight (ToF) sensor was used. The specific sensor chosen was the [VL53LOX](https://www.st.com/en/imaging-and-photonics-solutions/vl53l0x.html) on a breakout board from [Pimoroni](https://shop.pimoroni.com/products/vl53l1x-breakout?variant=12628497236051). By addressing the devices onboard registers, the sensor could be set have the ideal range and sampling frequency for this device.

There are alternative choices for measuring the hand distance such as sonar or linear variable resistors. For this project, LiDAR was the best option for its speed, low interference, accuracy and being non contact.

#### Touch sensor
To measure the amount of pressure the user puts onto the mouthpiece to simulate the change in embouchure or 'buzz' of the trombonists lips, a capacitive touch sensor was developed. This custom sensor was created wrapping a sheet of copper and an electronically insulating material around a curved surface. This created a capacitor. The amount of pressure the user places on the device changes the distance between each of the copper layers which changes the capacitance. This capacitance may be fed directly into the touch sensor board used, the [CAP1188](https://www.microchip.com/en-us/product/cap1188) by Microchip on a breakout board [from Adafruit](https://www.adafruit.com/product/1602?srsltid=AfmBOorhr3tcgvVw9TzywvyCsA9vLKghejZftADR245YXpShDvVI9MMv).

As the touch sensor and the ToF sensor both have the same I2C address as default, a 390K resister connected to the AD pin of the touch sensor was pulled to ground changing its I2C address from 0x29 to 0x28.

### Outputs
#### MIDI
To read the MIDI data which is being output by the Pi, a MIDI compatible device such as a portable synthesiser may be connected to any of the USB-A connectors on the front of the Pi.

#### Video
The 2 mini HDMI ports on the side of the Raspberry Pi may be connected to a monitor (or monitors) to display the video output. When the app is launched, it may be seen in the video output.