# Tromboneless
An open source device which is capable of measuring the uses oral input to synthesis the output a brass trombone would using the MIDI protocol.


## Steps to install:
1) Install Raspberry Pi OS onto your Raspberry Pi.<br>
   If you need a guide on how to flash a Pi with the OS, please follow this guide: <br>
   https://www.raspberrypi.com/documentation/computers/getting-started.html#installing-the-operating-system <br>
   <br>
2) clone this git repository using the command:<br>
 ```  git clone https://github.com/RyanMcB8/Tromboneless.git ```<br>

## SSH Steps

* SSH pair created on Raspberry with tromboneless.tech@gmail.com. Located in /home/tromboneless/.ssh
* No passkey
1) When generating own pair, make ssh of ed25519 format: ```ssh-keygen it ed25519```
2) Add your public key to /home/tromboneless/.ssh/authorized_keys. If connected to the same network as the pi, you can do this automatically with ```ssh-copy-id tromboneless@192.168.1.133```
3) Actually ssh in with ```ssh tromboneless@192.168.1.133```

